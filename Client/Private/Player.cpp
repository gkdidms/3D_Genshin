#include "Player.h"

#include "GameInstance.h"
#include "PartObject_Body.h"
#include "State_Manager.h"
#include "Wanderer_Body.h"

#include "Weapon_Ayus.h"

#include "SkillObj.h"

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

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC Desc{};
	Desc.fSpeedPecSec = 20.f;
	Desc.fRotatePecSec = XMConvertToRadians(60.f);

	m_pState_Manager->Set_CurrentState(CState_Manager::STATE_TYPE_IDEL);

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Bodys()))
		return E_FAIL;

	if (FAILED(Ready_Weapons()))
		return E_FAIL;

	if (FAILED(Ready_SkillObjs()))
		return E_FAIL;

	m_pCameraLook = dynamic_cast<CPartObject*>(m_PartObject[m_CurrentPlayerble][PART_BODY])->Get_CameraLook();

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
	Input_Key(fTimeDelta);
	
	for (auto& pObject : m_PartObject[m_CurrentPlayerble])
		pObject->Tick(fTimeDelta);
}

void CPlayer::Late_Tick(const _float& fTimeDelta)
{
	_vector vPos;
	_float4x4 RootMatrix;
	_matrix WorldMatrix = m_pTransformCom->Get_WorldMatrix();

	dynamic_cast<CPartObject*>(m_PartObject[m_CurrentPlayerble][PART_BODY])->Set_PlayerPos(&RootMatrix);
	XMStoreFloat4x4(&RootMatrix, XMLoadFloat4x4(&RootMatrix) * -1.f);
	vPos = XMVector3TransformCoord(XMLoadFloat4x4(&RootMatrix).r[3], WorldMatrix);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(vPos, 1.f));
	
	for (auto& pObject : m_PartObject[m_CurrentPlayerble])
		pObject->Late_Tick(fTimeDelta);

	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CPlayer::Render()
{
	for (auto& pObject : m_PartObject[m_CurrentPlayerble])
		pObject->Render();

	return S_OK;
}

void CPlayer::Change_Playerble() // �ڵ� �����ϱ�
{
	//ĳ���� ����
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

HRESULT CPlayer::Ready_Bodys()
{
	//PartObject::Body
	CPartObject::PART_DESC Desc{};

	Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	Desc.pState = &m_iState;
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
	WandererDesc.isElementalAir = &m_IsElementalAir;
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

	pGameObject = dynamic_cast<CPartObject*>(m_pGameInstance->Clone_Object(L"Prototype_GameObject_Weapon_Alaya", &WeaponDesc));
	if (nullptr == pGameObject)
		return E_FAIL;
	m_PartObject[PLAYER_WANDERER].emplace_back(pGameObject);

	//Nilou
	//pComponent = m_PartObject[PLAYER_NILOU][PART_BODY]->Get_Component(L"Com_Model");
	//if (nullptr == pComponent)
	//	return E_FAIL;

	//WeaponDesc.pHandCombinedTransformationMatrix = dynamic_cast<CModel*>(pComponent)->Get_BoneCombinedTransformationMatrix("PRIVATE_WeaponArrow");
	//if (nullptr == WeaponDesc.pHandCombinedTransformationMatrix)
	//	return E_FAIL;

	//WeaponDesc.pBackCombinedTransformationMatrix = dynamic_cast<CModel*>(pComponent)->Get_BoneCombinedTransformationMatrix("PRIVATE_WeaponRootBow");
	//if (nullptr == WeaponDesc.pBackCombinedTransformationMatrix)
	//	return E_FAIL;

	//pGameObject = dynamic_cast<CPartObject*>(m_pGameInstance->Clone_Object(L"Prototype_GameObject_Weapon_Regalis", &WeaponDesc));
	//if (nullptr == pGameObject)
	//	return E_FAIL;
	//m_PartObject[PLAYER_NILOU].emplace_back(pGameObject);

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
	// ���� ���� ������Ʈ
	m_iState = m_pState_Manager->Update(fTimeDelta, PLAYER_STATE(m_iState));

	// ����� ���� ��ų ���� �����ϰ� �ִ���, �ƴ��� üũ 
	if (m_CurrentPlayerble == PLAYER_WANDERER && m_iState == PLAYER_ELEMENTAL_START)
		m_IsElementalAir = true;
	else if (m_CurrentPlayerble == PLAYER_WANDERER && m_iState == PLAYER_ELEMENTAL_END)
		m_IsElementalAir = false;

	// ��Ʈ �ִϸ��̼� ���� �� ���� �ּ�
	// �ٱ� ����
	if (m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::HOLD) // ��
	{
		m_pTransformCom->LookForCamera(m_pGameInstance->Get_CamLook(), XMConvertToRadians(0.f));
		m_iDirState = DIR_STRIGHT;
	}
	if (m_pGameInstance->GetKeyState(DIK_S) == CInput_Device::HOLD) // ��
	{
		if (!m_IsElementalAir) m_pTransformCom->LookForCamera(m_pGameInstance->Get_CamLook(), XMConvertToRadians(180.f));
		m_iDirState = DIR_BACKWORK;
	}
	if (m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::TAP) // ����
	{
		m_pTransformCom->LookForCamera(m_pGameInstance->Get_CamLook(), XMConvertToRadians(-90.f));
		m_iDirState = DIR_STRIGHT;
	}
	if (m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::TAP) // ������ 
	{
		m_pTransformCom->LookForCamera(m_pGameInstance->Get_CamLook(), XMConvertToRadians(90.f));
		m_iDirState = DIR_STRIGHT;
	}
	if (m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::HOLD && m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::HOLD) // ���� ���̵�
	{
		m_pTransformCom->LookForCamera(m_pGameInstance->Get_CamLook(), XMConvertToRadians(-45.f));
		m_iDirState = DIR_LEFT_SIDE;
	}
	if (m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::HOLD && m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::HOLD) // ������ ���̵�
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

	for (auto& PartObjects : m_PartObject)
	{
		for (auto& pPartObject : PartObjects)
			Safe_Release(pPartObject);
		PartObjects.clear();
	}
}
