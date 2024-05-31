#include "Effect.h"

#include "GameInstance.h"

#include "Effect_Particle.h"
#include "Effect_Image.h"
#include "Effect_Trail.h"

CEffect::CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CBlendObject{ pDevice, pContext }
{
}

CEffect::CEffect(const CEffect& rhs)
    : CBlendObject{ rhs },
	m_EffectObjects { rhs.m_EffectObjects }
{
	for (auto& pEffect : m_EffectObjects)
		Safe_AddRef(pEffect);
}

HRESULT CEffect::Initialize_Prototype(const _char* szFileName)
{
	if (FAILED(File_Open(szFileName)))
		return E_FAIL;

    return S_OK;
}

HRESULT CEffect::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	EFFECT_DESC* pDesc = static_cast<EFFECT_DESC*>(pArg);
	 
	
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

	m_fDutation = pDesc->fDuration;
	m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(pDesc->pPlayerMatrix));
	m_pParentMatrix = pDesc->pPlayerMatrix;
	m_isFollowPlayer = pDesc->isFollowPlayer;

	for (auto& pEffect : m_EffectObjects)
	{
		CEffectObject::EFFECT_OBJECT_DESC EffectDesc{};
		EffectDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
		EffectDesc.vPos = pDesc->vPos;
		EffectDesc.vScale = pDesc->vScale;
		EffectDesc.RotationMatrix = pDesc->RotationMatrix;
		
		EffectDesc.iShaderPass = pDesc->iShaderPass;
		EffectDesc.isTrailMove = pDesc->isTrailMove;
		EffectDesc.fSpeed = pDesc->fSpeed;
		EffectDesc.iMoveType = pDesc->iMoveType;
		EffectDesc.fDuration = pDesc->fDuration;

		CGameObject* pEffectObject = pEffect->Clone(&EffectDesc);
		if (nullptr == pEffectObject)
			return E_FAIL;

		m_CloneEffectObject.emplace_back(pEffectObject);
	}

    return S_OK;
}

void CEffect::Priority_Tick(const _float& fTimeDelta)
{
	for (auto& pEffect : m_CloneEffectObject)
		pEffect->Priority_Tick(fTimeDelta);
}

void CEffect::Tick(const _float& fTimeDelta)
{
	m_fCurrentTime += fTimeDelta;

	if (m_fCurrentTime > m_fDutation)
		m_isDead = true;

	for (auto& pEffect : m_CloneEffectObject)
		pEffect->Tick(fTimeDelta);
}

void CEffect::Late_Tick(const _float& fTimeDelta)
{
	if (m_isFollowPlayer)
	{
		m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(m_pParentMatrix));
	}

	for (auto& pEffect : m_CloneEffectObject)
		pEffect->Late_Tick(fTimeDelta);
}

HRESULT CEffect::Render()
{
    return S_OK;
}

HRESULT CEffect::File_Open(const _char* szFileName)
{
	ifstream ifs(szFileName, ios::binary | ios::in);

	if (ifs.fail())
		return E_FAIL;

	_uint iNumObjects = 0;
	ifs.read((_char*)&iNumObjects, sizeof(_uint));

	//이펙트들 모두 저장
	for (size_t i = 0; i < iNumObjects; ++i)
	{
		_uint iEffectType = 0;
		ifs.read((_char*)&iEffectType, sizeof(_uint));

		if (iEffectType == EFFECT_PARTICLE)
		{
			CEffect_Particle::EFFECT_PARTICLE_DESC Desc{};
			ifs.read((_char*)&Desc.iNumInstance, sizeof(_uint));
			ifs.read((_char*)&Desc.iParticleType, sizeof(_int));
			ifs.read((_char*)&Desc.isLoop, sizeof(_bool));
			ifs.read((_char*)&Desc.vLifeTime, sizeof(_float2));
			ifs.read((_char*)&Desc.vOffsetPos, sizeof(_float3));
			ifs.read((_char*)&Desc.vPivotPos, sizeof(_float3));
			ifs.read((_char*)&Desc.vRange, sizeof(_float3));
			ifs.read((_char*)&Desc.vSize, sizeof(_float2));
			ifs.read((_char*)&Desc.vSpeed, sizeof(_float2));
			if (Desc.iParticleType == CEffect_Particle::PARTICLE_HELIX) ifs.read((_char*)&Desc.vPower, sizeof(_float2));

			//파티클 파일 이름
			_char strName[MAX_PATH] = "";
			_uint iFileNameSize = 0;
			ifs.read((_char*)&iFileNameSize, sizeof(_uint));
			ifs.read(strName, iFileNameSize);
			strcpy_s(Desc.szFileName, strName);

			//파티클 파일 경로
			_char strFilePath[MAX_PATH] = "";
			_uint iFilePathSize = 0;
			ifs.read((_char*)&iFilePathSize, sizeof(_uint));
			ifs.read(strFilePath, iFilePathSize);
			strcpy_s(Desc.szFilePath, strFilePath);

			//월드행렬
			_float4x4 WorldMatrix = {};
			ifs.read((_char*)&WorldMatrix, sizeof(_float4x4));
			Desc.WorldMatrix = WorldMatrix;

			//쉐이더 패스
			_uint iShaderPass = 0;
			ifs.read((_char*)&iShaderPass, sizeof(_uint));
			Desc.iShaderPass = iShaderPass;

			//파일 이름 저장 
			CGameObject* pParticle = CEffect_Particle::Create(m_pDevice, m_pContext, &Desc);
			if (nullptr == pParticle)
				return E_FAIL;

			m_EffectObjects.emplace_back(pParticle);
		}
		else if (iEffectType == EFFECT_IMG)
		{
			CEffect_Image::EFFECT_IMAGE_DESC Desc;

			//텍스쳐 파일 이름
			_char szName[MAX_PATH] = "";
			_uint iFileNameSize = 0;
			ifs.read((_char*)&iFileNameSize, sizeof(_uint));
			ifs.read(szName, iFileNameSize);
			strcpy_s(Desc.szFileName, szFileName);
			//파일 경로
			_char szFilePath[MAX_PATH] = "";
			_uint iFilePathSize = 0;
			ifs.read((_char*)&iFilePathSize, sizeof(_uint));
			ifs.read(szFilePath, iFilePathSize);
			strcpy_s(Desc.szFilePath, szFilePath);
			//월드행렬
			_float4x4 WorldMatrix;
			ifs.read((_char*)&WorldMatrix, sizeof(_float4x4));
			Desc.WorldMatrix = WorldMatrix;

			//이미지 갯수
			const _uint iTextureNum = 0;
			ifs.read((_char*)&iTextureNum, sizeof(_uint));

			//쉐이더 패스
			_uint iShaderPass = 0;
			ifs.read((_char*)&iShaderPass, sizeof(_uint));
			Desc.iShaderPass = iShaderPass;

			CGameObject* pEffect = CEffect_Image::Create(m_pDevice, m_pContext, &Desc);
			if (nullptr == pEffect)
				return E_FAIL;

			m_EffectObjects.emplace_back(pEffect);

		}
		else if (iEffectType == EFFECT_MESH)
		{
			////파일 이름
			//_char strName[MAX_PATH] = "";
			//_uint iFileNameSize = 0;
			//ifs.read((_char*)&iFileNameSize, sizeof(_uint));
			//ifs.read(strName, iFileNameSize);

			////파일 경로
			//_char strFilePath[MAX_PATH] = "";
			//_uint iFilePathSize = 0;
			//ifs.read((_char*)&iFilePathSize, sizeof(_uint));
			//ifs.read(strFilePath, iFilePathSize);

			////메쉬 이름
			//_char strMeshFileName[MAX_PATH] = "";
			//_uint iMeshFileNameSize = 0;
			//ifs.read((_char*)&iMeshFileNameSize, sizeof(_uint));
			//ifs.read(strMeshFileName, iMeshFileNameSize);

			////메쉬 파일 경로
			//_char strMeshFilePath[MAX_PATH] = "";
			//_uint iMeshFilePathSize = 0;
			//ifs.read((_char*)&iMeshFilePathSize, sizeof(_uint));
			//ifs.read(strMeshFilePath, iMeshFilePathSize);

			////월드행렬
			//_float4x4 WorldMatrix;
			//ifs.read((_char*)&WorldMatrix, sizeof(_float4x4));

			//CEffect_Mesh::EFFECT_MESH_DESC Desc{};
			//strcpy_s(Desc.szModelFilePath, strMeshFilePath); // 메쉬 경로
			//strcpy_s(Desc.strTextureFilePath, strFilePath); // 텍스쳐 경로

			////텍스쳐 이름
			//string stringFileName = strName;
			//wstring wstrFileName = L"";
			//wstrFileName.assign(stringFileName.begin(), stringFileName.end());
			//Desc.strTextureFileName = wstrFileName;

			////메쉬 이름
			//string stringMeshFileName = strMeshFileName;
			//Desc.strModelFileName = stringMeshFileName;

			//Desc.iEffectType = iEffectType;

			//Add_Effect(EFFECT_TYPE(iEffectType), &Desc);
		}
		else if (iEffectType == EFFECT_TRAIL)
		{
			//파일 이름
			_char strName[MAX_PATH] = "";
			_uint iFileNameSize = 0;
			ifs.read((_char*)&iFileNameSize, sizeof(_uint));
			ifs.read(strName, iFileNameSize);

			//파일 경로
			_char strFilePath[MAX_PATH] = "";
			_uint iFilePathSize = 0;
			ifs.read((_char*)&iFilePathSize, sizeof(_uint));
			ifs.read(strFilePath, iFilePathSize);

			//메쉬 이름
			_char strMeshFileName[MAX_PATH] = "";
			_uint iMeshFileNameSize = 0;
			ifs.read((_char*)&iMeshFileNameSize, sizeof(_uint));
			ifs.read(strMeshFileName, iMeshFileNameSize);

			//메쉬 파일 경로
			_char strMeshFilePath[MAX_PATH] = "";
			_uint iMeshFilePathSize = 0;
			ifs.read((_char*)&iMeshFilePathSize, sizeof(_uint));
			ifs.read(strMeshFilePath, iMeshFilePathSize);

			//마스크
			_bool isMask = false;
			ifs.read((_char*)&isMask, sizeof(_bool));

			_char szMaskFilePath[MAX_PATH] = "";

			if (isMask)
			{
				//마스크 파일 경로
				_uint iMaskFilePathSize = 0;
				ifs.read((_char*)&iMaskFilePathSize, sizeof(_uint));
				ifs.read(szMaskFilePath, iMaskFilePathSize);
			}

			//월드행렬
			_float4x4 WorldMatrix = {};
			ifs.read((_char*)&WorldMatrix, sizeof(_float4x4));

			//쉐이더 패스
			_uint iShaderPass = 0;
			ifs.read((_char*)&iShaderPass, sizeof(_uint));

			CEffect_Trail::EFFECT_TRAIL_DESC Desc{};
			strcpy_s(Desc.strFileName, strName); // 텍스쳐 이름
			strcpy_s(Desc.strMeshName, strMeshFileName); // 메쉬 이름
			strcpy_s(Desc.strMeshFilePath, strMeshFilePath); // 메쉬 경로
			strcpy_s(Desc.strFilePath, strFilePath); // 텍스쳐 경로
			strcpy_s(Desc.strMaskFilePath, szMaskFilePath); // 마스크 경로
			Desc.WorldMatrix = WorldMatrix;
			Desc.iShaderPass = iShaderPass;
			Desc.isMask = isMask;

			CGameObject* pEffect = CEffect_Trail::Create(m_pDevice, m_pContext, &Desc);
			if (nullptr == pEffect)
				return E_FAIL;

			m_EffectObjects.emplace_back(pEffect);
		}
	}

	ifs.close();

	return S_OK;
}

CEffect* CEffect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* szFilePath)
{
	CEffect* pInstance = new CEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(szFilePath)))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CEffect::Clone(void* pArg)
{
	CEffect* pInstance = new CEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MessageBox(g_hWnd, L"CEffect Clone Create Failur", nullptr, 0);
		Safe_Release(pInstance);
	}
		

	return pInstance;
}

void CEffect::Free()
{
    __super::Free();

	for (auto& pEffect : m_EffectObjects)
		Safe_Release(pEffect);
	m_EffectObjects.clear();

	for (auto& pEffect : m_CloneEffectObject)
		Safe_Release(pEffect);
	m_CloneEffectObject.clear();

	m_EffectObjects.clear();

}
