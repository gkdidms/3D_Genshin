#include "Weapon_Regalis.h"

#include "MainApp.h"
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
	__super::Tick(fTimeDelta);

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
		m_isHide = false;
		SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);
	}
	else
	{
		if (m_isHide == false)
			m_fCurrentTime = 0.f;
		m_isHide = true;
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
	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pColliderCom->Render();

	return S_OK;
}

HRESULT CWeapon_Regalis::Add_Components()
{
	if (FAILED(__super::Add_Component(CMainApp::g_iCurrentLevel, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(CMainApp::g_iCurrentLevel, L"Prototype_Component_Model_Regalis", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	CBounding_OBB::BOUNDING_OBB_DESC Desc{};
	Desc.eType = CCollider::COLLIDER_OBB;
	Desc.vExtents = _float3(0.1f, 0.1f, 0.4f);
	Desc.vCenter = _float3(0.f, 0.f, Desc.vExtents.z * -1.f);
	Desc.vRotation = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(CMainApp::g_iCurrentLevel, L"Prototype_Component_Collider", L"Com_Collider", reinterpret_cast<CComponent**>(&m_pColliderCom), &Desc)))
		return E_FAIL;

	if (FAILED(__super::Add_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeapon_Regalis::Bind_ResourceData()
{
	if (FAILED(__super::Bind_ResourceData()))
		return E_FAIL;
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
			Desc.fDuration = 0.5f;

			if (FAILED(m_pGameInstance->Add_GameObject(CMainApp::g_iCurrentLevel, TEXT("Prototype_GameObject_Effect_Nilou_Normal_00"), TEXT("Layer_Trail"), &Desc)))
				return E_FAIL;
		}
		else if (*m_pState == PLAYER_ATTACK_2)
		{

			m_isCreated = true;
			CEffect::EFFECT_DESC Desc{};

			Desc.pPlayerMatrix = m_pParentMatrix;
			Desc.fDuration = 0.5f;

			if (FAILED(m_pGameInstance->Add_GameObject(CMainApp::g_iCurrentLevel, TEXT("Prototype_GameObject_Effect_Nilou_Normal_01"), TEXT("Layer_Trail"), &Desc)))
				return E_FAIL;
		}
		else if (*m_pState == PLAYER_ATTACK_3)
		{
			if (m_fCurrentTime <= 0.2f)
				return S_OK;

			m_isCreated = true;

			CEffect::EFFECT_DESC Desc{};

			Desc.pPlayerMatrix = m_pParentMatrix;
			Desc.fDuration = 0.5f;

			if (FAILED(m_pGameInstance->Add_GameObject(CMainApp::g_iCurrentLevel, TEXT("Prototype_GameObject_Effect_Nilou_Normal_02"), TEXT("Layer_Trail"), &Desc)))
				return E_FAIL;

			Desc.pPlayerMatrix = m_pParentMatrix;
			Desc.fDuration = 0.5f;

			if (FAILED(m_pGameInstance->Add_GameObject(CMainApp::g_iCurrentLevel, TEXT("Prototype_GameObject_Effect_Nilou_Elemental_Art_Normal_02_Particle"), TEXT("Layer_Particle"), &Desc)))
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
			Desc.fDuration = 0.6f;

			if (FAILED(m_pGameInstance->Add_GameObject(CMainApp::g_iCurrentLevel, TEXT("Prototype_GameObject_Effect_Nilou_Elemental_Art_Water_00"), TEXT("Layer_Trail"), &Desc)))
				return E_FAIL;

			
		}
		else if (*m_pState == PLAYER_ATTACK_1)
		{
			if (m_fCurrentTime <= 0.2f)
				return S_OK;

			m_isCreated = true;

			CEffect::EFFECT_DESC Desc{};

			Desc.pPlayerMatrix = m_pParentMatrix;
			Desc.fDuration = 0.6f;

			if (FAILED(m_pGameInstance->Add_GameObject(CMainApp::g_iCurrentLevel, TEXT("Prototype_GameObject_Effect_Nilou_Elemental_Art_Normal_00"), TEXT("Layer_Trail"), &Desc)))
				return E_FAIL;
		}
		else if (*m_pState == PLAYER_ATTACK_2)
		{
			if (m_fCurrentTime <= 0.1f)
				return S_OK;

			m_isCreated = true;

			CEffect::EFFECT_DESC Desc{};

			Desc.pPlayerMatrix = m_pParentMatrix;
			Desc.fDuration = 0.6f;

			if (FAILED(m_pGameInstance->Add_GameObject(CMainApp::g_iCurrentLevel, TEXT("Prototype_GameObject_Effect_Nilou_Elemental_Art_Normal_01"), TEXT("Layer_Trail"), &Desc)))
				return E_FAIL;
		}
		else if (*m_pState == PALYER_ATTACK_SPEC) // 수정하기
		{
			if (m_fCurrentTime <= 0.3f)
				return S_OK;

			m_isCreated = true;

			CEffect::EFFECT_DESC Desc{};

			Desc.pPlayerMatrix = m_pParentMatrix;
			Desc.isBullet = true;
			Desc.fDuration = 3.f;
			Desc.fSpeed = 10.f;
			Desc.vTargetDir = XMLoadFloat4x4(m_pParentMatrix).r[2];

			if (FAILED(m_pGameInstance->Add_GameObject(CMainApp::g_iCurrentLevel, TEXT("Prototype_GameObject_Effect_Nilou_Elemental_Art_Normal_02"), TEXT("Layer_Trail"), &Desc)))
				return E_FAIL;

			Desc.pPlayerMatrix = m_pParentMatrix;
			Desc.isBullet = false;
			Desc.fDuration = 0.4f;

			if (FAILED(m_pGameInstance->Add_GameObject(CMainApp::g_iCurrentLevel, TEXT("Prototype_GameObject_Effect_Nilou_Elemental_Art_Normal_02_Particle"), TEXT("Layer_Particle"), &Desc)))
				return E_FAIL;
		}
		else if (*m_pState == PLAYER_ELEMENTAL_2)
		{
			m_isCreated = true;

			CEffect::EFFECT_DESC Desc{};

			Desc.pPlayerMatrix = m_pParentMatrix;
			Desc.fDuration = 0.4f;
			Desc.isFollowPlayer = true;

			if (FAILED(m_pGameInstance->Add_GameObject(CMainApp::g_iCurrentLevel, TEXT("Prototype_GameObject_Effect_Nilou_Elemental_Art_Water_01"), TEXT("Layer_Trail"), &Desc)))
				return E_FAIL;
		}
		else if (*m_pState == PLAYER_ELEMENTAL_3)
		{
			m_isCreated = true;

			CEffect::EFFECT_DESC Desc{};

			Desc.pPlayerMatrix = m_pParentMatrix;
			Desc.fDuration = 0.4f;
			Desc.isFollowPlayer = true;

			if (FAILED(m_pGameInstance->Add_GameObject(CMainApp::g_iCurrentLevel, TEXT("Prototype_GameObject_Effect_Nilou_Elemental_Art_Water_02"), TEXT("Layer_Trail"), &Desc)))
				return E_FAIL;
		}
		else if (*m_pState == PLAYER_ELEMENTAL_SPEC)
		{
			m_isCreated = true;

			CEffect::EFFECT_DESC Desc{};

			Desc.pPlayerMatrix = m_pParentMatrix;
			Desc.fDuration = 0.4f;
			Desc.isFollowPlayer = true;

			if (FAILED(m_pGameInstance->Add_GameObject(CMainApp::g_iCurrentLevel, TEXT("Prototype_GameObject_Effect_Nilou_Elemental_Art_Water_03"), TEXT("Layer_Trail"), &Desc)))
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
