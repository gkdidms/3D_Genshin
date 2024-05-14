#include "StateManager.h"

#include "GameInstance.h"

#include "StateRun.h"
#include "StateSprint.h"
#include "StateIdle.h"
#include "StateElementalArt.h"
#include "StateElementalBurst.h"
#include "StateAttack.h"
#include "StateJump.h"
#include "StateFly.h"
#include "StateFallAttack.h"
#include "StateFallGround.h"


IMPLEMENT_SINGLETON(CStateManager);

CStateManager::CStateManager()
	:m_pGameInstance { CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

PLAYER_STATE CStateManager::Set_CurrentState(STATE_TYPE Type, PLAYER_STATE CurrentState)
{
	m_StateType = Type;
	m_pCurrentState = m_PlayerStates[Type];

	return m_pCurrentState->Enter(*this, CurrentState);
}

HRESULT CStateManager::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	PLAYER_STATE_DESC* pDesc = static_cast<PLAYER_STATE_DESC*>(pArg);
	m_CurrentPlayerble = pDesc->pPlayerbleType;
	m_CurrentDir = pDesc->pPlayerDir;

	if (FAILED(Ready_State()))
		return E_FAIL;

	return S_OK;
}

HRESULT CStateManager::Ready_State()
{
	m_PlayerStates.emplace_back(CStateIdle::Create());
	m_PlayerStates.emplace_back(CStateAttack::Create());
	m_PlayerStates.emplace_back(CStateElementalArt::Create());
	m_PlayerStates.emplace_back(CStateElementalBurst::Create());
	m_PlayerStates.emplace_back(CStateRun::Create());
	m_PlayerStates.emplace_back(CStateSprint::Create());
	m_PlayerStates.emplace_back(CStateJump::Create());
	m_PlayerStates.emplace_back(CStateFly::Create());
	m_PlayerStates.emplace_back(CStateFallAttack::Create());
	m_PlayerStates.emplace_back(CStateFallGround::Create());

	return S_OK;
}

void CStateManager::Free()
{
	Safe_Release(m_pCurrentState);

	Safe_Release(m_pGameInstance);
}
