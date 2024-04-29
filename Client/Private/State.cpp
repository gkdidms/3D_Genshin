#include "State.h"

#include "GameInstance.h"
#include "State_Manager.h"

CState::CState()
	: m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

void CState::Free()
{
	Safe_Release(m_pGameInstance);
}

PLAYER_STATE CState::ToIdle(CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_IDEL, CurrentState);
}

PLAYER_STATE CState::ToRun(CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	if (m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_S) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::HOLD)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_RUN, CurrentState);

	return CurrentState;
}

PLAYER_STATE CState::ToSprint(CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	if (m_pGameInstance->GetKeyState(DIK_LSHIFT))
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_SPRINT, CurrentState);

	return CurrentState;
}

PLAYER_STATE CState::ToJump(CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	if (m_pGameInstance->GetKeyState(DIK_SPACE) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_JUMP, CurrentState);

	return CurrentState;
}

PLAYER_STATE CState::ToFly(CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	if (m_pGameInstance->GetKeyState(DIK_SPACE) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_FLY, CurrentState);

	return CurrentState;
}

PLAYER_STATE CState::ToAttack(CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	if (m_pGameInstance->GetMouseState(DIM_LB) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_ATTACK);

	return CurrentState;
}

PLAYER_STATE CState::ToElementalArt(CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	if (m_pGameInstance->GetKeyState(DIK_E) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_ELEMENTALART);

	return CurrentState;
}

PLAYER_STATE CState::ToElementalBurst(CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	if (m_pGameInstance->GetKeyState(DIK_Q) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_ELEMNETALBURST);

	return CurrentState;
}
