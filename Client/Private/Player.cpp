#include "Player.h"

#include "GameInstance.h"
#include "PartObject_Body.h"
#include "State_Manager.h"
#include "Wanderer_Body.h"

#include "Weapon.h"
#include "Weapon_Ayus.h"
#include "Weapon_Regalis.h"

#include "SkillObj.h"

#include "CutCamera.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject { pDevice, pContext },
	m_pState_Manager { CState_Manager::GetInstance() }
{
	Safe_AddRef(m_pState_Manager);
}

CPlayer::CPlayer(const CPlayer& rhs)
	: CGameObject { rhs },
	m_pState_Manager{ rhs.m_pState_Manager }
{
	Safe_AddRef(m_pState_Manager);
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
	
	m_vPlayerPos = pDesc->vPlayerPos;
	m_iPlayerNavigationIndex = pDesc->iPlayerNavigationIndex;

	m_pState_Manager->Set_CurrentState(CState_Manager::STATE_TYPE_IDEL);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (FAILED(Ready_Bodys()))
		return E_FAIL;

	if (FAILED(Ready_Weapons()))
		return E_FAIL;

	//if (FAILED(Ready_SkillObjs()))
	//	return E_FAIL;

	m_pCameraLook = dynamic_cast<CPartObject*>(m_PartObject[m_CurrentPlayerble][PART_BODY])->Get_CameraLook();
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_vPlayerPos.x, m_vPlayerPos.y, m_vPlayerPos.z, 1.f));
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));

	return S_OK;
}

void CPlayer::Priority_Tick(const _float& fTimeDelta)
{
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

	dynamic_cast<CPartObject*>(m_PartObject[m_CurrentPlayerble][PART_BODY])->Set_PlayerPos(&RootMatrix);
	XMStoreFloat4x4(&RootMatrix, XMLoadFloat4x4(&RootMatrix) * -1.f);
	_bool isMove = m_pTransformCom->Go_Run(XMLoadFloat4x4(&RootMatrix), m_pNavigationCom);

	SetUp_CellType(isMove);
	SetUp_OnTerrain();

	m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void CPlayer::Late_Tick(const _float& fTimeDelta)
{
	for (auto& pObject : m_PartObject[m_CurrentPlayerble])
		pObject->Late_Tick(fTimeDelta);

	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
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
		m_CurrentPlayerble = PLAYER_YAE;

	if (m_pGameInstance->GetKeyState(DIK_1) == CInput_Device::TAP
		|| m_pGameInstance->GetKeyState(DIK_2) == CInput_Device::TAP
		|| m_pGameInstance->GetKeyState(DIK_3) == CInput_Device::TAP
		|| m_pGameInstance->GetKeyState(DIK_4) == CInput_Device::TAP)
	{
		m_pCameraLook = dynamic_cast<CPartObject*>(m_PartObject[m_CurrentPlayerble][PART_BODY])->Get_CameraLook();
		m_pState_Manager->Set_CurrentPlayerble(m_CurrentPlayerble);
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

void CPlayer::SetUp_OnTerrain()
{
	if (m_isElementalAir || (m_CurrentPlayerble == PLAYER_WANDERER && m_iState == PLAYER_ELEMENTAL_END))
	{
		if (m_iState != PLAYER_FALL_ATTACK_LOOP && !m_isFly)
			return;
	}
		
	// 높이 조정
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float fHeight = m_pNavigationCom->Compute_Height(vPos);

	if (m_isJump)
	{		
		//점프하고 있을때 
		if (XMVectorGetY(vPos) < fHeight)
		{
			m_iState = PLAYER_FALL_GROUND_H;
			m_isJump = false;
		}

		return;
	}
	if (m_isElementalAir && m_iState == PLAYER_FALL_ATTACK_LOOP)
	{
		if (XMVectorGetY(vPos) <= fHeight)
			m_iState = PLAYER_ELEMENTAL_END;
		else return;
	}
	if (m_isFly)
	{
		m_isJump = false;
		if (XMVectorGetY(vPos) <= fHeight)
		{
			m_iState = PLAYER_FALL_GROUND_L;
			m_isFly = false;
		}
			
		else return;
	}
	if (m_isDrop)
	{
		if (XMVectorGetY(vPos) <= fHeight)
		{
			m_iState = m_pState_Manager->Set_CurrentState(CState_Manager::STATE_TYPE_FALL_GROUND);
			m_isDrop = false;
		}
		else return;
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vPos, fHeight));
}

void CPlayer::SetUp_CellType(_bool isMove)
{
	CCell::OPTION eOption = m_pNavigationCom->Get_OptionType();

	if (eOption == CCell::OPTION_FLY && isMove) //FLY
	{/*
		if (fHeight > XMVectorGetY(vPos))
			return;*/

		if (m_iState != PLAYER_FALL_ATTACK_LOOP && !m_isFly)
		{
			m_iState = m_pState_Manager->Set_CurrentState(CState_Manager::STATE_TYPE_FALL_ATTACK);
			m_isDrop = true;
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
	m_iState = m_pState_Manager->Update(fTimeDelta, PLAYER_STATE(m_iState));

	// 방랑자 원소 스킬 사용시 부유하고 있는지, 아닌지 체크 
	if (m_CurrentPlayerble == PLAYER_WANDERER && m_iState == PLAYER_ELEMENTAL_START)
		m_isElementalAir = true;
	else if (m_CurrentPlayerble == PLAYER_WANDERER && (m_iState == PLAYER_ELEMENTAL_END || m_iState == PLAYER_FALL_GROUND_L))
		m_isElementalAir = false;

	// fly 체크
	if (m_iState == PLAYER_FLY_START)
		m_isFly = true;

	// Jump 체크
	if (m_iState == PLAYER_JUMP || m_iState == PLAYER_JUMP_FOR_RUN || m_iState == PLAYER_JUMP_FOR_SPRINT)
		m_isJump = true;

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
	Desc.pFly = &m_isFly;
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
	CWanderer_Body::WANDERER_DESC WandererDesc{};
	WandererDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	WandererDesc.pState = &m_iState;
	WandererDesc.pDirState = &m_iDirState;
	WandererDesc.pFly = &m_isFly;
	WandererDesc.pHill = &m_eHill;
	WandererDesc.isElementalAir = &m_isElementalAir;
	WandererDesc.fSpeedPecSec = 20.f;
	WandererDesc.fRotatePecSec = XMConvertToRadians(45.f);

	pGameObject = m_pGameInstance->Clone_Object(L"Prototype_GameObject_Player_Wanderer", &WandererDesc);
	if (nullptr == pGameObject)
		return E_FAIL;
	m_PartObject[PLAYER_WANDERER].emplace_back(pGameObject);

	//Yae
	pGameObject = m_pGameInstance->Clone_Object(L"Prototype_GameObject_Player_Yae", &Desc);
	if (nullptr == pGameObject)
		return E_FAIL;
	m_PartObject[PLAYER_YAE].emplace_back(pGameObject);

	

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

	// Yae
	pComponent = m_PartObject[PLAYER_YAE][PART_BODY]->Get_Component(L"Com_Model");
	if (nullptr == pComponent)
		return E_FAIL;

	WeaponDesc.pHandCombinedTransformationMatrix = dynamic_cast<CModel*>(pComponent)->Get_BoneCombinedTransformationMatrix("PRIVATE_WeaponRootCatalyst");
	if (nullptr == WeaponDesc.pHandCombinedTransformationMatrix)
		return E_FAIL;

	WeaponDesc.eWeaponType = CWeapon::WEAPON_CATALYSE;
	pGameObject = dynamic_cast<CPartObject*>(m_pGameInstance->Clone_Object(L"Prototype_GameObject_Weapon_Narukami", &WeaponDesc));
	if (nullptr == pGameObject)
		return E_FAIL;
	m_PartObject[PLAYER_YAE].emplace_back(pGameObject);

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
	CSkillObj::SKILLOBJ_DESC SKillObjDesc{};
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

	CGameObject* pGameObject = dynamic_cast<CPartObject*>(m_pGameInstance->Clone_Object(L"Prototype_GameObject_SkillObj_Gohei", &SKillObjDesc));
	if (nullptr == pGameObject)
		return E_FAIL;

	m_PartObject[PLAYER_YAE].emplace_back(pGameObject);

	return S_OK;
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
		if (!m_isElementalAir) m_pTransformCom->LookForCamera(m_pGameInstance->Get_CamLook(), XMConvertToRadians(180.f));
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

	Safe_Release(m_pState_Manager);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pColliderCom);

	for (auto& PartObjects : m_PartObject)
	{
		for (auto& pPartObject : PartObjects)
			Safe_Release(pPartObject);
		PartObjects.clear();
	}
}
