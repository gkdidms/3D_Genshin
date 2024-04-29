#include "StateIdle.h"

#include "GameInstance.h"
#include "State_Manager.h"

CStateIdle::CStateIdle()
	: CState{}
{
}

PLAYER_STATE CStateIdle::Enter(class CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	PLAYER_STATE eState{ CurrentState };

	if ((eState = __super::ToJump(pStateManager, CurrentState)) != CurrentState) return eState;

	if ((eState = __super::ToRun(pStateManager, CurrentState)) != CurrentState) return eState;

	if ((eState = __super::ToRun(pStateManager, CurrentState)) != CurrentState) return eState;

	if ((eState = __super::ToSprint(pStateManager, CurrentState)) != CurrentState) return eState;

	if ((eState = __super::ToAttack(pStateManager, CurrentState)) != CurrentState) return eState;

	if ((eState = __super::ToElementalArt(pStateManager, CurrentState)) != CurrentState) return eState;

	if ((eState = __super::ToElementalBurst(pStateManager, CurrentState)) != CurrentState) return eState;

	return eState;
}

PLAYER_STATE CStateIdle::Update(const _float& fTimeDelta, class CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	PLAYER_STATE eState{ CurrentState };

	if ((eState = __super::ToJump(pStateManager, CurrentState)) != CurrentState) return eState;

	if ((eState = __super::ToRun(pStateManager, CurrentState)) != CurrentState) return eState;

	if ((eState = __super::ToRun(pStateManager, CurrentState)) != CurrentState) return eState;

	if ((eState = __super::ToSprint(pStateManager, CurrentState)) != CurrentState) return eState;

	if ((eState = __super::ToAttack(pStateManager, CurrentState)) != CurrentState) return eState;
	
	if ((eState = __super::ToElementalArt(pStateManager, CurrentState)) != CurrentState) return eState;
	
	if ((eState = __super::ToElementalBurst(pStateManager, CurrentState)) != CurrentState) return eState;

	return eState;
}

PLAYER_STATE CStateIdle::Exit(class CState_Manager& pStateManager, PLAYER_STATE CurrentState)
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
