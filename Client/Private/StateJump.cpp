#include "StateJump.h"

#include "State_Manager.h"

CStateJump::CStateJump()
{
}

CStateJump::~CStateJump()
{
}

PLAYER_STATE CStateJump::Enter(PLAYER_STATE CurrentState)
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
	if (CurrentState == PLAYER_JUMP)
		return PLAYER_FALL_GROUND_H;

	if (CurrentState == PLAYER_JUMP_FOR_RUN)
		return PLAYER_FALL_GROUND_FOR_RUN;

	if (CurrentState == PLAYER_JUMP_FOR_SPRINT)
		return PLAYER_FALL_GROUND_FOR_SPRINT;

	if (CurrentState == PLAYER_FALL_GROUND_H)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_IDEL, PLAYER_JUMP);

	if (CurrentState == PLAYER_FALL_GROUND_FOR_RUN)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_RUN, PLAYER_JUMP_FOR_RUN);

	if (CurrentState == PLAYER_FALL_GROUND_FOR_SPRINT)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_SPRINT, PLAYER_JUMP_FOR_SPRINT);


	return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_IDEL, PLAYER_JUMP);
}

CStateJump* CStateJump::Create()
{
	return new CStateJump();
}

void CStateJump::Free()
{
	__super::Free();
}
