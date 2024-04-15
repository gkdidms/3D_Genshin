#include "StateRun.h"

#include "GameInstance.h"
#include "State_Manager.h"

CStateRun::CStateRun()
	: CState{}
{
}

PLAYER_STATE CStateRun::Enter()
{
	return PLAYER_RUN_START;
}

PLAYER_STATE CStateRun::Update(class CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	if (m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::AWAY 
		|| m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::AWAY
		|| m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::AWAY
		|| m_pGameInstance->GetKeyState(DIK_S) == CInput_Device::AWAY)
		return PLAYER_RUN_STOP;

	if (m_pGameInstance->GetKeyState(DIK_LSHIFT) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_SPRINT);

	if (m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_S) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::HOLD)
		return PLAYER_RUN;

	if (m_pGameInstance->GetMouseState(DIM_LB) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_ATTACK);

	return CurrentState;
}

PLAYER_STATE CStateRun::Exit(class CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	if (m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_S) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::HOLD)
		return PLAYER_RUN;

	return PLAYER_RUN_STOP;
}

CStateRun* CStateRun::Create()
{
	CStateRun* pInstance = new CStateRun();

	return pInstance;
}

void CStateRun::Free()
{
	__super::Free();
}
