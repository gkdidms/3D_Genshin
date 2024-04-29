#include "StateElementalBurst.h"

#include "State_Manager.h"

CStateElementalBurst::CStateElementalBurst()
{
}

PLAYER_STATE CStateElementalBurst::Enter(class CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	return PLAYER_ELEMENTAL_BURST;
}

PLAYER_STATE CStateElementalBurst::Update(const _float& fTimeDelta, CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	return CurrentState;
}

PLAYER_STATE CStateElementalBurst::Exit(CState_Manager& pStateManager, PLAYER_STATE CurrentState)
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
