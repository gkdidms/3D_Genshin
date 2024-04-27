#include "StateSprint.h"

#include "GameInstance.h"
#include "State_Manager.h"

CStateSprint::CStateSprint()
	: CState{}
{
}

PLAYER_STATE CStateSprint::Enter(PLAYER_STATE CurrentState)
{
	if (CurrentState == PLAYER_JUMP_FOR_SPRINT || CurrentState == PLAYER_FALL_GROUND_FOR_SPRINT)
		return PLAYER_SPRINT;

	return PLAYER_SPRINT_START;
}

PLAYER_STATE CStateSprint::Update(const _float& fTimeDelta, class CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	if (m_pGameInstance->GetMouseState(DIM_LB) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_ATTACK);

	if (CurrentState == PLAYER_SPRINT && m_pGameInstance->GetKeyState(DIK_SPACE) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_JUMP, PLAYER_SPRINT);

	if (CurrentState == PLAYER_SPRINT && m_pGameInstance->GetKeyState(DIK_LSHIFT) == CInput_Device::HOLD)
		return PLAYER_SPRINT;

	if (CurrentState == PLAYER_SPRINT && m_pGameInstance->GetKeyState(DIK_LSHIFT) == CInput_Device::AWAY)
		return PLAYER_SPRINT_STOP;

	if (m_pGameInstance->GetKeyState(DIK_E) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_ELEMENTALART);

	if (CurrentState == PLAYER_SPRINT_STOP && 
		( m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_S) == CInput_Device::HOLD))
		return PLAYER_SPRINT_TO_RUN;

	if (m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::AWAY
		|| m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::AWAY
		|| m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::AWAY
		|| m_pGameInstance->GetKeyState(DIK_S) == CInput_Device::AWAY)
		return PLAYER_SPRINT_STOP;

	return CurrentState;
}

PLAYER_STATE CStateSprint::Exit(class CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	if (m_pGameInstance->GetKeyState(DIK_LSHIFT) == CInput_Device::HOLD)
		return PLAYER_SPRINT;

	if (m_pGameInstance->GetKeyState(DIK_Q) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_ELEMNETALBURST);

	if (CurrentState == PLAYER_SPRINT_START &&
		(m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::HOLD
			|| m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::HOLD
			|| m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::HOLD
			|| m_pGameInstance->GetKeyState(DIK_S) == CInput_Device::HOLD))
		return  PLAYER_SPRINT_TO_RUN;

	if (CurrentState == PLAYER_SPRINT_TO_RUN &&
		(m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::HOLD
			|| m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::HOLD
			|| m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::HOLD
			|| m_pGameInstance->GetKeyState(DIK_S) == CInput_Device::HOLD))
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_RUN, PLAYER_SPRINT_STOP);

	if (CurrentState == PLAYER_SPRINT_STOP)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_IDEL);

	return PLAYER_SPRINT_STOP;
}

CStateSprint* CStateSprint::Create()
{
	return new CStateSprint();
}

void CStateSprint::Free()
{
	__super::Free();
}
