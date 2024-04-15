#include "Model.h"

#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Channel.h"

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
	m_eMeshType { rhs.m_eMeshType },
	m_iRootBoneIndex { rhs.m_iRootBoneIndex }
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

HRESULT CModel::Initialize_Prototype(CMesh::MESHTYPE eMeshType, const _char* szModelFilePath, _fmatrix PreTransformMatrix, const _char* szBinaryFilePath, CREATETYPE eCreateType)
{
	m_eMeshType = eMeshType;

	XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);

	if (eCreateType == CREATE_READ)
	{
 		if (FAILED(Ready_Model(szModelFilePath, szBinaryFilePath)))
			return E_FAIL;

		return S_OK;
	}
	_uint iFlag = aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace | aiProcess_GenNormals;

	if (CMesh::TYPE_NONANIM == eMeshType)
		iFlag |= aiProcess_PreTransformVertices;

	m_pAIScene = m_Importer.ReadFile(szModelFilePath,  iFlag);
	if (nullptr == m_pAIScene)
		return E_FAIL;

	if (FAILED(Ready_Bones(m_pAIScene->mRootNode, -1)))
		return E_FAIL;
	
	if (FAILED(Ready_Meshes()))
		return E_FAIL;

	m_iNumMaterials = m_pAIScene->mNumMaterials;
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

CTexture* CModel::Ready_Materials(const char* pModelFilePath, const char* pFilePath)
{
	MESH_MATERIAL tMaterialTexture{};

	if (strlen(pFilePath) == 0)
		return nullptr;

	_char szFileName[MAX_PATH] = "";
	_char szExt[MAX_PATH] = "";
	_splitpath_s(pFilePath, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

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

	CTexture* pTexture = CTexture::Create(m_pDevice, m_pContext, szRealFullPath, 1);

	if (nullptr == pTexture)
		return nullptr;

	return pTexture;
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

HRESULT CModel::Ready_Model(const _char* szModelFilePath, const _char* szBinaryFilePath)
{
	ifstream ifs;
	ifs.open(szBinaryFilePath, ios::in | ios::binary);

 	if (!ifs.is_open())
		return E_FAIL;

	//Model
	ifs.read((_char*)&m_eMeshType, sizeof(_int));

	//Bones
	_int iNumBones = { 0 };
	ifs.read((_char*)&iNumBones, sizeof(_int));

	for (size_t i = 0; i < iNumBones; ++i)
	{
		_char pName[MAX_PATH] = "";
		_int iNumName = { 0 };

		ifs.read((_char*)&iNumName, sizeof(_int));
		ifs.read((_char*)pName, iNumName);

		_int iParentIndex = { 0 };
		ifs.read((_char*)&iParentIndex, sizeof(_int));

		_float4x4 TransformationMatrix;
		ifs.read((_char*)&TransformationMatrix, sizeof(_float4x4));

		CBone* pBone = CBone::Create(pName, iParentIndex, XMLoadFloat4x4(&TransformationMatrix));
		if (nullptr == pBone)
			return E_FAIL;

		m_Bones.emplace_back(pBone);
	}

	_uint iRootBoneIndex = { 0 };
	find_if(m_Bones.begin(), m_Bones.end(), [&](CBone* pBone)->_bool {
		if (pBone->Compare_NodeName("Bip001"))
			return true;

		iRootBoneIndex++;
		return false;
	});

	m_iRootBoneIndex = iRootBoneIndex;

	//Mesh
	ifs.read((_char*)&m_iNumMeshes, sizeof(_int));

	for (size_t i = 0; i < m_iNumMeshes; ++i)
	{
		_char pName[MAX_PATH] = "";
		_int iNumName = { 0 };

		ifs.read((char*)&iNumName, sizeof(_int));
		ifs.read((char*)pName, iNumName);

		_int iMaterialIndex = { 0 };
		ifs.read((_char*)&iMaterialIndex, sizeof(_int));

		_int iNumVertices = { 0 };
		ifs.read((_char*)&iNumVertices, sizeof(_int));

		_int iNumIndices = { 0 };
		ifs.read((_char*)&iNumIndices, sizeof(_int));

		int iNumFaces = { 0 };
		ifs.read((_char*)&iNumFaces, sizeof(_int));

		unsigned int* Indices = new unsigned int[iNumIndices];
		ZeroMemory(Indices, sizeof(unsigned int) * iNumIndices);

		ifs.read((_char*)&Indices[0], sizeof(_uint) * iNumIndices);

		vector<VTXANIMMESH> AnimMeshed;
		AnimMeshed.resize(iNumVertices);
		_int iNumBones = { 0 };
		vector<_float4x4> OffsetMatrices;
		
		vector<_int> BoneIndices;
		
		_int iNumWeight = { 0 };

		vector<VTXMESH> Meshed;
		Meshed.resize(iNumVertices);
		if (m_eMeshType == CMesh::TYPE_ANIM)
		{
			ifs.read((_char*)&AnimMeshed[0], sizeof(VTXANIMMESH) * iNumVertices);

			ifs.read((_char*)&iNumBones, sizeof(_int));

			OffsetMatrices.resize(iNumBones);
			BoneIndices.resize(iNumBones);
			ifs.read((_char*)&OffsetMatrices[0], sizeof(_float4x4) * iNumBones);
			ifs.read((_char*)&BoneIndices[0], sizeof(_int) * iNumBones);

			ifs.read((_char*)&iNumWeight, sizeof(_int));
		}
		else
		{
			ifs.read((_char*)&Meshed[0], sizeof(VTXMESH) * iNumVertices);
		}

		//Anim & NonAnim 구분하기
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eMeshType, 
			XMLoadFloat4x4(&m_PreTransformMatrix), pName, iMaterialIndex, iNumVertices, iNumIndices, 
			iNumFaces, Indices, AnimMeshed, Meshed, iNumBones, OffsetMatrices, 
			BoneIndices, iNumWeight, m_Bones);

		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.emplace_back(pMesh);
	}

	//Animation
	ifs.read((_char*)&m_iNumAnimations, sizeof(_int));

	for (size_t i = 0; i < m_iNumAnimations; ++i)
	{
		_char pName[MAX_PATH] = "";
		_int iNumName = { 0 };

		ifs.read((char*)&iNumName, sizeof(_int));
		ifs.read((char*)pName, iNumName);

		_double Duration = { 0.0 };
		ifs.read((char*)&Duration, sizeof(_double));

		_double TickPerSecond = { 0.0 };
		ifs.read((char*)&TickPerSecond, sizeof(_double));

		_uint iNumChannels = { 0 };
		ifs.read((char*)&iNumChannels, sizeof(_uint));

		vector<CChannel*> Channels;

		for (size_t j = 0; j < iNumChannels; ++j)
		{
			_char pChannelName[MAX_PATH] = "";
			_int iNumChannelName = { 0 };

			ifs.read((char*)&iNumChannelName, sizeof(_int));
			ifs.read((char*)pChannelName, iNumChannelName);

			_int iBoneIndex = { 0 };
			ifs.read((_char*)&iBoneIndex, sizeof(_int));

			_uint iNumKeyFrames = { 0 };
			ifs.read((_char*)&iNumKeyFrames, sizeof(_uint));

			vector<KEYFRAME> KeyFrames;
			KeyFrames.resize(iNumKeyFrames);
			ifs.read((_char*)&KeyFrames[0], sizeof(KEYFRAME) * iNumKeyFrames);

			CChannel* pChannel = CChannel::Create(pChannelName, iBoneIndex, iNumKeyFrames, KeyFrames);
			if (pChannel == nullptr)
				return E_FAIL;

			Channels.emplace_back(pChannel);
		}

		CAnimation* pAnimation = CAnimation::Create(pName, Duration, TickPerSecond, iNumChannels, Channels);
		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.emplace_back(pAnimation);
	}

	ifs.read((_char*)&m_iNumMaterials, sizeof(_int));

	vector<_char*> pAiMaterial;
	MESH_MATERIAL tMaterialTexture{};
	for (size_t i = 0; i < m_iNumMaterials; ++i)
	{
		for (size_t j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			_char pFilePath[MAX_PATH] = "";
			_int iNumName = strlen(pFilePath) + 1;

			ifs.read((_char*)&iNumName, sizeof(_int));
			ifs.read((_char*)pFilePath, iNumName);

			tMaterialTexture.pMaterialTextures[j] = Ready_Materials(szModelFilePath, pFilePath);
		}
		m_Materials.emplace_back(tMaterialTexture);
	}

 	ifs.close();

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

void CModel::Play_Animation(const _float& fTimeDelta, _float4* vMovePos)
{
	if (m_Animations[m_tAnimDesc.iCurrentAnimIndex]->IsFirst())
	{
		m_Animations[m_tAnimDesc.iCurrentAnimIndex]->Linear_TransformationMatrix(fTimeDelta, m_Bones);

		for (auto& pBone : m_Bones)
			pBone->Update_CombinedTransformMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));

		XMStoreFloat4(vMovePos, XMVectorSet(0.f, 0.f, 0.f, 1.f));
		return;
	}
	else
		m_Animations[m_tAnimDesc.iCurrentAnimIndex]->Update_TransformationMatrix(fTimeDelta, m_Bones, m_tAnimDesc.isLoop);

	for (auto& pBone : m_Bones)
		pBone->Update_CombinedTransformMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));
}

void CModel::Play_Animation(const _float& fTimeDelta, _float4* vMovePos, _bool isInterpolation)
{
	if (m_Animations[m_tAnimDesc.iCurrentAnimIndex]->IsFirst() && isInterpolation)
	{
		m_Animations[m_tAnimDesc.iCurrentAnimIndex]->Linear_TransformationMatrix(fTimeDelta, m_Bones);

		for (auto& pBone : m_Bones)
			pBone->Update_CombinedTransformMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));
		
		XMStoreFloat4(vMovePos, XMVectorSet(0.f, 0.f, 0.f, 1.f));
		return;
	}
	else
		m_Animations[m_tAnimDesc.iCurrentAnimIndex]->Update_TransformationMatrix(fTimeDelta, m_Bones, m_tAnimDesc.isLoop);

	for (auto& pBone : m_Bones)
		pBone->Update_CombinedTransformMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));

	if (!Get_LoopAnimation_Finished())
	{
		const _float4x4* CombinedTransformMatrix = m_Bones[m_iRootBoneIndex]->Get_CombinedTransformMatrix();
		XMStoreFloat4(vMovePos, XMVectorSet(CombinedTransformMatrix->m[3][0], CombinedTransformMatrix->m[3][1], CombinedTransformMatrix->m[3][2] * -1.f, 1.f));
	}

	return;
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CMesh::MESHTYPE eMeshType, const _char* szModelFilePath, _fmatrix PreTransformMatrix, const _char* szBinaryFilePath, CREATETYPE eCreateType)
{
	CModel* pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eMeshType, szModelFilePath, PreTransformMatrix, szBinaryFilePath, eCreateType)))
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
