#include "StateFallAttack.h"

#include "GameInstance.h"
#include "State_Manager.h"

CStateFallAttack::CStateFallAttack()
	: CState{ }
{
}

PLAYER_STATE CStateFallAttack::Enter(class CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	return PLAYER_FALL_ATTACK_LOOP;
}

PLAYER_STATE CStateFallAttack::Update(const _float& fTimeDelta, CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	if (CurrentState == PLAYER_FALL_ATTACK_LOOP && m_pGameInstance->GetKeyState(DIK_SPACE) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_FLY);

	return PLAYER_FALL_ATTACK_LOOP;
}

PLAYER_STATE CStateFallAttack::Exit(CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	return PLAYER_FALL_ATTACK_LOOP;
}

CStateFallAttack* CStateFallAttack::Create()
{
	return new CStateFallAttack();
}

void CStateFallAttack::Free()
{
	__super::Free();
}
