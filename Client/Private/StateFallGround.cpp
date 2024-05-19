#include "StateFallGround.h"

#include "StateManager.h"
#include "GameInstance.h"

CStateFallGround::CStateFallGround()
	: CState{}
{
}

PLAYER_STATE CStateFallGround::Enter(class CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	if (Check_Move())
		return PLAYER_FALL_GROUND_FOR_RUN;

	m_fDuration = 1.f;

	return PLAYER_FALL_GROUND_H;
}

PLAYER_STATE CStateFallGround::Update(const _float& fTimeDelta, CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	if (m_fDuration < m_fTime)
	{
		if (CurrentState == PLAYER_FALL_GROUND_H || CurrentState == PLAYER_FALL_GROUND_L)
		{
			if (Check_Move())
				return PLAYER_FALL_GROUND_FOR_RUN;

			if (m_pGameInstance->GetKeyState(DIK_LSHIFT))
				return PLAYER_FALL_GROUND_FOR_SPRINT;
		}
	}
	else
		m_fTime += fTimeDelta;

	return CurrentState;
}

PLAYER_STATE CStateFallGround::Exit(CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	if (CurrentState == PLAYER_FALL_GROUND_FOR_RUN)
		return pStateManager.Set_CurrentState(CStateManager::STATE_TYPE_RUN, CurrentState);

	else if (CurrentState == PLAYER_FALL_GROUND_FOR_SPRINT)
		return pStateManager.Set_CurrentState(CStateManager::STATE_TYPE_SPRINT, CurrentState);

	else if (CurrentState == PLAYER_FALL_GROUND_H)
		return pStateManager.Set_CurrentState(CStateManager::STATE_TYPE_IDEL);

	return PLAYER_FALL_GROUND_FOR_RUN;
}

CStateFallGround* CStateFallGround::Create()
{
	return new CStateFallGround;
}

void CStateFallGround::Free()
{
	__super::Free();
}
