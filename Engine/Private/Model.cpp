#include "Model.h"

#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Bone.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent{ pDevice, pContext } 
{
}

CModel::CModel(const CModel& rhs)
	: CComponent{ rhs },
	m_iNumMeshes { rhs.m_iNumMeshes },
	m_Meshes{ rhs.m_Meshes },
	m_iNumMaterials { rhs.m_iNumMaterials },
	m_Materials{ rhs.m_Materials },
	//m_Bones { rhs.m_Bones },
	m_iNumAnimations { rhs.m_iNumAnimations },
	//m_Animations { rhs.m_Animations },
	m_PreTransformMatrix { rhs.m_PreTransformMatrix },
	m_eMeshType { rhs.m_eMeshType }
{
	for (auto& MaterialDesc : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_AddRef(MaterialDesc.pMaterialTextures[i]);
	}

	for (auto& pBone : rhs.m_Bones)
		m_Bones.emplace_back(pBone->Clone());

	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);

	for (auto& pAnimation : rhs.m_Animations)
		m_Animations.emplace_back(pAnimation->Clone());
}

HRESULT CModel::Initialize_Prototype(CMesh::MESHTYPE eMeshType, const _char* szModelFilePath, _fmatrix PreTransformMatrix)
{
	_uint iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;//aiProcess_CalcTangentSpace | aiProcess_GenNormals;

	if (CMesh::TYPE_NONANIM == eMeshType)
		iFlag |= aiProcess_PreTransformVertices;

	m_eMeshType = eMeshType;

	m_pAIScene = m_Importer.ReadFile(szModelFilePath,  iFlag);
	if (nullptr == m_pAIScene)
		return E_FAIL;

	XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);

	if (FAILED(Ready_Bones(m_pAIScene->mRootNode, -1)))
		return E_FAIL;
	
	if (FAILED(Ready_Meshes()))
		return E_FAIL;

	if (FAILED(Ready_Materials(szModelFilePath)))
		return E_FAIL;

	if (FAILED(Ready_Animation()))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Initialize(void* pArv)
{
	return S_OK;
}

void CModel::Render(_uint iMeshIndex)
{
	m_Meshes[iMeshIndex]->Render();
}

HRESULT CModel::Ready_Meshes()
{
	m_iNumMeshes = m_pAIScene->mNumMeshes;
	for (size_t i = 0; i < m_iNumMeshes; ++i)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eMeshType, m_pAIScene->mMeshes[i], XMLoadFloat4x4(&m_PreTransformMatrix), m_Bones);
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.emplace_back(pMesh);
	}

	return S_OK;
}

HRESULT CModel::Ready_Materials(const char* pModelFilePath)
{
	m_iNumMaterials = m_pAIScene->mNumMaterials;

	for (size_t i = 0; i < m_iNumMaterials; ++i)
	{
		aiMaterial* pAiMaterial = m_pAIScene->mMaterials[i];

		MESH_MATERIAL tMaterialTexture{};

		for (_uint j = aiTextureType_DIFFUSE; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			aiString strTextureFilePath;

			if (FAILED(pAiMaterial->GetTexture(aiTextureType(j), 0, &strTextureFilePath)))
				continue;

			_char szFileName[MAX_PATH] = "";
			_char szExt[MAX_PATH] = "";
			_splitpath_s(strTextureFilePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			_char szDrive[MAX_PATH] = "";
			_char szDirectory[MAX_PATH] = "";
			_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDirectory, MAX_PATH, nullptr, 0, nullptr, 0);

			_char		szFullPath[MAX_PATH] = "";
			strcpy_s(szFullPath, szDrive);
			strcat_s(szFullPath, szDirectory);
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szExt);

			_tchar szRealFullPath[MAX_PATH] = L"";
			MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szRealFullPath, MAX_PATH);

			tMaterialTexture.pMaterialTextures[j] = CTexture::Create(m_pDevice, m_pContext, szRealFullPath, 1);

			if (nullptr == tMaterialTexture.pMaterialTextures[j])
				return E_FAIL;
		}

		m_Materials.emplace_back(tMaterialTexture);
	}

	return S_OK;
}

HRESULT CModel::Ready_Bones(const aiNode* pAINode, _int iCountBoneIndex)
{
	CBone* pBone = CBone::Create(pAINode, iCountBoneIndex);
	if (nullptr == pBone)
		return E_FAIL;

	m_Bones.emplace_back(pBone);

	_uint iParenIndex = m_Bones.size() - 1;

	for (size_t i = 0; i < pAINode->mNumChildren; ++i)
	{
		Ready_Bones(pAINode->mChildren[i], iParenIndex);
	}

	return S_OK;
}

HRESULT CModel::Ready_Animation()
{
	m_iNumAnimations = m_pAIScene->mNumAnimations;
	
	for (size_t i = 0; i < m_iNumAnimations; ++i)
	{
		CAnimation* pAnimation = CAnimation::Create(m_pAIScene->mAnimations[i], m_Bones);
		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.emplace_back(pAnimation);
	}
	
	return S_OK;
}

HRESULT CModel::Bind_Material(CShader* pShader, const char* strConstansName, _uint iMeshIndex, aiTextureType iTextureType)
{
	if (m_Materials[m_Meshes[iMeshIndex]->Get_MaterialIndex()].pMaterialTextures[iTextureType] == nullptr)
		return E_FAIL;

	return m_Materials[m_Meshes[iMeshIndex]->Get_MaterialIndex()].pMaterialTextures[iTextureType]->Bind_ShaderResource(pShader, strConstansName, 0);
}

HRESULT CModel::Bind_BoneMatrices(CShader* pShader, const char* strConstansName, _uint iMeshIndex)
{
	ZeroMemory(m_MeshBoneMatrices, sizeof(_float4x4) * 512);

	m_Meshes[iMeshIndex]->Fill_Matrices(m_Bones, m_MeshBoneMatrices);

	return pShader->Bind_Matrices(strConstansName, m_MeshBoneMatrices, 512);
}

void CModel::Play_Animation(const _float& fTimeDelta)
{
	// TransformMatrix 업데이트
	/* 현재 애니메이션의 상태에 맞도록 뼈들의 상태행렬(TransformationMatrix)을 만들고 갱신해준다. */
	m_Animations[m_tAnimDesc.iCurrentAnimIndex]->Update_TransformationMatrix(fTimeDelta, m_Bones, m_tAnimDesc.isLoop);

	//CombinedTransformMatrix 업데이트 
	for (auto& pBone : m_Bones)
		pBone->Update_CombinedTransformMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CMesh::MESHTYPE eMeshType, const _char* szModelFilePath, _fmatrix PreTransformMatrix)
{
	CModel* pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eMeshType, szModelFilePath, PreTransformMatrix)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* CModel::Clone(void* vArg)
{
	CModel* pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize(vArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CModel::Free()
{                                            
	__super::Free();

	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);

	for (auto& pBone : m_Bones)
		Safe_Release(pBone);

	for (auto& MaterialDesc : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_Release(MaterialDesc.pMaterialTextures[i]);
	}

	m_Materials.clear();

	for (auto& pMesh : m_Meshes)
		Safe_Release(pMesh);

	m_Meshes.clear();

	m_Importer.FreeScene();
}
