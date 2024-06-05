#include "StateHit.h"

CStateHit::CStateHit()
	: CState{}
{
}

PLAYER_STATE CStateHit::Enter(CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	m_fDuration = 0.3f;
	return PLAYER_HIT;
}

PLAYER_STATE CStateHit::Update(const _float& fTimeDelta, CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	PLAYER_STATE eState{ CurrentState };

	m_fTime += fTimeDelta;

	if (m_fDuration > m_fTime)
		return eState;

	if ((eState = __super::ToJump(pStateManager, CurrentState)) != CurrentState) return eState;

	if ((eState = __super::ToRun(pStateManager, CurrentState)) != CurrentState) return eState;

	if ((eState = __super::ToSprint(pStateManager, CurrentState)) != CurrentState) return eState;

	if ((eState = __super::ToAttack(pStateManager, CurrentState)) != CurrentState) return eState;

	if ((eState = __super::ToExtraAttack(pStateManager, CurrentState)) != CurrentState) return eState;

	if ((eState = __super::ToElementalArt(pStateManager, CurrentState)) != CurrentState) return eState;

	if ((eState = __super::ToElementalBurst(pStateManager, CurrentState)) != CurrentState) return eState;

	return eState;
}

PLAYER_STATE CStateHit::Exit(CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	return __super::ToIdle(pStateManager, CurrentState);
}

CStateHit* CStateHit::Create()
{
	return new CStateHit;
}

void CStateHit::Free()
{
	__super::Free();
}
