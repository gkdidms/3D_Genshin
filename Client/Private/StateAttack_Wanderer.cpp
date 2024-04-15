#include "StateAttack_Wanderer.h"

#include "State_Manager.h"
#include "GameInstance.h"

CStateAttack_Wanderer::CStateAttack_Wanderer()
	: CState{}
{
}

PLAYER_STATE CStateAttack_Wanderer::Enter()
{
	m_iAttackCount = 0;

	return PLAYER_ATTACK_1;
}

PLAYER_STATE CStateAttack_Wanderer::Update(CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	if (PLAYER_IDLE_PUT_AWAY == CurrentState) // attack -> idle »çÀÌ°ª
		return CurrentState;

	if (m_pGameInstance->GetMouseState(DIM_LB) == CInput_Device::TAP)
	{
		if (m_iAttackCount >= m_iMaxAttack)
			m_iAttackCount = 0;

		if (m_iAttackCount++ == 0)
			return PLAYER_ATTACK_1;
		else if (m_iAttackCount++ == 1)
			return PLAYER_ATTACK_2;
		else if (m_iAttackCount++ == 2)
			return PLAYER_ATTACK_3;
	}

	return CurrentState;
}

PLAYER_STATE CStateAttack_Wanderer::Exit(CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	if (PLAYER_IDLE_PUT_AWAY == CurrentState)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_IDEL);

	return PLAYER_IDLE_PUT_AWAY;
}

CStateAttack_Wanderer* CStateAttack_Wanderer::Create()
{
	CStateAttack_Wanderer* pInstance = new CStateAttack_Wanderer();

	return pInstance;
}

void CStateAttack_Wanderer::Free()
{
	__super::Free();
}
