#include "Avatar_UI.h"

#include "GameInstance.h"
#include "StateManager.h"

#include "Avatar_Side.h"
#include "Avatar_Side_Select.h"

CAvatar_UI::CAvatar_UI(ID3D11Device* pDevice, ID3D11DeviceContext* pConext)
	: CUI{ pDevice, pConext},
	m_pStateManager{ CStateManager::GetInstance() }
{
	Safe_AddRef(m_pStateManager);
}

CAvatar_UI::CAvatar_UI(const CAvatar_UI& rhs)
	: CUI { rhs },
	m_pStateManager{ rhs.m_pStateManager }
{
	Safe_AddRef(m_pStateManager);
}

HRESULT CAvatar_UI::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAvatar_UI::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Btn()))
		return E_FAIL;

	return S_OK;
}

void CAvatar_UI::Priority_Tick(const _float& fTimeDelta)
{
}

void CAvatar_UI::Tick(const _float& fTimeDelta)
{
}

void CAvatar_UI::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_UI, this);
}

HRESULT CAvatar_UI::Render()
{
	_uint iIndex = 0;
	_uint iCurrentPlayerble = m_pStateManager->Get_Playerble();
	for (auto& pAvatarSide : m_AvatarSides)
	{
		if (iIndex++ == iCurrentPlayerble)
			continue;

		pAvatarSide->Render();
	}

	m_AvatarSelect[iCurrentPlayerble]->Render();
		
	return S_OK;
}

HRESULT CAvatar_UI::Ready_Btn()
{
	CAvatar_Side::AVATAR_SIDE_DESC SideDesc{};
	_float fDistance = { 70.f };
	_float fHeight = { 130.f };
	SideDesc.vPos = _float2(0.f, fHeight);
	SideDesc.iPlayerble = CPlayer::PLAYER_TIGHNARI;
	CGameObject* pTighanriAvatarSide = m_pGameInstance->Clone_Object(L"Prototype_GameObject_UI_Avatar_Side", &SideDesc);
	m_AvatarSides[CPlayer::PLAYER_TIGHNARI] = pTighanriAvatarSide;

	SideDesc.vPos = _float2(0.f, fHeight - fDistance * 1);
	SideDesc.iPlayerble = CPlayer::PLAYER_NILOU;
	CGameObject* pNilouAvatarSide = m_pGameInstance->Clone_Object(L"Prototype_GameObject_UI_Avatar_Side", &SideDesc);
	m_AvatarSides[CPlayer::PLAYER_NILOU] = pNilouAvatarSide;

	SideDesc.vPos = _float2(0.f, fHeight - fDistance * 2);
	SideDesc.iPlayerble = CPlayer::PLAYER_WANDERER;
	CGameObject* pWandererAvatarSide = m_pGameInstance->Clone_Object(L"Prototype_GameObject_UI_Avatar_Side", &SideDesc);
	m_AvatarSides[CPlayer::PLAYER_WANDERER] = pWandererAvatarSide;

	SideDesc.vPos = _float2(0.f, fHeight - fDistance * 3);
	SideDesc.iPlayerble = CPlayer::PLAYER_FEIYAN;
	CGameObject* pFeiyanAvatarSide = m_pGameInstance->Clone_Object(L"Prototype_GameObject_UI_Avatar_Side", &SideDesc);
	m_AvatarSides[CPlayer::PLAYER_FEIYAN] = pFeiyanAvatarSide;


	CAvatar_Side_Select::AVATAR_SIDE_SELECT_DESC SideSelectDesc{};
	fDistance = { 70.f };
	fHeight = { 130.f };
	SideSelectDesc.vPos = _float2(0.f, fHeight);
	SideSelectDesc.iPlayerble = CPlayer::PLAYER_TIGHNARI;
	CGameObject* pTighanriAvatarSelect = m_pGameInstance->Clone_Object(L"Prototype_GameObject_UI_Avatar_Select", &SideSelectDesc);
	m_AvatarSelect[CPlayer::PLAYER_TIGHNARI] = pTighanriAvatarSelect;

	SideSelectDesc.vPos = _float2(0.f, fHeight - fDistance * 1);
	SideSelectDesc.iPlayerble = CPlayer::PLAYER_NILOU;
	CGameObject* pNilouAvatarSelect = m_pGameInstance->Clone_Object(L"Prototype_GameObject_UI_Avatar_Select", &SideSelectDesc);
	m_AvatarSelect[CPlayer::PLAYER_NILOU] = pNilouAvatarSelect;

	SideSelectDesc.vPos = _float2(0.f, fHeight - fDistance * 2);
	SideSelectDesc.iPlayerble = CPlayer::PLAYER_WANDERER;
	CGameObject* pWandererAvatarSelect = m_pGameInstance->Clone_Object(L"Prototype_GameObject_UI_Avatar_Select", &SideSelectDesc);
	m_AvatarSelect[CPlayer::PLAYER_WANDERER] = pWandererAvatarSelect;

	SideSelectDesc.vPos = _float2(0.f, fHeight - fDistance * 3);
	SideSelectDesc.iPlayerble = CPlayer::PLAYER_FEIYAN;
	CGameObject* pFeiyanAvatarSelect = m_pGameInstance->Clone_Object(L"Prototype_GameObject_UI_Avatar_Select", &SideSelectDesc);
	m_AvatarSelect[CPlayer::PLAYER_FEIYAN] = pFeiyanAvatarSelect;

	return S_OK;
}

CAvatar_UI* CAvatar_UI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pConext)
{
	CAvatar_UI* pInstance = new CAvatar_UI(pDevice, pConext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CAvatar_UI::Clone(void* pArg)
{
	CAvatar_UI* pInstance = new CAvatar_UI(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CAvatar_UI::Free()
{
	__super::Free();

	for (auto& pSide : m_AvatarSides)
		Safe_Release(pSide);

	for(auto & pSelect : m_AvatarSelect)
		Safe_Release(pSelect);

	Safe_Release(m_pStateManager);
}
