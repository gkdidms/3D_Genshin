#include "StateExtraAttack.h"

#include "StateManager.h"

CStateExtraAttack::CStateExtraAttack()
	: CState{}
{
}

PLAYER_STATE CStateExtraAttack::Enter(CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	m_fDuration = { 1.f };

	return PLAYER_EXTRA_ATTACK_START;
}

PLAYER_STATE CStateExtraAttack::Update(const _float& fTimeDelta, CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	PLAYER_STATE eState{ CurrentState };

	if (CurrentState == PLAYER_EXTRA_ATTACK_END)
	{
		if (m_fDuration <= m_fTime)
		{
			//다른 스킬 사용 가능
			if ((eState = __super::ToJump(pStateManager, CurrentState)) != CurrentState) return eState;

			if ((eState = __super::ToRun(pStateManager, CurrentState)) != CurrentState) return eState;

			if ((eState = __super::ToSprint(pStateManager, CurrentState)) != CurrentState) return eState;

			if ((eState = __super::ToAttack(pStateManager, CurrentState)) != CurrentState) return eState;

			if ((eState = __super::ToExtraAttack(pStateManager, CurrentState)) != CurrentState) return eState;

			if ((eState = __super::ToElementalArt(pStateManager, CurrentState)) != CurrentState) return eState;

			if ((eState = __super::ToElementalBurst(pStateManager, CurrentState)) != CurrentState) return eState;
		}
		else
			m_fTime += fTimeDelta;
	}
	return CurrentState;
}

PLAYER_STATE CStateExtraAttack::Exit(CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	if (CurrentState == PLAYER_EXTRA_ATTACK_START)
		return PLAYER_EXTRA_ATTACK;
	else if (CurrentState == PLAYER_EXTRA_ATTACK)
		return PLAYER_EXTRA_ATTACK_END;

	return pStateManager.Set_CurrentState(CStateManager::STATE_TYPE_IDEL);
}

CStateExtraAttack* CStateExtraAttack::Create()
{
	return new CStateExtraAttack();
}

void CStateExtraAttack::Free()
{
	__super::Free();
}
