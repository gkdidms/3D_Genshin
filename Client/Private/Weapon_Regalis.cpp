#include "Weapon_Regalis.h"

#include "StateManager.h"
#include "Effect.h"

CWeapon_Regalis::CWeapon_Regalis(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CWeapon{ pDevice, pContext }
{
}

CWeapon_Regalis::CWeapon_Regalis(const CWeapon_Regalis& rhs)
	: CWeapon{ rhs }
{
}

HRESULT CWeapon_Regalis::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWeapon_Regalis::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	//m_pTransformCom->Scaling(10.f, 10.f, 10.f);
	//m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.0f));

	return S_OK;
}

void CWeapon_Regalis::Priority_Tick(const _float& fTimeDelta)
{
}

void CWeapon_Regalis::Tick(const _float& fTimeDelta)
{
	_matrix		SocketMatrix = XMMatrixIdentity();

	if (*m_pState == PLAYER_ATTACK_1
		|| *m_pState == PLAYER_ATTACK_2
		|| *m_pState == PLAYER_ATTACK_3
		|| *m_pState == PLAYER_ATTACK_4
		|| *m_pState == PLAYER_ELEMENTAL_1
		|| *m_pState == PLAYER_ELEMENTAL_1_END
		|| *m_pState == PLAYER_ELEMENTAL_2
		|| *m_pState == PLAYER_ELEMENTAL_2_END
		|| *m_pState == PLAYER_ELEMENTAL_3
		|| *m_pState == PLAYER_ELEMENTAL_3_END)
	{
		SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);
	}
	else
	{
		SocketMatrix = XMLoadFloat4x4(m_pBackMatrix);
	}

	SocketMatrix.r[0] = XMVector3Normalize(SocketMatrix.r[0]);
	SocketMatrix.r[1] = XMVector3Normalize(SocketMatrix.r[1]);
	SocketMatrix.r[2] = XMVector3Normalize(SocketMatrix.r[2]);

	XMStoreFloat4x4(&m_pWorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));

	m_pColliderCom->Tick(XMLoadFloat4x4(&m_pWorldMatrix));

	Create_TrailBuffer(fTimeDelta);
}

void CWeapon_Regalis::Late_Tick(const _float& fTimeDelta)
{
	if (m_iPreState != *m_pState)
	{
		if (*m_pState == PLAYER_ELEMENTAL_1_END ||
			*m_pState == PLAYER_ELEMENTAL_2_END ||
			*m_pState == PLAYER_ELEMENTAL_3_END)
			return;
		m_iPreState = *m_pState;
		m_isCreated = false;
		m_fCurrentTime = 0.f;
	}



	//m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CWeapon_Regalis::Render()
{
	if (m_isHide)
		return S_OK;

	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pColliderCom->Render();

	return S_OK;
}

HRESULT CWeapon_Regalis::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Regalis", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	CBounding_OBB::BOUNDING_OBB_DESC Desc{};
	Desc.eType = CCollider::COLLIDER_OBB;
	Desc.vExtents = _float3(0.1f, 0.1f, 0.4f);
	Desc.vCenter = _float3(0.f, 0.f, Desc.vExtents.z * -1.f);
	Desc.vRotation = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider", L"Com_Collider", reinterpret_cast<CComponent**>(&m_pColliderCom), &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeapon_Regalis::Bind_ResourceData()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fFar", m_pGameInstance->Get_CamFar(), sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

void CWeapon_Regalis::Change_Animation(const _float& fTimeDelta)
{
}

HRESULT CWeapon_Regalis::Create_TrailBuffer(const _float& fTimeDelta)
{
	//트레일 버퍼 생성하기 
	if (m_isCreated)
		return S_OK;

	m_fCurrentTime += fTimeDelta;

	if (m_pStateManager->isNormalAttack())
	{
		
		if (*m_pState == PLAYER_ATTACK_1) // 이펙트에 쉐이더 패스 가져오는거 추가하기
		{

			m_isCreated = true;
			CEffect::EFFECT_DESC Desc{};
			Desc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&Desc.RotationMatrix, XMMatrixRotationZ(XMConvertToRadians(-90.f)) * XMMatrixRotationY(XMConvertToRadians(30.f)) * XMMatrixRotationX(XMConvertToRadians(45.f)));
			Desc.vPos = _float4(0.f, 1.f, 0.f, 1.f);
			Desc.vScale = _float3(3.f, 3.f, 3.f);
			Desc.fDuration = 2.f;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Nilou_Normal_01"), TEXT("Layer_Trail"), &Desc)))
				return E_FAIL;
		}
		else if (*m_pState == PLAYER_ATTACK_2)
		{

			m_isCreated = true;
			CEffect::EFFECT_DESC Desc{};

			Desc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&Desc.RotationMatrix, XMMatrixRotationY(XMConvertToRadians(-90.f)) * XMMatrixRotationX(XMConvertToRadians(180.f)));
			Desc.vPos = _float4(0.f, 1.f, 0.f, 1.f);
			Desc.vScale = _float3(3.f, 3.f, 3.f);
			Desc.fDuration = 2.f;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Nilou_Normal_02"), TEXT("Layer_Trail"), &Desc)))
				return E_FAIL;
		}
		else if (*m_pState == PLAYER_ATTACK_3)
		{
			if (m_fCurrentTime <= 0.2f)
				return S_OK;

			m_isCreated = true;

			CEffect::EFFECT_DESC Desc{};

			Desc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&Desc.RotationMatrix, XMMatrixRotationY(XMConvertToRadians(-90.f)) * XMMatrixRotationX(XMConvertToRadians(180.f)));
			Desc.vPos = _float4(0.f, 1.f, 1.f, 1.f);
			Desc.vScale = _float3(4.f, 4.f, 4.f);
			Desc.fDuration = 2.f;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Nilou_Normal_03"), TEXT("Layer_Trail"), &Desc)))
				return E_FAIL;

			CEffect::EFFECT_DESC ParticleDesc{};

			ParticleDesc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&ParticleDesc.RotationMatrix, XMMatrixIdentity());
			ParticleDesc.vPos = _float4(0.f, 0.f, 0.f, 1.f);
			ParticleDesc.vScale = _float3(1.f, 1.f, 1.f);
			ParticleDesc.fDuration = 0.8f;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Nilou_Blade_Particle"), TEXT("Layer_Trail"), &ParticleDesc)))
				return E_FAIL;
		}
	}
	else if (m_pStateManager->isElementalArt())
	{
		if (*m_pState == PLAYER_ELEMENTAL_1)
		{
			m_isCreated = true;

			CEffect::EFFECT_DESC Desc{};

			Desc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&Desc.RotationMatrix, XMMatrixRotationX(XMConvertToRadians(90.f)));
			Desc.vPos = _float4(0.f, -1.5f, 0.f, 1.f);
			Desc.vScale = _float3(2.f, 2.f, 2.f);
			Desc.fDuration = 0.6f;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Nilou_Elemental_Start"), TEXT("Layer_Trail"), &Desc)))
				return E_FAIL;

			CEffect::EFFECT_DESC ParticleDesc{};

			ParticleDesc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&ParticleDesc.RotationMatrix, XMMatrixIdentity());
			ParticleDesc.vPos = _float4(0.f, 0.f, 0.f, 1.f);
			ParticleDesc.vScale = _float3(1.f, 1.f, 1.f);
			ParticleDesc.fDuration = 0.6f;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_ParticleTwinkle_1"), TEXT("Layer_Trail"), &ParticleDesc)))
				return E_FAIL;

			//물방울
			CEffect::EFFECT_DESC WaterParticleDesc{};

			WaterParticleDesc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&WaterParticleDesc.RotationMatrix, XMMatrixIdentity());
			WaterParticleDesc.vPos = _float4(0.f, 0.f, 0.f, 1.f);
			WaterParticleDesc.vScale = _float3(1.f, 1.f, 1.f);
			WaterParticleDesc.fDuration = 0.6f;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_ParticleWater_1"), TEXT("Layer_Particle"), &WaterParticleDesc)))
				return E_FAIL;

			//버블
			CEffect::EFFECT_DESC BubleParticleDesc{};

			BubleParticleDesc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&BubleParticleDesc.RotationMatrix, XMMatrixIdentity());
			BubleParticleDesc.vPos = _float4(0.f, 0.f, 0.f, 1.f);
			BubleParticleDesc.vScale = _float3(1.f, 1.f, 1.f);
			BubleParticleDesc.fDuration = 0.6f;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_ParticleBuble_1"), TEXT("Layer_Particle"), &BubleParticleDesc)))
				return E_FAIL;

			
		}
		else if (*m_pState == PLAYER_ATTACK_1)
		{
			if (m_fCurrentTime <= 0.2f)
				return S_OK;

			m_isCreated = true;

			CEffect::EFFECT_DESC Desc{};

			Desc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&Desc.RotationMatrix, XMMatrixRotationX(XMConvertToRadians(120.f)));
			Desc.vPos = _float4(0.f, 1.f, 1.f, 1.f);
			Desc.vScale = _float3(2.f, 2.f, 2.f);
			Desc.fDuration = 0.6f;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Nilou_Elemental_Normal_1"), TEXT("Layer_Trail"), &Desc)))
				return E_FAIL;

			CEffect::EFFECT_DESC ParticleDesc{};

			ParticleDesc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&ParticleDesc.RotationMatrix, XMMatrixIdentity());
			ParticleDesc.vPos = _float4(0.f, 0.f, 0.f, 1.f);
			ParticleDesc.vScale = _float3(1.f, 1.f, 1.f);
			ParticleDesc.fDuration = 0.6f;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_ParticleTwinkle_1"), TEXT("Layer_Trail"), &ParticleDesc)))
				return E_FAIL;

			//물방울
			CEffect::EFFECT_DESC WaterParticleDesc{};

			WaterParticleDesc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&WaterParticleDesc.RotationMatrix, XMMatrixIdentity());
			WaterParticleDesc.vPos = _float4(0.f, 0.f, 0.f, 1.f);
			WaterParticleDesc.vScale = _float3(1.f, 1.f, 1.f);
			WaterParticleDesc.fDuration = 0.8f;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_ParticleWater_1"), TEXT("Layer_Particle"), &WaterParticleDesc)))
				return E_FAIL;

			//버블
			CEffect::EFFECT_DESC BubleParticleDesc{};

			BubleParticleDesc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&BubleParticleDesc.RotationMatrix, XMMatrixIdentity());
			BubleParticleDesc.vPos = _float4(0.f, 0.f, 0.f, 1.f);
			BubleParticleDesc.vScale = _float3(1.f, 1.f, 1.f);
			BubleParticleDesc.fDuration = 0.8f;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_ParticleBuble_1"), TEXT("Layer_Particle"), &BubleParticleDesc)))
				return E_FAIL;

		}
		else if (*m_pState == PLAYER_ATTACK_2)
		{
			if (m_fCurrentTime <= 0.1f)
				return S_OK;

			m_isCreated = true;

			CEffect::EFFECT_DESC Desc{};

			Desc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&Desc.RotationMatrix, XMMatrixRotationX(XMConvertToRadians(100.f)) * XMMatrixRotationY(XMConvertToRadians(90.f)));
			Desc.vPos = _float4(0.f, 1.f, 1.f, 1.f);
			Desc.vScale = _float3(2.f, 2.f, 2.f);
			Desc.fDuration = 0.6f;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Nilou_Elemental_Normal_2"), TEXT("Layer_Trail"), &Desc)))
				return E_FAIL;

			CEffect::EFFECT_DESC ParticleDesc{};

			ParticleDesc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&ParticleDesc.RotationMatrix, XMMatrixIdentity());
			ParticleDesc.vPos = _float4(0.f, 0.f, 0.f, 1.f);
			ParticleDesc.vScale = _float3(1.f, 1.f, 1.f);
			ParticleDesc.fDuration = 0.4f;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_ParticleTwinkle_1"), TEXT("Layer_Trail"), &ParticleDesc)))
				return E_FAIL;

			//물방울
			CEffect::EFFECT_DESC WaterParticleDesc{};

			WaterParticleDesc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&WaterParticleDesc.RotationMatrix, XMMatrixIdentity());
			WaterParticleDesc.vPos = _float4(0.f, 0.f, 0.f, 1.f);
			WaterParticleDesc.vScale = _float3(1.f, 1.f, 1.f);
			WaterParticleDesc.fDuration = 0.8f;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_ParticleWater_1"), TEXT("Layer_Particle"), &WaterParticleDesc)))
				return E_FAIL;

			//버블
			CEffect::EFFECT_DESC BubleParticleDesc{};

			BubleParticleDesc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&BubleParticleDesc.RotationMatrix, XMMatrixIdentity());
			BubleParticleDesc.vPos = _float4(0.f, 0.f, 0.f, 1.f);
			BubleParticleDesc.vScale = _float3(1.f, 1.f, 1.f);
			BubleParticleDesc.fDuration = 0.8f;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_ParticleBuble_1"), TEXT("Layer_Particle"), &BubleParticleDesc)))
				return E_FAIL;

		}
		else if (*m_pState == PALYER_ATTACK_SPEC) // 수정하기
		{
			if (m_fCurrentTime <= 0.3f)
				return S_OK;

			m_isCreated = true;

			CEffect::EFFECT_DESC Desc{};

			Desc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&Desc.RotationMatrix, XMMatrixRotationX(XMConvertToRadians(90.f)));
			Desc.vPos = _float4(0.f, 1.f, 1.f, 1.f);
			Desc.vScale = _float3(1.f, 1.f, 1.f);
			Desc.fDuration = 3.f;
			Desc.isTrailMove = true;
			Desc.fSpeed = 10.f;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Nilou_Elemental_Normal_spec"), TEXT("Layer_Trail"), &Desc)))
				return E_FAIL;

			//물방울
			CEffect::EFFECT_DESC WaterParticleDesc{};

			WaterParticleDesc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&WaterParticleDesc.RotationMatrix, XMMatrixIdentity());
			WaterParticleDesc.vPos = _float4(0.f, 0.f, 0.f, 1.f);
			WaterParticleDesc.vScale = _float3(1.f, 1.f, 1.f);
			WaterParticleDesc.fDuration = 0.8f;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_ParticleWater_1"), TEXT("Layer_Particle"), &WaterParticleDesc)))
				return E_FAIL;

			//버블
			CEffect::EFFECT_DESC BubleParticleDesc{};

			BubleParticleDesc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&BubleParticleDesc.RotationMatrix, XMMatrixIdentity());
			BubleParticleDesc.vPos = _float4(0.f, 0.f, 0.f, 1.f);
			BubleParticleDesc.vScale = _float3(1.f, 1.f, 1.f);
			BubleParticleDesc.fDuration = 0.8f;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_ParticleBuble_1"), TEXT("Layer_Particle"), &BubleParticleDesc)))
				return E_FAIL;

		}
		else if (*m_pState == PLAYER_ELEMENTAL_2)
		{
			m_isCreated = true;

			CEffect::EFFECT_DESC Desc{};

			Desc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&Desc.RotationMatrix, XMMatrixRotationX(XMConvertToRadians(90.f)) * XMMatrixRotationZ(XMConvertToRadians(15.f)));
			Desc.vPos = _float4(0.f, 1.f, 0.f, 1.f);
			Desc.vScale = _float3(2.f, 2.f, 2.f);
			Desc.fDuration = 3.f;
			Desc.isFollowPlayer = true;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Nilou_Elemental_Ring"), TEXT("Layer_Trail"), &Desc)))
				return E_FAIL;

			//물방울
			CEffect::EFFECT_DESC WaterParticleDesc{};

			WaterParticleDesc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&WaterParticleDesc.RotationMatrix, XMMatrixIdentity());
			WaterParticleDesc.vPos = _float4(0.f, 0.f, 0.f, 1.f);
			WaterParticleDesc.vScale = _float3(1.f, 1.f, 1.f);
			WaterParticleDesc.fDuration = 0.8f;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_ParticleWater_1"), TEXT("Layer_Particle"), &WaterParticleDesc)))
				return E_FAIL;

			//버블
			CEffect::EFFECT_DESC BubleParticleDesc{};

			BubleParticleDesc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&BubleParticleDesc.RotationMatrix, XMMatrixIdentity());
			BubleParticleDesc.vPos = _float4(0.f, 0.f, 0.f, 1.f);
			BubleParticleDesc.vScale = _float3(1.f, 1.f, 1.f);
			BubleParticleDesc.fDuration = 0.8f;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_ParticleBuble_1"), TEXT("Layer_Particle"), &BubleParticleDesc)))
				return E_FAIL;

		}
		else if (*m_pState == PLAYER_ELEMENTAL_3)
		{
			m_isCreated = true;

			CEffect::EFFECT_DESC Desc{};

			Desc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&Desc.RotationMatrix, XMMatrixRotationX(XMConvertToRadians(90.f)) * XMMatrixRotationZ(XMConvertToRadians(30.f)));
			Desc.vPos = _float4(0.f, 1.f, 0.f, 1.f);
			Desc.vScale = _float3(3.f, 3.f, 3.f);
			Desc.fDuration = 3.f;
			Desc.isFollowPlayer = true;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Nilou_Elemental_Ring"), TEXT("Layer_Trail"), &Desc)))
				return E_FAIL;

			//물방울
			CEffect::EFFECT_DESC WaterParticleDesc{};

			WaterParticleDesc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&WaterParticleDesc.RotationMatrix, XMMatrixIdentity());
			WaterParticleDesc.vPos = _float4(0.f, 0.f, 0.f, 1.f);
			WaterParticleDesc.vScale = _float3(1.f, 1.f, 1.f);
			WaterParticleDesc.fDuration = 0.8f;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_ParticleWater_1"), TEXT("Layer_Particle"), &WaterParticleDesc)))
				return E_FAIL;

			//버블
			CEffect::EFFECT_DESC BubleParticleDesc{};

			BubleParticleDesc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&BubleParticleDesc.RotationMatrix, XMMatrixIdentity());
			BubleParticleDesc.vPos = _float4(0.f, 0.f, 0.f, 1.f);
			BubleParticleDesc.vScale = _float3(1.f, 1.f, 1.f);
			BubleParticleDesc.fDuration = 0.8f;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_ParticleBuble_1"), TEXT("Layer_Particle"), &BubleParticleDesc)))
				return E_FAIL;

		}
		else if (*m_pState == PLAYER_ELEMENTAL_SPEC)
		{
			m_isCreated = true;

			CEffect::EFFECT_DESC Desc{};

			Desc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&Desc.RotationMatrix, XMMatrixRotationX(XMConvertToRadians(90.f)) * XMMatrixRotationZ(XMConvertToRadians(60.f)));
			Desc.vPos = _float4(0.f, 1.f, 0.f, 1.f);
			Desc.vScale = _float3(3.f, 3.f, 3.f);
			Desc.fDuration = 3.f;
			Desc.isFollowPlayer = true;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Nilou_Elemental_Ring"), TEXT("Layer_Trail"), &Desc)))
				return E_FAIL;

			//물방울
			CEffect::EFFECT_DESC WaterParticleDesc{};

			WaterParticleDesc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&WaterParticleDesc.RotationMatrix, XMMatrixIdentity());
			WaterParticleDesc.vPos = _float4(0.f, 0.f, 0.f, 1.f);
			WaterParticleDesc.vScale = _float3(1.f, 1.f, 1.f);
			WaterParticleDesc.fDuration = 0.8f;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_ParticleWater_1"), TEXT("Layer_Particle"), &WaterParticleDesc)))
				return E_FAIL;

			//버블
			CEffect::EFFECT_DESC BubleParticleDesc{};

			BubleParticleDesc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&BubleParticleDesc.RotationMatrix, XMMatrixIdentity());
			BubleParticleDesc.vPos = _float4(0.f, 0.f, 0.f, 1.f);
			BubleParticleDesc.vScale = _float3(1.f, 1.f, 1.f);
			BubleParticleDesc.fDuration = 0.8f;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_ParticleBuble_1"), TEXT("Layer_Particle"), &BubleParticleDesc)))
				return E_FAIL;

		}
		else if (*m_pState == PLAYER_ELEMENTAL_SPEC_END)
		{
			m_isCreated = true;

			CEffect::EFFECT_DESC Desc{};

			Desc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&Desc.RotationMatrix, XMMatrixRotationX(XMConvertToRadians(90.f)));
			Desc.vPos = _float4(0.f, 1.f, 0.f, 1.f);
			Desc.vScale = _float3(3.f, 3.f, 3.f);
			Desc.fDuration = 3.f;
			Desc.isFollowPlayer = true;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Nilou_Elemental_Ring"), TEXT("Layer_Trail"), &Desc)))
				return E_FAIL;

			//물방울
			CEffect::EFFECT_DESC WaterParticleDesc{};

			WaterParticleDesc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&WaterParticleDesc.RotationMatrix, XMMatrixIdentity());
			WaterParticleDesc.vPos = _float4(0.f, 0.f, 0.f, 1.f);
			WaterParticleDesc.vScale = _float3(1.f, 1.f, 1.f);
			WaterParticleDesc.fDuration = 0.8f;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_ParticleWater_1"), TEXT("Layer_Particle"), &WaterParticleDesc)))
				return E_FAIL;

			//버블
			CEffect::EFFECT_DESC BubleParticleDesc{};

			BubleParticleDesc.pPlayerMatrix = m_pParentMatrix;
			XMStoreFloat4x4(&BubleParticleDesc.RotationMatrix, XMMatrixIdentity());
			BubleParticleDesc.vPos = _float4(0.f, 0.f, 0.f, 1.f);
			BubleParticleDesc.vScale = _float3(1.f, 1.f, 1.f);
			BubleParticleDesc.fDuration = 0.8f;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_ParticleBuble_1"), TEXT("Layer_Particle"), &BubleParticleDesc)))
				return E_FAIL;
		}
	}
	

	return S_OK;
}

CWeapon_Regalis* CWeapon_Regalis::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWeapon_Regalis* pInstance = new CWeapon_Regalis(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CWeapon_Regalis::Clone(void* pArg)
{
	CWeapon_Regalis* pInstance = new CWeapon_Regalis(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CWeapon_Regalis::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
}
