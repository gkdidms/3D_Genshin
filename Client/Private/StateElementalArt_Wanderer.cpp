#include "StateElementalArt_Wanderer.h"

CStateElementalArt_Wanderer::CStateElementalArt_Wanderer()
	: CState{}
{
}

PLAYER_STATE CStateElementalArt_Wanderer::Enter()
{
	return PLAYER_ELEMENTAL_1;
}

PLAYER_STATE CStateElementalArt_Wanderer::Update(CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	return CurrentState;
}

PLAYER_STATE CStateElementalArt_Wanderer::Exit(CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	return PLAYER_ELEMENTAL_END;
}

CStateElementalArt_Wanderer* CStateElementalArt_Wanderer::Create()
{
	CStateElementalArt_Wanderer* pInstance = new CStateElementalArt_Wanderer();

	return pInstance;
}

void CStateElementalArt_Wanderer::Free()
{
	__super::Free();
}
