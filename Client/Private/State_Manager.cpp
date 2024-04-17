#include "State_Manager.h"

#include "GameInstance.h"

#include "StateRun.h"
#include "StateSprint.h"
#include "StateIdle.h"
#include "StateElementalBurst.h"
#include "StateAttack.h"
#include "StateElementalArt_Wanderer.h"
#include "StateElementalArt_Tighanri.h"
#include "StateElementalArt_Nilou.h"
#include "StateElementalArt_Yae.h"

IMPLEMENT_SINGLETON(CState_Manager)

CState_Manager::CState_Manager()
	:m_pGameInstance { CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

PLAYER_STATE CState_Manager::Set_CurrentState(STATE_TYPE Type, PLAYER_STATE CurrentState)
{
	if (m_pCurrentState != nullptr)
		Safe_Release(m_pCurrentState);

	switch (Type)
	{
	case Client::CState_Manager::STATE_TYPE_IDEL:
		m_pCurrentState = CStateIdle::Create();
		break;
	case Client::CState_Manager::STATE_TYPE_ATTACK:
	{
		_uint m_iMaxAttack = { 0 };
		if (m_CurrentPlayerble == CPlayer::PLAYER_TIGHNARI || m_CurrentPlayerble == CPlayer::PLAYER_NILOU)
			m_iMaxAttack = 4;
		else if (m_CurrentPlayerble == CPlayer::PLAYER_WANDERER || m_CurrentPlayerble == CPlayer::PLAYER_YAE)
			m_iMaxAttack = 4;

		m_pCurrentState = CStateAttack::Create(m_iMaxAttack);
		break;
	}
	case Client::CState_Manager::STATE_TYPE_ELEMENTALART:
	{
		if (m_CurrentPlayerble == CPlayer::PLAYER_WANDERER)
			m_pCurrentState = CStateElementalArt_Wanderer::Create();
		else if (m_CurrentPlayerble == CPlayer::PLAYER_TIGHNARI)
			m_pCurrentState = CStateElementalArt_Tighanri::Create();
		else if (m_CurrentPlayerble == CPlayer::PLAYER_YAE)
			m_pCurrentState = CStateElementalArt_Yae::Create();
		else if (m_CurrentPlayerble == CPlayer::PLAYER_NILOU)
			m_pCurrentState = CStateElementalArt_Nilou::Create();
		break;
	}
	case Client::CState_Manager::STATE_TYPE_ELEMNETALBURST:
		m_pCurrentState = CStateElementalBurst::Create();
		break;
	case Client::CState_Manager::STATE_TYPE_RUN:
		m_pCurrentState = CStateRun::Create();
		break;
	case Client::CState_Manager::STATE_TYPE_SPRINT:
		m_pCurrentState = CStateSprint::Create();
		break;
	case Client::CState_Manager::STATE_TYPE_END:
		break;
	default:
		break;
	}

	if (nullptr == m_pCurrentState)
		return PLAYER_END;

	return m_pCurrentState->Enter(CurrentState);
}

void CState_Manager::Set_CurrentPlayerble(CPlayer::PLAYERBLE_TYPE PlayerType)
{
	if (m_CurrentPlayerble == PlayerType)
		return;

	m_CurrentPlayerble = PlayerType; 

	Set_CurrentState(STATE_TYPE_IDEL);
}

void CState_Manager::Free()
{
	Safe_Release(m_pCurrentState);

	Safe_Release(m_pGameInstance);
}
