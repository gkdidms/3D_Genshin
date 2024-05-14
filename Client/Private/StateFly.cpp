#include "StateFly.h"

#include "StateManager.h"
#include "GameInstance.h"

CStateFly::CStateFly()
	: CState{}
{
}

PLAYER_STATE CStateFly::Enter(class CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	return PLAYER_FLY_START;
}

PLAYER_STATE CStateFly::Update(const _float& fTimeDelta, CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	if (CurrentState == PLAYER_FLY_START || CurrentState == PLAYER_FALL_GROUND_L)
		return CurrentState;

	if (m_pGameInstance->GetKeyState(DIK_SPACE) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CStateManager::STATE_TYPE_FALL_ATTACK);

	if (m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::HOLD)
		return PLAYER_FLY_TURN_L;

	if (m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::HOLD)
		return PLAYER_FLY_TURN_R;
		
	return CurrentState;
}

PLAYER_STATE CStateFly::Exit(CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	if (CurrentState == PLAYER_FLY_START)
		return PLAYER_FLY_NORMAL;

	if (CurrentState == PLAYER_FALL_GROUND_L)
		return __super::ToIdle(pStateManager, CurrentState);

	return PLAYER_FALL_GROUND_L;
}

CStateFly* CStateFly::Create()
{
	return new CStateFly();
}

void CStateFly::Free()
{
	__super::Free();
}
