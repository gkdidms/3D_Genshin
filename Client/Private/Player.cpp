#include "Player.h"

#include "GameInstance.h"
#include "PartObject_Body.h"
#include "State_Manager.h"

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

	m_pState_Manager->Initialize();
	m_pState_Manager->Set_CurrentState(CState_Manager::STATE_TYPE_IDEL);

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	return S_OK;
}

void CPlayer::Priority_Tick(const _float& fTimeDelta)
{
	for (auto& pObject : m_PartObject[m_CurrentPlayerble])
		pObject->Priority_Tick(fTimeDelta);
}

void CPlayer::Tick(const _float& fTimeDelta)
{
	Input_Key(fTimeDelta);

	for (auto& pObject : m_PartObject[m_CurrentPlayerble])
		pObject->Tick(fTimeDelta);
}

void CPlayer::Late_Tick(const _float& fTimeDelta)
{
	_float4 vPos;
	m_PartObject[m_CurrentPlayerble][PART_BODY]->Set_PlayerPos(&vPos);
	XMStoreFloat4(&vPos, XMVector3TransformCoord(XMLoadFloat4(&vPos), m_pTransformCom->Get_WorldMatrix()));
	_vector vMovePos = XMVectorSetW(XMLoadFloat4(&vPos), 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vMovePos);

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

HRESULT CPlayer::Ready_PartObjects()
{
	//PartObject::Body
	CPartObject_Body::PART_DESC Desc{};

	Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	Desc.pState = &m_iState;
	Desc.fSpeedPecSec = 20.f;
	Desc.fRotatePecSec = XMConvertToRadians(45.f);

	CPartObject* pBodyObject = dynamic_cast<CPartObject*>(m_pGameInstance->Clone_Object(L"Prototype_GameObject_Player_Tighnari", &Desc));
	if (nullptr == pBodyObject)
		return E_FAIL;

	m_PartObject[PLAYER_TIGHNARI].emplace_back(pBodyObject);

	pBodyObject = dynamic_cast<CPartObject*>(m_pGameInstance->Clone_Object(L"Prototype_GameObject_Player_Nilou", &Desc));
	if (nullptr == pBodyObject)
		return E_FAIL;

	m_PartObject[PLAYER_NILOU].emplace_back(pBodyObject);

	pBodyObject = dynamic_cast<CPartObject*>(m_pGameInstance->Clone_Object(L"Prototype_GameObject_Player_Wanderer", &Desc));
	if (nullptr == pBodyObject)
		return E_FAIL;

	m_PartObject[PLAYER_WANDERER].emplace_back(pBodyObject);

	pBodyObject = dynamic_cast<CPartObject*>(m_pGameInstance->Clone_Object(L"Prototype_GameObject_Player_Yae", &Desc));
	if (nullptr == pBodyObject)
		return E_FAIL;

	m_PartObject[PLAYER_YAE].emplace_back(pBodyObject);

	return S_OK;
}

void CPlayer::Input_Key(const _float& fTimeDelta)
{
	m_iState = m_pState_Manager->Update(PLAYER_STATE(m_iState));

	// 뛰기 제어
	if (m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::HOLD) // 앞
	{
		m_pTransformCom->LookForCamera(m_pGameInstance->Get_CamLook(), XMConvertToRadians(0.f));
	}
	if (m_pGameInstance->GetKeyState(DIK_S) == CInput_Device::HOLD) // 뒤
	{
		m_pTransformCom->LookForCamera(m_pGameInstance->Get_CamLook(), XMConvertToRadians(180.f));
	}
	if (m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::TAP) // 왼쪽
	{
		m_pTransformCom->LookForCamera(m_pGameInstance->Get_CamLook(), XMConvertToRadians(-90.f));
	}
	if (m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::TAP) // 오른쪽 
	{
		m_pTransformCom->LookForCamera(m_pGameInstance->Get_CamLook(), XMConvertToRadians(90.f));
	}
	if (m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::HOLD && m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::HOLD) // 왼쪽 사이드
	{
		m_pTransformCom->LookForCamera(m_pGameInstance->Get_CamLook(), XMConvertToRadians(-45.f));
	}
	if (m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::HOLD && m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::HOLD) // 오른쪽 사이드
	{
		m_pTransformCom->LookForCamera(m_pGameInstance->Get_CamLook(), XMConvertToRadians(45.f));
	}

	//캐릭터 변경
	if (m_pGameInstance->GetKeyState(DIK_1) == CInput_Device::TAP)
		m_CurrentPlayerble = PLAYER_TIGHNARI;

	if (m_pGameInstance->GetKeyState(DIK_2) == CInput_Device::TAP)
		m_CurrentPlayerble = PLAYER_NILOU;

	if (m_pGameInstance->GetKeyState(DIK_3) == CInput_Device::TAP)
		m_CurrentPlayerble = PLAYER_WANDERER;

	if (m_pGameInstance->GetKeyState(DIK_4) == CInput_Device::TAP)
		m_CurrentPlayerble = PLAYER_YAE;
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
