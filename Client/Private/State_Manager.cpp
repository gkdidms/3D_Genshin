#include "State_Manager.h"

#include "GameInstance.h"

#include "StateRun.h"
#include "StateSprint.h"
#include "StateIdle.h"

IMPLEMENT_SINGLETON(CState_Manager)

CState_Manager::CState_Manager()
	:m_pGameInstance { CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

PLAYER_STATE CState_Manager::Set_CurrentState(STATE_TYPE Type)
{
	m_pCurrentState = m_States[Type];

	return m_pCurrentState->Enter();
}

HRESULT CState_Manager::Initialize()
{
	m_States[STATE_TYPE_IDEL] = CStateIdle::Create();
	if (nullptr == m_States[STATE_TYPE_IDEL])
		return E_FAIL;

	m_States[STATE_TYPE_RUN] = CStateRun::Create();
	if (nullptr == m_States[STATE_TYPE_RUN])
		return E_FAIL;

	m_States[STATE_TYPE_SPRINT] = CStateSprint::Create();
	if (nullptr == m_States[STATE_TYPE_SPRINT])
		return E_FAIL;

	return S_OK;
}

void CState_Manager::Free()
{
	Safe_Release(m_pGameInstance);

	for (auto& pState : m_States)
		Safe_Release(pState);
}
