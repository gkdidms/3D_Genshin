#include "StateIdle.h"

#include "GameInstance.h"
#include "State_Manager.h"

CStateIdle::CStateIdle()
	: CState{}
{
}

PLAYER_STATE CStateIdle::Enter()
{
	return PLAYER_IDLE;
}

PLAYER_STATE CStateIdle::Update(class CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	if (m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_S) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::HOLD)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_RUN);

	if (m_pGameInstance->GetMouseState(DIM_LB) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_ATTACK);

	return CurrentState;
}

PLAYER_STATE CStateIdle::Exit(class CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	return PLAYER_IDLE;
}

CStateIdle* CStateIdle::Create()
{
	CStateIdle* pInstance = new CStateIdle();

	return pInstance;
}

void CStateIdle::Free()
{
	__super::Free();
}
