#include "Player.h"

#include "GameInstance.h"
#include "PartObject_Body.h"
#include "PartObject.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject { pDevice, pContext }
{
}

CPlayer::CPlayer(const CPlayer& rhs)
	: CGameObject { rhs }
{
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

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	return S_OK;
}

void CPlayer::Priority_Tick(const _float& fTimeDelta)
{
	for (auto& pObject : m_PartObject)
		pObject->Priority_Tick(fTimeDelta);
}

void CPlayer::Tick(const _float& fTimeDelta)
{
	Input_Key(fTimeDelta);

	for (auto& pObject : m_PartObject)
		pObject->Tick(fTimeDelta);
}

void CPlayer::Late_Tick(const _float& fTimeDelta)
{
	_float4 vPos;
	m_PartObject[PART_BODY]->Set_PlayerPos(&vPos);
	XMStoreFloat4(&vPos, XMVector3TransformCoord(XMLoadFloat4(&vPos), m_pTransformCom->Get_WorldMatrix()));
	//_vector vMovePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION) + XMLoadFloat4(&vPos);
	_vector vMovePos = XMVectorSetW(XMLoadFloat4(&vPos), 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vMovePos);

	for (auto& pObject : m_PartObject)
		pObject->Late_Tick(fTimeDelta);

	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CPlayer::Render()
{
	for (auto& pObject : m_PartObject)
		pObject->Render();

	return S_OK;
}

HRESULT CPlayer::Ready_PartObjects()
{
	//PartObject::Body
	CPartObject_Body::BODY_DESC Desc{};

	Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	Desc.strPrototypeModelTag = L"Prototype_Component_Model_Tighnari";
	Desc.pState = &m_iState;
	Desc.fSpeedPecSec = 20.f;
	Desc.fRotatePecSec = XMConvertToRadians(45.f);

	CPartObject* pBodyObject = dynamic_cast<CPartObject*>(m_pGameInstance->Clone_Object(L"Prototype_GameObject_Player_Body", &Desc));
	if (nullptr == pBodyObject)
		return E_FAIL;

	m_PartObject.emplace_back(pBodyObject);

	return S_OK;
}

void CPlayer::Input_Key(const _float& fTimeDelta)
{
	if (m_pGameInstance->GetMouseState(DIM_LB) == CInput_Device::TAP) // �Ϲ� ����
	{
		if (m_iNumMouseClick >= m_iMaxMouseClick)
			m_iNumMouseClick = 0;

		if (m_iNumMouseClick == 0)
			m_iState = PLAYER_ATTACK_1;
		else if (m_iNumMouseClick == 1)
			m_iState = PLAYER_ATTACK_2;
		else if (m_iNumMouseClick == 2)
			m_iState = PLAYER_ATTACK_3;
		else if (m_iNumMouseClick == 3)
			m_iState = PLAYER_ATTACK_4;

		++m_iNumMouseClick;
	}
	if (m_pGameInstance->GetKeyState(DIK_E) == CInput_Device::TAP) // ���ҽ�ų
	{
		m_iState = PLAYER_ELEMENTAL_1;
	}
	if (m_pGameInstance->GetKeyState(DIK_Q) == CInput_Device::TAP) // �������� ��ų
	{
		m_iState = PLAYER_ELENENTAL_BURST;
	}

	if (m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::HOLD)
	{
		m_iState = PLAYER_RUN;
	}
	if (m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::HOLD)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, -1.f, 0.f, 0.f), fTimeDelta);
	}
	if (m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::HOLD)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta);
	}
	if (m_pGameInstance->GetKeyState(DIK_S) == CInput_Device::HOLD)
	{
		m_iState = PLAYER_RUN;
	}

	if (m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::AWAY)
	{
		m_iState = PLAYER_STOP;
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

	for (auto& pPartObject : m_PartObject)
		Safe_Release(pPartObject);
	m_PartObject.clear();
}
