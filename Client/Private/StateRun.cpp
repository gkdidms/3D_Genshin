#include "StateRun.h"

#include "GameInstance.h"
#include "State_Manager.h"

CStateRun::CStateRun()
	: CState{}
{
}

PLAYER_STATE CStateRun::Enter(PLAYER_STATE CurrentState)
{
	if (CurrentState == PLAYER_SPRINT_STOP)
		return PLAYER_RUN;

	return PLAYER_RUN_START;
}

PLAYER_STATE CStateRun::Update(const _float& fTimeDelta, class CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	if (m_pGameInstance->GetKeyState(DIK_LSHIFT) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_SPRINT);

	if (m_pGameInstance->GetMouseState(DIM_LB) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_ATTACK);

	if (m_pGameInstance->GetKeyState(DIK_Q) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_ELEMNETALBURST);

	if (m_pGameInstance->GetKeyState(DIK_E) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_ELEMENTALART);

	if (m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::AWAY 
		|| m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::AWAY
		|| m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::AWAY
		|| m_pGameInstance->GetKeyState(DIK_S) == CInput_Device::AWAY)
		return PLAYER_RUN_STOP;

	if (m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_S) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::HOLD)
	{
		if (CurrentState == PLAYER_RUN_STOP)
			return PLAYER_RUN_START;
		else if (CurrentState == PLAYER_RUN)
			return PLAYER_RUN;
	}

	return CurrentState;
}

PLAYER_STATE CStateRun::Exit(class CState_Manager& pStateManager, PLAYER_STATE CurrentState) // 애니메이션 루프가 false인 애들이 애니메이션이 끝났을때 호출됨  
{
	if (m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_S) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::HOLD)
		return PLAYER_RUN;

	if (CurrentState == PLAYER_RUN_STOP)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_IDEL);

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
