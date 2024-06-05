#include "Effect.h"

#include "MainApp.h"
#include "GameInstance.h"

#include "EffectObject.h"
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

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(pDesc->pPlayerMatrix));
	m_pParentMatrix = pDesc->pPlayerMatrix;
	m_isFollowPlayer = pDesc->isFollowPlayer;
	m_fDutation = pDesc->fDuration;
	m_isBullet = pDesc->isBullet;
	m_vTargetDir = pDesc->vTargetDir;
	m_fSpeed = pDesc->fSpeed;

	for (auto& pEffect : m_EffectObjects)
	{
		CEffectObject::EFFECT_OBJECT_DESC EffectDesc{};
		EffectDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
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

	if (m_isBullet)
	{
		// 총알이라면 타겟의 방향으로 이동하기.

		_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		vPos += m_vTargetDir * m_fSpeed * fTimeDelta;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
	}

	for (auto& pEffect : m_CloneEffectObject)
		pEffect->Tick(fTimeDelta);

	m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void CEffect::Late_Tick(const _float& fTimeDelta)
{
	if (m_isFollowPlayer)
	{
		m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(m_pParentMatrix));
	}

	for (auto& pEffect : m_CloneEffectObject)
		pEffect->Late_Tick(fTimeDelta);

#ifdef _DEBUG
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONLIGHT, this);
#endif // _DEBUG
}

HRESULT CEffect::Render()
{
#ifdef _DEBUG
	m_pColliderCom->Render();
#endif // _DEBUG

    return S_OK;
}

HRESULT CEffect::Add_Components()
{
	CBounding_AABB::BOUNDING_AABB_DESC BoundingBoxDesc{};
	BoundingBoxDesc.eType = CCollider::COLLIDER_AABB;
	BoundingBoxDesc.vExtents = _float3(0.5f, 0.5f, 0.5f);
	BoundingBoxDesc.vCenter = _float3(0.f, BoundingBoxDesc.vExtents.y, 0.f);

	if (FAILED(Add_Component(CMainApp::g_iCurrentLevel, L"Prototype_Component_Collider", L"Com_Collider", reinterpret_cast<CComponent**>(&m_pColliderCom), &BoundingBoxDesc)))
		return E_FAIL;

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

		if (iEffectType == CEffectObject::EFFECT_PARTICLE)
		{
			// 파티클 옵션
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
			ifs.read((_char*)&Desc.vPower, sizeof(_float2));
			Desc.iEffectType = iEffectType;

			//파일 경로
			_char strFilePath[MAX_PATH] = "";
			_uint iFilePathSize = 0;
			ifs.read((_char*)&iFilePathSize, sizeof(_uint));
			ifs.read(strFilePath, iFilePathSize);
			strcpy_s(Desc.strTextureFilePath, strFilePath);

			//마스크 경로
			_bool isMask = false;
			ifs.read((_char*)&isMask, sizeof(_bool));
			Desc.isMask = isMask;

			_char szMaskFilePath[MAX_PATH] = "";
			_uint iMaskFilePathSize = 0;
			ifs.read((_char*)&iMaskFilePathSize, sizeof(_uint));
			ifs.read(szMaskFilePath, iMaskFilePathSize);
			Desc.strMaskFilePath = szMaskFilePath;

			//노이즈 경로
			_bool isNoise = false;
			ifs.read((_char*)&isNoise, sizeof(_bool));
			Desc.isNoise = isNoise;

			_char szNoiseFilePath[MAX_PATH] = "";
			_uint iNoiseFilePathSize = 0;
			ifs.read((_char*)&iNoiseFilePathSize, sizeof(_uint));
			ifs.read(szNoiseFilePath, iNoiseFilePathSize);
			Desc.strNoiseFilePath = szNoiseFilePath;

			//이미지 갯수
			_uint iTextureNum = 0.f;
			ifs.read((_char*)&iTextureNum, sizeof(_uint));
			Desc.iTextureNum = iTextureNum;

			//색상
			_float4 vColor = {};
			ifs.read((_char*)&vColor, sizeof(_float4));
			Desc.vColor = vColor;

			//렌더러 타입
			_uint iRendererType = 0;
			ifs.read((_char*)&iRendererType, sizeof(_uint));
			Desc.iRendererType = iRendererType;

			//프레임 루프
			_bool isFrameLoop = false;
			ifs.read((_char*)&isFrameLoop, sizeof(_bool));
			Desc.isFrameLoop = isFrameLoop;

			//시작 시간
			_float fStartTime = 0.f;
			ifs.read((_char*)&fStartTime, sizeof(_float));
			Desc.fStartTime = fStartTime;

			//끝 시간
			_float fDrationTime = 0.f;
			ifs.read((_char*)&fDrationTime, sizeof(_float));
			Desc.fDurationTime = fDrationTime;

			//월드행렬
			_float4x4 WorldMatrix = {};
			ifs.read((_char*)&WorldMatrix, sizeof(_float4x4));
			Desc.WorldMatrix = XMLoadFloat4x4(&WorldMatrix);

			//쉐이더 패스
			_uint iShaderPass = 0;
			ifs.read((_char*)&iShaderPass, sizeof(_uint));
			Desc.iShaderPass = iShaderPass;

			Desc.iEffectType = iEffectType;


			//파일 이름 저장 
			CGameObject* pParticle = CEffect_Particle::Create(m_pDevice, m_pContext, &Desc);
			if (nullptr == pParticle)
				return E_FAIL;

			m_EffectObjects.emplace_back(pParticle);
		}
		else if (iEffectType == CEffectObject::EFFECT_IMG)
		{
			CEffect_Image::EFFECT_IMAGE_DESC Desc{};

			_uint iTextureMoveType = 0;
			ifs.read((_char*)&iTextureMoveType, sizeof(_uint));
			Desc.iTextureMoveType = iTextureMoveType;

			//파일 경로
			_char strFilePath[MAX_PATH] = "";
			_uint iFilePathSize = 0;
			ifs.read((_char*)&iFilePathSize, sizeof(_uint));
			ifs.read(strFilePath, iFilePathSize);
			strcpy_s(Desc.strTextureFilePath, strFilePath);

			//마스크 경로
			_bool isMask = false;
			ifs.read((_char*)&isMask, sizeof(_bool));
			Desc.isMask = isMask;

			_char szMaskFilePath[MAX_PATH] = "";
			_uint iMaskFilePathSize = 0;
			ifs.read((_char*)&iMaskFilePathSize, sizeof(_uint));
			ifs.read(szMaskFilePath, iMaskFilePathSize);
			Desc.strMaskFilePath = szMaskFilePath;

			//노이즈 경로
			_bool isNoise = false;
			ifs.read((_char*)&isNoise, sizeof(_bool));
			Desc.isNoise = isNoise;

			_char szNoiseFilePath[MAX_PATH] = "";
			_uint iNoiseFilePathSize = 0;
			ifs.read((_char*)&iNoiseFilePathSize, sizeof(_uint));
			ifs.read(szNoiseFilePath, iNoiseFilePathSize);
			Desc.strNoiseFilePath = szNoiseFilePath;

			//이미지 갯수
			_uint iTextureNum = 0.f;
			ifs.read((_char*)&iTextureNum, sizeof(_uint));
			Desc.iTextureNum = iTextureNum;

			//색상
			_float4 vColor = {};
			ifs.read((_char*)&vColor, sizeof(_float4));
			Desc.vColor = vColor;

			//렌더러 타입
			_uint iRendererType = 0;
			ifs.read((_char*)&iRendererType, sizeof(_uint));
			Desc.iRendererType = iRendererType;

			//프레임 루프
			_bool isFrameLoop = false;
			ifs.read((_char*)&isFrameLoop, sizeof(_bool));
			Desc.isFrameLoop = isFrameLoop;

			//시작 시간
			_float fStartTime = 0.f;
			ifs.read((_char*)&fStartTime, sizeof(_float));
			Desc.fStartTime = fStartTime;

			//끝 시간
			_float fDrationTime = 0.f;
			ifs.read((_char*)&fDrationTime, sizeof(_float));
			Desc.fDurationTime = fDrationTime;

			//월드행렬
			_float4x4 WorldMatrix = {};
			ifs.read((_char*)&WorldMatrix, sizeof(_float4x4));
			Desc.WorldMatrix = XMLoadFloat4x4(&WorldMatrix);

			//쉐이더 패스
			_uint iShaderPass = 0;
			ifs.read((_char*)&iShaderPass, sizeof(_uint));
			Desc.iShaderPass = iShaderPass;

			Desc.iEffectType = iEffectType;

			CGameObject* pEffect = CEffect_Image::Create(m_pDevice, m_pContext, &Desc);
			if (nullptr == pEffect)
				return E_FAIL;

			m_EffectObjects.emplace_back(pEffect);

		}
		else if (iEffectType == CEffectObject::EFFECT_TRAIL)
		{
			CEffect_Trail::EFFECT_TRAIL_DESC Desc{};

			//메쉬 파일 경로
			_char szMeshFilePath[MAX_PATH] = "";
			_uint iMeshFilePathSize = 0;
			ifs.read((_char*)&iMeshFilePathSize, sizeof(_uint));
			ifs.read(szMeshFilePath, iMeshFilePathSize);
			strcpy_s(Desc.strMeshFilePath, szMeshFilePath);

			//메쉬 타입
			_uint iTrailMoveType = 0;
			ifs.read((_char*)&iTrailMoveType, sizeof(_uint));
			Desc.iTrailType = iTrailMoveType;

			//파일 경로
			_char strFilePath[MAX_PATH] = "";
			_uint iFilePathSize = 0;
			ifs.read((_char*)&iFilePathSize, sizeof(_uint));
			ifs.read(strFilePath, iFilePathSize);
			strcpy_s(Desc.strTextureFilePath, strFilePath);

			//마스크 경로
			_bool isMask = false;
			ifs.read((_char*)&isMask, sizeof(_bool));
			Desc.isMask = isMask;

			_char szMaskFilePath[MAX_PATH] = "";
			_uint iMaskFilePathSize = 0;
			ifs.read((_char*)&iMaskFilePathSize, sizeof(_uint));
			ifs.read(szMaskFilePath, iMaskFilePathSize);
			Desc.strMaskFilePath = szMaskFilePath;

			//노이즈 경로
			_bool isNoise = false;
			ifs.read((_char*)&isNoise, sizeof(_bool));
			Desc.isNoise = isNoise;

			_char szNoiseFilePath[MAX_PATH] = "";
			_uint iNoiseFilePathSize = 0;
			ifs.read((_char*)&iNoiseFilePathSize, sizeof(_uint));
			ifs.read(szNoiseFilePath, iNoiseFilePathSize);
			Desc.strNoiseFilePath = szNoiseFilePath;

			//이미지 갯수
			_uint iTextureNum = 0.f;
			ifs.read((_char*)&iTextureNum, sizeof(_uint));
			Desc.iTextureNum = iTextureNum;

			//색상
			_float4 vColor = {};
			ifs.read((_char*)&vColor, sizeof(_float4));
			Desc.vColor = vColor;

			//렌더러 타입
			_uint iRendererType = 0;
			ifs.read((_char*)&iRendererType, sizeof(_uint));
			Desc.iRendererType = iRendererType;

			//프레임 루프
			_bool isFrameLoop = false;
			ifs.read((_char*)&isFrameLoop, sizeof(_bool));
			Desc.isFrameLoop = isFrameLoop;

			//시작 시간
			_float fStartTime = 0.f;
			ifs.read((_char*)&fStartTime, sizeof(_float));
			Desc.fStartTime = fStartTime;

			//끝 시간
			_float fDrationTime = 0.f;
			ifs.read((_char*)&fDrationTime, sizeof(_float));
			Desc.fDurationTime = fDrationTime;

			//월드행렬
			_float4x4 WorldMatrix = {};
			ifs.read((_char*)&WorldMatrix, sizeof(_float4x4));
			Desc.WorldMatrix = XMLoadFloat4x4(&WorldMatrix);

			//쉐이더 패스
			_uint iShaderPass = 0;
			ifs.read((_char*)&iShaderPass, sizeof(_uint));
			Desc.iShaderPass = iShaderPass;

			Desc.iEffectType = iEffectType;

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

	Safe_Release(m_pColliderCom);

	for (auto& pEffect : m_EffectObjects)
		Safe_Release(pEffect);
	m_EffectObjects.clear();

	for (auto& pEffect : m_CloneEffectObject)
		Safe_Release(pEffect);
	m_CloneEffectObject.clear();

	m_EffectObjects.clear();

}
