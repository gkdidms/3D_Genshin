#include "StateElementalArt_Wanderer.h"

#include "State_Manager.h"
#include "GameInstance.h"

CStateElementalArt_Wanderer::CStateElementalArt_Wanderer()
	: CState{}
{
}

PLAYER_STATE CStateElementalArt_Wanderer::Enter(PLAYER_STATE CurrentState)
{
	return PLAYER_ELEMENTAL_START;
}

PLAYER_STATE CStateElementalArt_Wanderer::Update(const _float& fTimeDelta, CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	if (CurrentState == PLAYER_FALL_ATTACK_LOOP && m_pGameInstance->GetKeyState(DIK_SPACE) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_FLY);

	if (CurrentState == PLAYER_ELEMENTAL_END || CurrentState == PLAYER_ELEMENTAL_START || CurrentState == PLAYER_FALL_ATTACK_LOOP) // 종료 시점이라면 키 입력 불가능
		return CurrentState;

	//일정 시간 지나면 풀려야 함
	m_fTime += fTimeDelta;
	m_fCurrentTime += fTimeDelta;

	//지속 시간이 끝나면
	if (m_fCurrentTime > m_fFinishTime)
		return PLAYER_FALL_ATTACK_LOOP;

	if (m_fTime < m_fAttackTime)
		return CurrentState;

	if (m_pGameInstance->GetKeyState(DIK_Q) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_ELEMNETALBURST);

	if ((CurrentState == PLAYER_ELEMENTAL_UP || CurrentState == PLAYER_ELEMENTAL_UP_START) && m_pGameInstance->GetKeyState(DIK_SPACE) == CInput_Device::NONE)
		return PLAYER_IDLE;

	if (m_pGameInstance->GetKeyState(DIK_SPACE) == CInput_Device::HOLD)
	{
		//이미 업 상태라면
		return (CurrentState != PLAYER_ELEMENTAL_UP_START || CurrentState != PLAYER_ELEMENTAL_UP) ? PLAYER_ELEMENTAL_UP : CurrentState;
	}

	if (m_pGameInstance->GetMouseState(DIM_LB) == CInput_Device::TAP)
	{
		m_fTime = 0;

		if (m_iAttackCount >= m_iMaxAttack)
			m_iAttackCount = 0;

		m_iAttackCount++;

		if (m_iAttackCount - 1 == 0)
			return PLAYER_ATTACK_1;
		else if (m_iAttackCount - 1 == 1)
			return PLAYER_ATTACK_2;
		else if (m_iAttackCount - 1 == 2)
			return PLAYER_ATTACK_3;
	}

	if (m_pGameInstance->GetKeyState(DIK_E) == CInput_Device::TAP)
		return PLAYER_FALL_ATTACK_LOOP;

	if (m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_S) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::HOLD)
	{
		if (CurrentState != PLAYER_RUN)
			return PLAYER_RUN_START;
	}

	if (m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::AWAY
		|| m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::AWAY
		|| m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::AWAY
		|| m_pGameInstance->GetKeyState(DIK_S) == CInput_Device::AWAY)
		return PLAYER_RUN_STOP;

	return CurrentState;
}

PLAYER_STATE CStateElementalArt_Wanderer::Exit(CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	if (CurrentState == PLAYER_RUN)
		return PLAYER_RUN;

	if (CurrentState == PLAYER_ATTACK_3)
		return PLAYER_ATTACK_END;

	if (CurrentState == PLAYER_ELEMENTAL_UP_START)
		return PLAYER_ELEMENTAL_UP;

	if (CurrentState == PLAYER_ATTACK_1
		|| CurrentState == PLAYER_ATTACK_2 
		|| CurrentState == PLAYER_ATTACK_END
		|| CurrentState == PLAYER_RUN_STOP 
		|| CurrentState == PLAYER_ELEMENTAL_START)
		return PLAYER_IDLE;

	if (CurrentState == PLAYER_RUN_START)
		return PLAYER_RUN;

	if (CurrentState == PLAYER_ELEMENTAL_END)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_IDEL);

	return PLAYER_FALL_ATTACK_LOOP;
}

CStateElementalArt_Wanderer* CStateElementalArt_Wanderer::Create()
{
	return new CStateElementalArt_Wanderer();
}

void CStateElementalArt_Wanderer::Free()
{
	__super::Free();
}
