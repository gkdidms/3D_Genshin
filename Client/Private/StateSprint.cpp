#include "StateSprint.h"

#include "GameInstance.h"
#include "StateManager.h"

CStateSprint::CStateSprint()
	: CState{}
{
}

PLAYER_STATE CStateSprint::Enter(class CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	m_fCurrentTime = { 0.f };

	if (CurrentState == PLAYER_JUMP_FOR_SPRINT || CurrentState == PLAYER_FALL_GROUND_FOR_SPRINT)
		return PLAYER_SPRINT;

	return PLAYER_SPRINT_START;
}

PLAYER_STATE CStateSprint::Update(const _float& fTimeDelta, class CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	PLAYER_STATE eState{ CurrentState };

	if (CurrentState == PLAYER_SPRINT_STOP)
	{
		m_fCurrentTime += fTimeDelta;

		if (m_fDurationTime <= m_fCurrentTime && Check_Move())
			return 	pStateManager.Set_CurrentState(CStateManager::STATE_TYPE_RUN);

		return eState;
	}

	if (CurrentState == PLAYER_SPRINT_TO_RUN)
		return eState;

	if ((eState = ToAttack(pStateManager, CurrentState)) != CurrentState)
		return eState;

	if ((eState = ToElementalArt(pStateManager, CurrentState)) != CurrentState)
		return eState;

	if ((eState = ToElementalBurst(pStateManager, CurrentState)) != CurrentState)
		return eState;

	if ((eState = ToJump(pStateManager, CurrentState)) != CurrentState)
		return eState;

	if ((eState = ToSprint(pStateManager, CurrentState)) != CurrentState)
		return eState;

	return CurrentState;
}

PLAYER_STATE CStateSprint::Exit(class CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	PLAYER_STATE eState{ CurrentState };

	if (CurrentState == PLAYER_SPRINT_START &&
		(m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::HOLD
			|| m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::HOLD
			|| m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::HOLD
			|| m_pGameInstance->GetKeyState(DIK_S) == CInput_Device::HOLD))
		return  PLAYER_SPRINT_TO_RUN;

	if ((eState == ToRun(pStateManager, CurrentState)) != CurrentState)
		return eState;

	if ((eState == ToIdle(pStateManager, CurrentState)) != CurrentState)
		return eState;

	if (m_pGameInstance->GetKeyState(DIK_LSHIFT) == CInput_Device::HOLD)
		return PLAYER_SPRINT;

	return PLAYER_SPRINT_STOP;
}

PLAYER_STATE CStateSprint::ToIdle(CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	if (CurrentState == PLAYER_SPRINT_STOP)
		return __super::ToIdle(pStateManager, CurrentState);

	return CurrentState;
}

PLAYER_STATE CStateSprint::ToRun(CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	if (CurrentState == PLAYER_SPRINT_TO_RUN)
		return __super::ToRun(pStateManager, CurrentState);

	return CurrentState;
}

PLAYER_STATE CStateSprint::ToSprint(CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	if (CurrentState == PLAYER_SPRINT && m_pGameInstance->GetKeyState(DIK_LSHIFT) == CInput_Device::HOLD)
		return PLAYER_SPRINT;

	if (CurrentState == PLAYER_SPRINT && m_pGameInstance->GetKeyState(DIK_LSHIFT) == CInput_Device::AWAY)
		return PLAYER_SPRINT_STOP;

	if (CurrentState == PLAYER_SPRINT_STOP &&
		(m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::HOLD
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

PLAYER_STATE CStateSprint::ToJump(CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	if (CurrentState == PLAYER_SPRINT)
		return __super::ToJump(pStateManager, CurrentState);

	return CurrentState;
}

PLAYER_STATE CStateSprint::ToAttack(CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	return __super::ToAttack(pStateManager, CurrentState);
}

PLAYER_STATE CStateSprint::ToElementalArt(CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	return __super::ToElementalArt(pStateManager, CurrentState);
}

PLAYER_STATE CStateSprint::ToElementalBurst(CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	return __super::ToElementalBurst(pStateManager, CurrentState);
}

CStateSprint* CStateSprint::Create()
{
	return new CStateSprint();
}

void CStateSprint::Free()
{
	__super::Free();
}