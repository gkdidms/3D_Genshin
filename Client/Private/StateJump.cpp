#include "StateJump.h"

#include "State_Manager.h"

CStateJump::CStateJump()
	: CState{}
{
}

PLAYER_STATE CStateJump::Enter(class CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	if (CurrentState == PLAYER_IDLE)
		return PLAYER_JUMP;
	if (CurrentState == PLAYER_RUN)
		return PLAYER_JUMP_FOR_RUN;
	if (CurrentState == PLAYER_SPRINT)
		return PLAYER_SPRINT;

	return PLAYER_JUMP;
}

PLAYER_STATE CStateJump::Update(const _float& fTimeDelta, CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	return CurrentState;
}

PLAYER_STATE CStateJump::Exit(CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	PLAYER_STATE eState{ CurrentState };

	if (CurrentState == PLAYER_JUMP)
		return PLAYER_FALL_GROUND_H;

	if (CurrentState == PLAYER_JUMP_FOR_RUN)
		return PLAYER_FALL_GROUND_FOR_RUN;

	if (CurrentState == PLAYER_JUMP_FOR_SPRINT)
		return PLAYER_FALL_GROUND_FOR_SPRINT;

	if ((eState = ToIdle(pStateManager, CurrentState)) != CurrentState)
		return eState;

	if ((eState = ToRun(pStateManager, CurrentState)) != CurrentState)
		return eState;

	if ((eState = ToSprint(pStateManager, CurrentState)) != CurrentState)
		return eState;

	return __super::ToIdle(pStateManager, CurrentState);
}

PLAYER_STATE CStateJump::ToIdle(CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	if (CurrentState == PLAYER_FALL_GROUND_H)
	{
		return __super::ToIdle(pStateManager, CurrentState);
	}

	return CurrentState;
}

PLAYER_STATE CStateJump::ToRun(CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	if (CurrentState == PLAYER_FALL_GROUND_FOR_RUN)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_RUN, PLAYER_JUMP_FOR_RUN);

	return CurrentState;
}

PLAYER_STATE CStateJump::ToSprint(CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	if (CurrentState == PLAYER_FALL_GROUND_FOR_SPRINT)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_SPRINT, PLAYER_JUMP_FOR_SPRINT);

	return CurrentState;
}

CStateJump* CStateJump::Create()
{
	return new CStateJump();
}

void CStateJump::Free()
{
	__super::Free();
}
