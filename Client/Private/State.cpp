#include "State.h"

#include "GameInstance.h"
#include "StateManager.h"

CState::CState()
	: m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

void CState::Free()
{
	Safe_Release(m_pGameInstance);
}

PLAYER_STATE CState::ToIdle(CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	return pStateManager.Set_CurrentState(CStateManager::STATE_TYPE_IDEL, CurrentState);
}

PLAYER_STATE CState::ToRun(CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	if (Check_Move())
		return pStateManager.Set_CurrentState(CStateManager::STATE_TYPE_RUN, CurrentState);

	return CurrentState;
}

PLAYER_STATE CState::ToSprint(CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	if (m_pGameInstance->GetKeyState(DIK_LSHIFT))
		return pStateManager.Set_CurrentState(CStateManager::STATE_TYPE_SPRINT, CurrentState);

	return CurrentState;
}

PLAYER_STATE CState::ToJump(CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	if (m_pGameInstance->GetKeyState(DIK_SPACE) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CStateManager::STATE_TYPE_JUMP, CurrentState);

	return CurrentState;
}

PLAYER_STATE CState::ToFly(CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	if (m_pGameInstance->GetKeyState(DIK_SPACE) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CStateManager::STATE_TYPE_FLY, CurrentState);

	return CurrentState;
}

PLAYER_STATE CState::ToAttack(CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	if (m_pGameInstance->GetMouseState(DIM_LB) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CStateManager::STATE_TYPE_ATTACK);

	return CurrentState;
}

PLAYER_STATE CState::ToExtraAttack(CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	if (m_pGameInstance->GetMouseState(DIM_RB) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CStateManager::STATE_TYPE_EXTRA_ATTACK);

	return CurrentState;
}

PLAYER_STATE CState::ToElementalArt(CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	if (m_pGameInstance->GetKeyState(DIK_E) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CStateManager::STATE_TYPE_ELEMENTALART);

	return CurrentState;
}

PLAYER_STATE CState::ToElementalBurst(CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	if (m_pGameInstance->GetKeyState(DIK_Q) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CStateManager::STATE_TYPE_ELEMNETALBURST);

	return CurrentState;
}

_bool CState::Check_Move()
{
	if (m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_S) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::HOLD)
		return true;
	else if (m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::AWAY
		|| m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::AWAY
		|| m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::AWAY
		|| m_pGameInstance->GetKeyState(DIK_S) == CInput_Device::AWAY)
		return false;

	return false;
}
