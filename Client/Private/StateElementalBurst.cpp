#include "StateElementalBurst.h"

#include "StateManager.h"

CStateElementalBurst::CStateElementalBurst()
{
}

PLAYER_STATE CStateElementalBurst::Enter(class CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	m_fTime = { 3.f };
	return PLAYER_ELEMENTAL_BURST;
}

PLAYER_STATE CStateElementalBurst::Update(const _float& fTimeDelta, CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	if (m_fDuration <= m_fTime)
	{
		return __super::ToIdle(pStateManager, CurrentState);
	}
	else m_fTime += fTimeDelta;

	return CurrentState;
}

PLAYER_STATE CStateElementalBurst::Exit(CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	if (CurrentState == PLAYER_ELEMENTAL_BURST_END)
		return __super::ToIdle(pStateManager, CurrentState);

	return PLAYER_ELEMENTAL_BURST_END;
}

CStateElementalBurst* CStateElementalBurst::Create()
{
	return new CStateElementalBurst();
}

void CStateElementalBurst::Free()
{
	__super::Free();
}
