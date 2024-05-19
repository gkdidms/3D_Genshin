#include "StateIdle.h"

#include "GameInstance.h"
#include "StateManager.h"

CStateIdle::CStateIdle()
	: CState{}
{
}

PLAYER_STATE CStateIdle::Enter(class CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	PLAYER_STATE eState{ CurrentState };

	return PLAYER_IDLE;
}

PLAYER_STATE CStateIdle::Update(const _float& fTimeDelta, class CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	PLAYER_STATE eState{ CurrentState };

	if ((eState = __super::ToJump(pStateManager, CurrentState)) != CurrentState) return eState;

	if ((eState = __super::ToRun(pStateManager, CurrentState)) != CurrentState) return eState;

	if ((eState = __super::ToSprint(pStateManager, CurrentState)) != CurrentState) return eState;

	if ((eState = __super::ToAttack(pStateManager, CurrentState)) != CurrentState) return eState;

	if ((eState = __super::ToExtraAttack(pStateManager, CurrentState)) != CurrentState) return eState;
	
	if ((eState = __super::ToElementalArt(pStateManager, CurrentState)) != CurrentState) return eState;
	
	if ((eState = __super::ToElementalBurst(pStateManager, CurrentState)) != CurrentState) return eState;

	return PLAYER_IDLE;
}

PLAYER_STATE CStateIdle::Exit(class CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	return PLAYER_IDLE;
}

CStateIdle* CStateIdle::Create()
{
	return new CStateIdle();
}

void CStateIdle::Free()
{
	__super::Free();
}
