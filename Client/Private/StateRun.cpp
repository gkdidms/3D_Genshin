#include "StateRun.h"

#include "GameInstance.h"
#include "State_Manager.h"

CStateRun::CStateRun()
	: CState{}
{
}

PLAYER_STATE CStateRun::Enter(class CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	if (CurrentState == PLAYER_SPRINT_STOP || CurrentState == PLAYER_JUMP_FOR_RUN || CurrentState == PLAYER_FALL_GROUND_FOR_RUN)
		return PLAYER_RUN;

	return PLAYER_RUN;
}

PLAYER_STATE CStateRun::Update(const _float& fTimeDelta, class CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	PLAYER_STATE eState{ CurrentState };

	if (CurrentState == PLAYER_RUN && m_pGameInstance->GetKeyState(DIK_SPACE) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_JUMP, PLAYER_RUN);

	if ((eState = __super::ToSprint(pStateManager, CurrentState)) != CurrentState)
		return eState;

	if ((eState = __super::ToAttack(pStateManager, CurrentState)) != CurrentState)
		return eState;

	if ((eState = __super::ToElementalArt(pStateManager, CurrentState)) != CurrentState)
		return eState;

	if ((eState = __super::ToElementalBurst(pStateManager, CurrentState)) != CurrentState)
		return eState;

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
		return __super::ToIdle(pStateManager, CurrentState);

	return PLAYER_RUN_STOP;
}

CStateRun* CStateRun::Create()
{
	return new CStateRun();
}

void CStateRun::Free()
{
	__super::Free();
}
