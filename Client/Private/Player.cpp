#include "Player.h"

#include "GameInstance.h"
#include "PartObject_Body.h"
#include "StateManager.h"
#include "Wanderer_Body.h"

#include "Weapon.h"
#include "Weapon_Ayus.h"
#include "Weapon_Regalis.h"

#include "Hili.h"

#include "SkillObj.h"
#include "Flycloak.h"
#include "KasaRing.h"

#include "CutCamera.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject { pDevice, pContext },
	m_pStateManager { CStateManager::GetInstance() }
{
	Safe_AddRef(m_pStateManager);
}

CPlayer::CPlayer(const CPlayer& rhs)
	: CGameObject { rhs },
	m_pStateManager{ rhs.m_pStateManager }
{
	Safe_AddRef(m_pStateManager);
}

_bool CPlayer::isAttack()
{
	return m_pStateManager->isAttack();
}

_float CPlayer::Get_PlayerbleHP()
{
	return dynamic_cast<CPartObject_Body*>(m_PartObject[m_CurrentPlayerble][PART_BODY])->Get_HP();
}

_float CPlayer::Get_PlayerbleMaxHP()
{
	return dynamic_cast<CPartObject_Body*>(m_PartObject[m_CurrentPlayerble][PART_BODY])->Get_MaxHP();
}

void CPlayer::Set_PlayerMove(_vector vMoveSpeed)
{
	// 받아온 속도 만큼 이동해주기
	_matrix MoveMatrix = XMMatrixIdentity();
	MoveMatrix.r[3] = vMoveSpeed;

	m_pTransformCom->Go_Run(MoveMatrix, m_pNavigationCom);
}

void CPlayer::Set_Height(_float fHeight) // 플레이어 높이를 직접 설정
{
	m_fHeight = fHeight;
	m_isPlanHeight = true;
}

_uint CPlayer::Get_CurrentWeapon()
{
	return dynamic_cast<CWeapon*>(m_PartObject[m_CurrentPlayerble][PART_WEAPON])->Get_WeaponType();
}

CCollider* CPlayer::Get_SwordCollider()
{
	return dynamic_cast<CWeapon_Regalis*>(m_PartObject[m_CurrentPlayerble][PART_WEAPON])->Get_Collider();
}

ELEMENTAL_TYPE CPlayer::Get_CurrentPlayerbleElemental()
{
	return dynamic_cast<CPartObject_Body*>(m_PartObject[m_CurrentPlayerble][PART_BODY])->Get_PlayerbleElemental();
}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	PLAYER_DESC* pDesc = static_cast<PLAYER_DESC*>(pArg);
	
	_float3 vPlayerPos = pDesc->vPlayerPos;
	m_iPlayerNavigationIndex = pDesc->iPlayerNavigationIndex;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (FAILED(Ready_Bodys()))
		return E_FAIL;

	if (FAILED(Ready_Weapons()))
		return E_FAIL;

	if (FAILED(Ready_Flycloak()))
		return E_FAIL;

	if (FAILED(Ready_SkillObjs()))
		return E_FAIL;

	m_pCameraLook = dynamic_cast<CPartObject*>(m_PartObject[m_CurrentPlayerble][PART_BODY])->Get_CameraLook();
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(vPlayerPos.x, vPlayerPos.y, vPlayerPos.z, 1.f));
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));


	CStateManager::PLAYER_STATE_DESC PlayerStateDesc{};
	PlayerStateDesc.pPlayerbleType = &m_CurrentPlayerble;
	PlayerStateDesc.pPlayerDir = &m_iDirState;

	m_pStateManager->Initialize(&PlayerStateDesc);
	m_pStateManager->Set_CurrentState(CStateManager::STATE_TYPE_IDEL);

	return S_OK;
}

void CPlayer::Priority_Tick(const _float& fTimeDelta)
{
	m_isPlanHeight = false;

	for (auto& pObject : m_PartObject[m_CurrentPlayerble])
		pObject->Priority_Tick(fTimeDelta);
}

void CPlayer::Tick(const _float& fTimeDelta)
{
	Change_Playerble();
	Check_State(fTimeDelta);
	Input_Key(fTimeDelta);
	
	for (auto& pObject : m_PartObject[m_CurrentPlayerble])
		pObject->Tick(fTimeDelta);

	_vector vPos;
	_float4x4 RootMatrix;
	
	_bool isMove = true;
	
	if (!m_isColl)
	{
		dynamic_cast<CPartObject*>(m_PartObject[m_CurrentPlayerble][PART_BODY])->Set_PlayerPos(&RootMatrix);
		XMStoreFloat4x4(&RootMatrix, XMLoadFloat4x4(&RootMatrix) * -1.f);
		isMove = m_pTransformCom->Go_Run(XMLoadFloat4x4(&RootMatrix), m_pNavigationCom, m_pStateManager->isFly());
	}
	SetUp_CellType(isMove);
	SetUp_OnTerrain(fTimeDelta);
	Ready_PlayerStateRank();

	m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void CPlayer::Late_Tick(const _float& fTimeDelta)
{
	for (auto& pObject : m_PartObject[m_CurrentPlayerble])
		pObject->Late_Tick(fTimeDelta);

	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);

	m_isColl = false; // 초기화
}

HRESULT CPlayer::Render()
{
	for (auto& pObject : m_PartObject[m_CurrentPlayerble])
		pObject->Render();

#ifdef _DEBUG
	m_pColliderCom->Render();
#endif // _DEBUG

	return S_OK;
}

void CPlayer::Change_Playerble() // 코드 수정하기
{
	//캐릭터 변경
	if (m_pGameInstance->GetKeyState(DIK_1) == CInput_Device::TAP)
		m_CurrentPlayerble = PLAYER_TIGHNARI;
	if (m_pGameInstance->GetKeyState(DIK_2) == CInput_Device::TAP)
		m_CurrentPlayerble = PLAYER_NILOU;
	if (m_pGameInstance->GetKeyState(DIK_3) == CInput_Device::TAP)
		m_CurrentPlayerble = PLAYER_WANDERER;
	if (m_pGameInstance->GetKeyState(DIK_4) == CInput_Device::TAP)
		m_CurrentPlayerble = PLAYER_FEIYAN;

	if (m_pGameInstance->GetKeyState(DIK_1) == CInput_Device::TAP
		|| m_pGameInstance->GetKeyState(DIK_2) == CInput_Device::TAP
		|| m_pGameInstance->GetKeyState(DIK_3) == CInput_Device::TAP
		|| m_pGameInstance->GetKeyState(DIK_4) == CInput_Device::TAP)
	{
		m_pStateManager->Set_CurrentState(CStateManager::STATE_TYPE_IDEL);
		m_pCameraLook = dynamic_cast<CPartObject*>(m_PartObject[m_CurrentPlayerble][PART_BODY])->Get_CameraLook();
	}
}

HRESULT CPlayer::Add_Components()
{
	CNavigation::NAVIGATION_DESC NavigationDesc = {};
	NavigationDesc.iIndex = m_iPlayerNavigationIndex;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Navigation", L"Com_Navigation", reinterpret_cast<CComponent**>(&m_pNavigationCom), &NavigationDesc)))
		return E_FAIL;

	CBounding_AABB::BOUNDING_AABB_DESC ColliderDesc = {};
	ColliderDesc.eType = CCollider::COLLIDER_AABB;
	ColliderDesc.vExtents = _float3{ 0.5f, 0.8f, 0.5f };
	ColliderDesc.vCenter = _float3{ 0.f, ColliderDesc.vExtents.y, 0.f };
	
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider", L"Com_Collider", reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
		return E_FAIL;

	return S_OK;
}

void CPlayer::SetUp_OnTerrain(const _float& fTimeDelta)
{
	if (m_pStateManager->isElementalArt() || (m_CurrentPlayerble == PLAYER_WANDERER && m_iState == PLAYER_ELEMENTAL_END))
	{
		if (m_iState != PLAYER_FALL_ATTACK_LOOP && !m_pStateManager->isFly())
			return;
	}
		
	// 높이 조정
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	if (!m_isPlanHeight)
		m_fHeight = m_pNavigationCom->Compute_Height(vPos);

	if (m_pStateManager->isJump())
	{		
		//점프하고 있을때 
		m_fJumpCurrentTime += fTimeDelta;

		if (m_fJumpDurationTime > m_fJumpCurrentTime)
			return;

		if (XMVectorGetY(vPos) <= m_fHeight)
		{
			m_fJumpCurrentTime = { 0.f };
			m_iState = m_pStateManager->Set_CurrentState(CStateManager::STATE_TYPE_FALL_GROUND);
		}

		return;
	}
	if (m_pStateManager->isFly())
	{
		if (XMVectorGetY(vPos) <= m_fHeight)
		{
			m_iState = m_pStateManager->Set_CurrentState(CStateManager::STATE_TYPE_FALL_GROUND);
		}
			
		else return;
	}
	if (m_pStateManager->isDrop())
	{
		if (XMVectorGetY(vPos) <= m_fHeight)
		{
			m_iState = m_pStateManager->Set_CurrentState(CStateManager::STATE_TYPE_FALL_GROUND);
		}
		else return;
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPos, m_fHeight));
}

void CPlayer::SetUp_CellType(_bool isMove)
{
	CCell::OPTION eOption = m_pNavigationCom->Get_OptionType();

	if (eOption == CCell::OPTION_FLY && isMove) //FLY
	{
		if (m_iState != PLAYER_FALL_ATTACK_LOOP && !m_pStateManager->isFly() && !m_pStateManager->isJump())
		{
			m_iState = m_pStateManager->Set_CurrentState(CStateManager::STATE_TYPE_FALL_ATTACK);
		}
	}

	if (eOption == CCell::OPTION_STAIRS) // 계단 타입
	{
		//true 이면 아래
		m_eHill = m_pNavigationCom->isLook(m_pTransformCom->Get_State(CTransform::STATE_LOOK)) ? HILL_DOWN : HILL_UP;
	}
	else
		m_eHill = HILL_END;
}

void CPlayer::Check_State(const _float& fTimeDelta)
{
	// 상태 패턴 업데이트
	m_iState = m_pStateManager->Update(fTimeDelta, PLAYER_STATE(m_iState));

	// 원소폭팔 사용 시 카메라 제어 
	// 원소 스킬 사용 시 플레이어 원래 위치로 회전시키기 
	if (m_iState == PLAYER_ELEMENTAL_BURST)
	{
		m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));
		dynamic_cast<CCutCamera*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), 1))->isCutScene(true, m_CurrentPlayerble);
	}
	else if (m_iState == PLAYER_ELEMENTAL_BURST_END)
	{
		dynamic_cast<CCutCamera*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), 1))->isCutScene(false);
	}
}

HRESULT CPlayer::Ready_Bodys()
{
	//PartObject::Body
	CPartObject_Body::BODY_DESC Desc{};

	Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	Desc.pState = &m_iState;
	Desc.pDirState = &m_iDirState;
	Desc.pHill = &m_eHill;
	Desc.fSpeedPecSec = 20.f;
	Desc.fRotatePecSec = XMConvertToRadians(45.f);

	// Tighnari
	CGameObject* pGameObject = m_pGameInstance->Clone_Object(L"Prototype_GameObject_Player_Tighnari", &Desc);
	if (nullptr == pGameObject)
		return E_FAIL;
	m_PartObject[PLAYER_TIGHNARI].emplace_back(pGameObject);

	//Nilou
	pGameObject = m_pGameInstance->Clone_Object(L"Prototype_GameObject_Player_Nilou", &Desc);
	if (nullptr == pGameObject)
		return E_FAIL;
	m_PartObject[PLAYER_NILOU].emplace_back(pGameObject);

	//Wanderer
	Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	Desc.pState = &m_iState;
	Desc.pDirState = &m_iDirState;
	Desc.pHill = &m_eHill;
	Desc.fSpeedPecSec = 20.f;
	Desc.fRotatePecSec = XMConvertToRadians(45.f);

	pGameObject = m_pGameInstance->Clone_Object(L"Prototype_GameObject_Player_Wanderer", &Desc);
	if (nullptr == pGameObject)
		return E_FAIL;
	m_PartObject[PLAYER_WANDERER].emplace_back(pGameObject);

	//Yae
	//pGameObject = m_pGameInstance->Clone_Object(L"Prototype_GameObject_Player_Yae", &Desc);
	//if (nullptr == pGameObject)
	//	return E_FAIL;
	//m_PartObject[PLAYER_YAE].emplace_back(pGameObject);

	//Feiyan Prototype_GameObject_Player_Feiyan
	pGameObject = m_pGameInstance->Clone_Object(L"Prototype_GameObject_Player_Feiyan", &Desc);
	if (nullptr == pGameObject)
		return E_FAIL;
	m_PartObject[PLAYER_FEIYAN].emplace_back(pGameObject);

	return S_OK;
}

HRESULT CPlayer::Ready_Weapons()
{
	// PartObject::Weapon
	CWeapon_Ayus::WEAPON_DESC WeaponDesc{};
	CComponent* pComponent = m_PartObject[PLAYER_TIGHNARI][PART_BODY]->Get_Component(L"Com_Model");
	if (nullptr == pComponent)
		return E_FAIL;

	WeaponDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	WeaponDesc.pState = &m_iState;
	WeaponDesc.fSpeedPecSec = 20.f;
	WeaponDesc.fRotatePecSec = XMConvertToRadians(45.f);

	//Tighnari
	WeaponDesc.pHandCombinedTransformationMatrix = dynamic_cast<CModel*>(pComponent)->Get_BoneCombinedTransformationMatrix("PRIVATE_WeaponArrow");
	if (nullptr == WeaponDesc.pHandCombinedTransformationMatrix)
		return E_FAIL;

	WeaponDesc.pBackCombinedTransformationMatrix = dynamic_cast<CModel*>(pComponent)->Get_BoneCombinedTransformationMatrix("PRIVATE_WeaponRootBow");
	if (nullptr == WeaponDesc.pBackCombinedTransformationMatrix)
		return E_FAIL;

	WeaponDesc.eWeaponType = CWeapon::WEAPON_BOW;
	CGameObject* pGameObject = dynamic_cast<CPartObject*>(m_pGameInstance->Clone_Object(L"Prototype_GameObject_Weapon_Ayus", &WeaponDesc));
	if (nullptr == pGameObject)
		return E_FAIL;
	m_PartObject[PLAYER_TIGHNARI].emplace_back(pGameObject);

	// Yae // Feiyan
	pComponent = m_PartObject[PLAYER_FEIYAN][PART_BODY]->Get_Component(L"Com_Model");
	if (nullptr == pComponent)
		return E_FAIL;

	WeaponDesc.pHandCombinedTransformationMatrix = dynamic_cast<CModel*>(pComponent)->Get_BoneCombinedTransformationMatrix("PRIVATE_WeaponRootCatalyst");
	if (nullptr == WeaponDesc.pHandCombinedTransformationMatrix)
		return E_FAIL;

	WeaponDesc.eWeaponType = CWeapon::WEAPON_CATALYSE;
	pGameObject = dynamic_cast<CPartObject*>(m_pGameInstance->Clone_Object(L"Prototype_GameObject_Weapon_Narukami", &WeaponDesc));
	if (nullptr == pGameObject)
		return E_FAIL;
	m_PartObject[PLAYER_FEIYAN].emplace_back(pGameObject);

	// Wanderer
	pComponent = m_PartObject[PLAYER_WANDERER][PART_BODY]->Get_Component(L"Com_Model");
	if (nullptr == pComponent)
		return E_FAIL;

	WeaponDesc.pHandCombinedTransformationMatrix = dynamic_cast<CModel*>(pComponent)->Get_BoneCombinedTransformationMatrix("PRIVATE_WeaponRootSword");
	if (nullptr == WeaponDesc.pHandCombinedTransformationMatrix)
		return E_FAIL;
	WeaponDesc.eWeaponType = CWeapon::WEAPON_CATALYSE;
	pGameObject = dynamic_cast<CPartObject*>(m_pGameInstance->Clone_Object(L"Prototype_GameObject_Weapon_Alaya", &WeaponDesc));
	if (nullptr == pGameObject)
		return E_FAIL;
	m_PartObject[PLAYER_WANDERER].emplace_back(pGameObject);

	//Nilou
	pComponent = m_PartObject[PLAYER_NILOU][PART_BODY]->Get_Component(L"Com_Model");
	if (nullptr == pComponent)
		return E_FAIL;

	WeaponDesc.pHandCombinedTransformationMatrix = dynamic_cast<CModel*>(pComponent)->Get_BoneCombinedTransformationMatrix("WeaponR");
	if (nullptr == WeaponDesc.pHandCombinedTransformationMatrix)
		return E_FAIL;

	WeaponDesc.pBackCombinedTransformationMatrix = dynamic_cast<CModel*>(pComponent)->Get_BoneCombinedTransformationMatrix("PRIVATE_WeaponRootSword");
	if (nullptr == WeaponDesc.pBackCombinedTransformationMatrix)
		return E_FAIL;

	WeaponDesc.eWeaponType = CWeapon::WEAPON_SWORD;
	pGameObject = dynamic_cast<CPartObject*>(m_pGameInstance->Clone_Object(L"Prototype_GameObject_Weapon_Regalis", &WeaponDesc));
	if (nullptr == pGameObject)
		return E_FAIL;
	m_PartObject[PLAYER_NILOU].emplace_back(pGameObject);

	return S_OK;
}

HRESULT CPlayer::Ready_SkillObjs()
{
	// Yae
	/*CSkillObj::SKILLOBJ_DESC SKillObjDesc{};
	CComponent* pComponent = m_PartObject[PLAYER_YAE][PART_BODY]->Get_Component(L"Com_Model");
	if (nullptr == pComponent)
		return E_FAIL;

	SKillObjDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	SKillObjDesc.pState = &m_iState;
	SKillObjDesc.fSpeedPecSec = 20.f;
	SKillObjDesc.fRotatePecSec = XMConvertToRadians(45.f);

	SKillObjDesc.pHandCombinedTransformationMatrix = dynamic_cast<CModel*>(pComponent)->Get_BoneCombinedTransformationMatrix("WeaponR");
	if (nullptr == SKillObjDesc.pHandCombinedTransformationMatrix)
		return E_FAIL;

	CGameObject* pGameObject = m_pGameInstance->Clone_Object(L"Prototype_GameObject_SkillObj_Gohei", &SKillObjDesc);
	if (nullptr == pGameObject)
		return E_FAIL;

	m_PartObject[PLAYER_YAE].emplace_back(pGameObject);

	pGameObject = dynamic_cast<CPartObject*>(m_pGameInstance->Clone_Object(L"Prototype_GameObject_SkillObj_FoxTail_Burst", &SKillObjDesc));
	if (nullptr == pGameObject)
		return E_FAIL;

	m_PartObject[PLAYER_YAE].emplace_back(pGameObject);*/

	CSkillObj::SKILLOBJ_DESC SKillObjDesc{};

	SKillObjDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	SKillObjDesc.pState = &m_iState;
	SKillObjDesc.fSpeedPecSec = 20.f;
	SKillObjDesc.fRotatePecSec = XMConvertToRadians(45.f);

	//Wanderer
	CComponent*  pComponent = m_PartObject[PLAYER_WANDERER][PART_BODY]->Get_Component(L"Com_Model");
	if (nullptr == pComponent)
		return E_FAIL;

	SKillObjDesc.pHandCombinedTransformationMatrix = dynamic_cast<CModel*>(pComponent)->Get_BoneCombinedTransformationMatrix("_HatRootB_CB_A01");
	if (nullptr == SKillObjDesc.pHandCombinedTransformationMatrix)
		return E_FAIL;

	CGameObject* pGameObject = m_pGameInstance->Clone_Object(L"Prototype_GameObject_SkillObj_KasaRing", &SKillObjDesc);
	if (nullptr == pGameObject)
		return E_FAIL;

	m_PartObject[PLAYER_WANDERER].emplace_back(pGameObject);


	return S_OK;
}

HRESULT CPlayer::Ready_Flycloak()
{
	CFlycloak::FLYCLOAK_DESC FlycloakDesc{};
	CModel* pComponent = dynamic_cast<CModel*>(m_PartObject[PLAYER_TIGHNARI][PART_BODY]->Get_Component(L"Com_Model"));

	FlycloakDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	FlycloakDesc.pState = &m_iState;
	FlycloakDesc.pSocketCombinedTransformationMatrix = pComponent->Get_BoneCombinedTransformationMatrix("_FlycloakRootB_CB_A01");
	if (nullptr == FlycloakDesc.pSocketCombinedTransformationMatrix)
		return E_FAIL;

	CGameObject* pFlycloak = m_pGameInstance->Clone_Object(L"Prototype_GameObject_Flycloak", &FlycloakDesc);
	if (nullptr == pFlycloak)
		return E_FAIL;

	m_PartObject[PLAYER_TIGHNARI].emplace_back(pFlycloak);

	/* Nilou */
	pComponent = dynamic_cast<CModel*>(m_PartObject[PLAYER_NILOU][PART_BODY]->Get_Component(L"Com_Model"));

	FlycloakDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	FlycloakDesc.pState = &m_iState;
	FlycloakDesc.pSocketCombinedTransformationMatrix = pComponent->Get_BoneCombinedTransformationMatrix("_FlycloakRootB_CB_A01");
	if (nullptr == FlycloakDesc.pSocketCombinedTransformationMatrix)
		return E_FAIL;

	pFlycloak = m_pGameInstance->Clone_Object(L"Prototype_GameObject_Flycloak", &FlycloakDesc);
	if (nullptr == pFlycloak)
		return E_FAIL;

	m_PartObject[PLAYER_NILOU].emplace_back(pFlycloak);

	/* Wanderer */
	pComponent = dynamic_cast<CModel*>(m_PartObject[PLAYER_WANDERER][PART_BODY]->Get_Component(L"Com_Model"));

	FlycloakDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	FlycloakDesc.pState = &m_iState;
	FlycloakDesc.pSocketCombinedTransformationMatrix = pComponent->Get_BoneCombinedTransformationMatrix("_FlycloakRootB_CB_A01");
	if (nullptr == FlycloakDesc.pSocketCombinedTransformationMatrix)
		return E_FAIL;

	pFlycloak = m_pGameInstance->Clone_Object(L"Prototype_GameObject_Flycloak", &FlycloakDesc);
	if (nullptr == pFlycloak)
		return E_FAIL;

	m_PartObject[PLAYER_WANDERER].emplace_back(pFlycloak);

	/* PLAYER_FEIYAN */
	pComponent = dynamic_cast<CModel*>(m_PartObject[PLAYER_FEIYAN][PART_BODY]->Get_Component(L"Com_Model"));

	FlycloakDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	FlycloakDesc.pState = &m_iState;
	FlycloakDesc.pSocketCombinedTransformationMatrix = pComponent->Get_BoneCombinedTransformationMatrix("_FlycloakRootB_CB_A01");
	if (nullptr == FlycloakDesc.pSocketCombinedTransformationMatrix)
		return E_FAIL;

	pFlycloak = m_pGameInstance->Clone_Object(L"Prototype_GameObject_Flycloak", &FlycloakDesc);
	if (nullptr == pFlycloak)
		return E_FAIL;

	m_PartObject[PLAYER_FEIYAN].emplace_back(pFlycloak);

	return S_OK;
}

void CPlayer::Ready_PlayerStateRank() // 플레이어 상태에 따른 등급 정의
{
	switch (m_iState)
	{
	case Client::PLAYER_IDLE:
		m_strStateRank = 'D';
		break;
	case Client::PLAYER_ATTACK_IDLE:
		m_strStateRank = 'D';
		break;
	case Client::PLAYER_IDLE_PUT_AWAY:
		m_strStateRank = 'D';
		break;
	case Client::PLAYER_RUN_START:
		m_strStateRank = 'B';
		break;
	case Client::PLAYER_RUN:
		m_strStateRank = 'B';
		break;
	case Client::PLAYER_RUN_STOP:
		m_strStateRank = 'B';
		break;
	case Client::PLAYER_SPRINT_START:
		m_strStateRank = 'A';
		break;
	case Client::PLAYER_SPRINT:
		m_strStateRank = 'A';
		break;
	case Client::PLAYER_SPRINT_TO_RUN:
		m_strStateRank = 'A';
		break;
	case Client::PLAYER_SPRINT_STOP:
		m_strStateRank = 'A';
		break;
	case Client::PLAYER_ATTACK_1:
		m_strStateRank = 'C';
		break;
	case Client::PLAYER_ATTACK_2:
		m_strStateRank = 'C';
		break;
	case Client::PLAYER_ATTACK_3:
		m_strStateRank = 'C';
		break;
	case Client::PLAYER_ATTACK_4:
		m_strStateRank = 'C';
		break;
	case Client::PALYER_ATTACK_SPEC:
		m_strStateRank = 'C';
		break;
	case Client::PLAYER_ATTACK_END:
		m_strStateRank = 'C';
		break;
	case Client::PLAYER_ELEMENTAL_START:
		m_strStateRank = 'C';
		break;
	case Client::PLAYER_ELEMENTAL_1:
		m_strStateRank = 'C';
		break;
	case Client::PLAYER_ELEMENTAL_2:
		m_strStateRank = 'C';
		break;
	case Client::PLAYER_ELEMENTAL_3:
		m_strStateRank = 'C';
		break;
	case Client::PLAYER_ELEMENTAL_SPEC:
		m_strStateRank = 'C';
		break;
	case Client::PLAYER_ELEMENTAL_1_END:
		m_strStateRank = 'C';
		break;
	case Client::PLAYER_ELEMENTAL_2_END:
		m_strStateRank = 'C';
		break;
	case Client::PLAYER_ELEMENTAL_3_END:
		m_strStateRank = 'C';
		break;
	case Client::PLAYER_ELEMENTAL_SPEC_END:
		m_strStateRank = 'C';
		break;
	case Client::PLAYER_ELEMENTAL_UP_START:
		m_strStateRank = 'C';
		break;
	case Client::PLAYER_ELEMENTAL_UP:
		m_strStateRank = 'C';
		break;
	case Client::PLAYER_ELEMENTAL_END:
		m_strStateRank = 'C';
		break;
	case Client::PLAYER_ELEMENTAL_BURST:
		m_strStateRank = 'C';
		break;
	case Client::PLAYER_ELEMENTAL_BURST_END:
		m_strStateRank = 'C';
		break;
	case Client::PLAYER_JUMP:
		m_strStateRank = 'B';
		break;
	case Client::PLAYER_JUMP_FOR_RUN:
		m_strStateRank = 'B';
		break;
	case Client::PLAYER_JUMP_FOR_SPRINT:
		m_strStateRank = 'B';
		break;
	case Client::PLAYER_FALL_GROUND_H:
		m_strStateRank = 'D';
		break;
	case Client::PLAYER_FALL_GROUND_L:
		m_strStateRank = 'D';
		break;
	case Client::PLAYER_FALL_GROUND_FOR_RUN:
		m_strStateRank = 'D';
		break;
	case Client::PLAYER_FALL_GROUND_FOR_SPRINT:
		m_strStateRank = 'A';
		break;
	case Client::PLAYER_FALL_ATTACK_START:
		m_strStateRank = 'C';
		break;
	case Client::PLAYER_FALL_ATTACK_LOOP:
		m_strStateRank = 'C';
		break;
	case Client::PLAYER_FALL_ATTACK_END:
		m_strStateRank = 'C';
		break;
	case Client::PLAYER_FLY_START:
		m_strStateRank = 'B';
		break;
	case Client::PLAYER_FLY_NORMAL:
		m_strStateRank = 'B';
		break;
	case Client::PLAYER_FLY_TURN_L:
		m_strStateRank = 'B';
		break;
	case Client::PLAYER_FLY_TURN_R:
		m_strStateRank = 'B';
		break;
	case Client::PLAYER_HILL_UP:
		m_strStateRank = 'B';
		break;
	case Client::PLAYER_HILL_DOWN:
		m_strStateRank = 'B';
		break;
	case Client::PLAYER_END:
		break;
	default:
		break;
	}
}

void CPlayer::Input_Key(const _float& fTimeDelta)
{
	m_iDirState == DIR_END;
	// 루트 애니메이션 제어 전 까지 주석
	// 뛰기 제어
	if (m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::HOLD) // 앞
	{
		m_pTransformCom->LookForCamera(m_pGameInstance->Get_CamLook(), XMConvertToRadians(0.f));
		m_iDirState = DIR_STRIGHT;
	}
	if (m_pGameInstance->GetKeyState(DIK_S) == CInput_Device::HOLD) // 뒤
	{
		if (!m_pStateManager->isElementalArt()) m_pTransformCom->LookForCamera(m_pGameInstance->Get_CamLook(), XMConvertToRadians(180.f));
		m_iDirState = DIR_BACKWORK;
	}
	if (m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::TAP) // 왼쪽
	{
		m_pTransformCom->LookForCamera(m_pGameInstance->Get_CamLook(), XMConvertToRadians(-90.f));
		m_iDirState = DIR_STRIGHT;
	}
	if (m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::TAP) // 오른쪽 
	{
		m_pTransformCom->LookForCamera(m_pGameInstance->Get_CamLook(), XMConvertToRadians(90.f));
		m_iDirState = DIR_STRIGHT;
	}
	if (m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::HOLD && m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::HOLD) // 왼쪽 사이드
	{
		m_pTransformCom->LookForCamera(m_pGameInstance->Get_CamLook(), XMConvertToRadians(-45.f));
		m_iDirState = DIR_LEFT_SIDE;
	}
	if (m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::HOLD && m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::HOLD) // 오른쪽 사이드
	{
		m_pTransformCom->LookForCamera(m_pGameInstance->Get_CamLook(), XMConvertToRadians(45.f));
		m_iDirState = DIR_RIGHT_SIDE;
	}
}

CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer* pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pStateManager);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pColliderCom);

	for (auto& PartObjects : m_PartObject)
	{
		for (auto& pPartObject : PartObjects)
			Safe_Release(pPartObject);
		PartObjects.clear();
	}
}
