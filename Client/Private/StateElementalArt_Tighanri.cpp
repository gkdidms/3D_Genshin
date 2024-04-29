#include "StateElementalArt_Tighanri.h"

#include "State_Manager.h"

CStateElementalArt_Tighanri::CStateElementalArt_Tighanri()
{
}

PLAYER_STATE CStateElementalArt_Tighanri::Enter(class CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	return PLAYER_ELEMENTAL_1;
}

PLAYER_STATE CStateElementalArt_Tighanri::Update(const _float& fTimeDelta, CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	return CurrentState;
}

PLAYER_STATE CStateElementalArt_Tighanri::Exit(CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	if (CurrentState == PLAYER_ELEMENTAL_END)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_IDEL);

	return PLAYER_ELEMENTAL_END;
}

CStateElementalArt_Tighanri* CStateElementalArt_Tighanri::Create()
{
	return new CStateElementalArt_Tighanri();
}

void CStateElementalArt_Tighanri::Free()
{
	__super::Free();
}
