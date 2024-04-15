#include "StateSprint.h"

#include "GameInstance.h"
#include "State_Manager.h"

CStateSprint::CStateSprint()
	: CState{}
{
}

PLAYER_STATE CStateSprint::Enter()
{
	return PLAYER_SPRINT_START;
}

PLAYER_STATE CStateSprint::Update(class CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	if (m_pGameInstance->GetKeyState(DIK_LSHIFT) == CInput_Device::HOLD)
		return PLAYER_SPRINT;

	if (CurrentState == PLAYER_SPRINT_STOP
		&& m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::HOLD
		&& m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::HOLD
		&& m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::HOLD
		&& m_pGameInstance->GetKeyState(DIK_S) == CInput_Device::HOLD)
		return PLAYER_SPRINT_TO_RUN;

	return CurrentState;
}

PLAYER_STATE CStateSprint::Exit(class CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	if (m_pGameInstance->GetKeyState(DIK_LSHIFT) == CInput_Device::HOLD)
		return PLAYER_SPRINT;

	if (CurrentState == PLAYER_SPRINT_STOP
		&& m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::HOLD
		&& m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::HOLD
		&& m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::HOLD
		&& m_pGameInstance->GetKeyState(DIK_S) == CInput_Device::HOLD)
		return PLAYER_SPRINT_TO_RUN;

	if (CurrentState == PLAYER_SPRINT_TO_RUN
		&& m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::HOLD
		&& m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::HOLD
		&& m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::HOLD
		&& m_pGameInstance->GetKeyState(DIK_S) == CInput_Device::HOLD)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_RUN);

	return PLAYER_SPRINT_STOP;
}

CStateSprint* CStateSprint::Create()
{
	CStateSprint* pInstance = new CStateSprint();

	return pInstance;
}

void CStateSprint::Free()
{
	__super::Free();
}
