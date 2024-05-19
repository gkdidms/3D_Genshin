#include "StateElementalArt_Feiyan.h"

#include "StateManager.h"

CStateElementalArt_Feiyan::CStateElementalArt_Feiyan()
{
}

HRESULT CStateElementalArt_Feiyan::Initialize()
{
	return S_OK;
}

PLAYER_STATE CStateElementalArt_Feiyan::Enter(CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	return PLAYER_ELEMENTAL_START;
}

PLAYER_STATE CStateElementalArt_Feiyan::Update(const _float& fTimeDelta, CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	return CurrentState;
}

PLAYER_STATE CStateElementalArt_Feiyan::Exit(CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	if (CurrentState == PLAYER_ELEMENTAL_START)
		return PLAYER_ELEMENTAL_END;

	return pStateManager.Set_CurrentState(CStateManager::STATE_TYPE_IDEL);
}

CStateElementalArt_Feiyan* CStateElementalArt_Feiyan::Create()
{
	CStateElementalArt_Feiyan* pInstance = new CStateElementalArt_Feiyan();

	if (FAILED(pInstance->Initialize()))
		Safe_Release(pInstance);

	return pInstance;
}

void CStateElementalArt_Feiyan::Free()
{
	__super::Free();
}
