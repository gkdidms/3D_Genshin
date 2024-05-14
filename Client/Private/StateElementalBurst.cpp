#include "StateElementalBurst.h"

#include "StateManager.h"

CStateElementalBurst::CStateElementalBurst()
{
}

PLAYER_STATE CStateElementalBurst::Enter(class CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	return PLAYER_ELEMENTAL_BURST;
}

PLAYER_STATE CStateElementalBurst::Update(const _float& fTimeDelta, CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
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
