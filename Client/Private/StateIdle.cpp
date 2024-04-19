#include "StateIdle.h"

#include "GameInstance.h"
#include "State_Manager.h"

CStateIdle::CStateIdle()
	: CState{}
{
}

PLAYER_STATE CStateIdle::Enter(PLAYER_STATE CurrentState)
{
	return PLAYER_IDLE;
}

PLAYER_STATE CStateIdle::Update(const _float& fTimeDelta, class CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	if (m_pGameInstance->GetKeyState(DIK_SPACE) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_JUMP, PLAYER_IDLE);

	if (m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_S) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::HOLD)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_RUN);

	if (m_pGameInstance->GetMouseState(DIM_LB) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_ATTACK);

	if (m_pGameInstance->GetKeyState(DIK_E) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_ELEMENTALART);

	if (m_pGameInstance->GetKeyState(DIK_Q) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_ELEMNETALBURST);

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
