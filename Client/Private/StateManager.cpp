#include "StateManager.h"

#include "GameInstance.h"

#include "StateRun.h"
#include "StateSprint.h"
#include "StateIdle.h"
#include "StateElementalArt_Tighanri.h"
#include "StateElementalArt_Nilou.h"
#include "StateElementalArt_Wanderer.h"
#include "StateElementalArt_Yae.h"
#include "StateElementalArt_Feiyan.h"

#include "StateExtraAttack.h"
#include "StateElementalBurst.h"
#include "StateAttack.h"
#include "StateJump.h"
#include "StateFly.h"
#include "StateFallAttack.h"
#include "StateFallGround.h"
#include "StateHit.h"


IMPLEMENT_SINGLETON(CStateManager);

CStateManager::CStateManager()
	:m_pGameInstance { CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

PLAYER_STATE CStateManager::Set_CurrentState(STATE_TYPE Type, PLAYER_STATE CurrentState)
{
	if (Type == m_StateType) return CurrentState;

	Safe_Release(m_pCurrentState);

	switch (Type)
	{
	case Client::CStateManager::STATE_TYPE_IDEL:
		m_pCurrentState = CStateIdle::Create();
		break;
	case Client::CStateManager::STATE_TYPE_ATTACK:
		m_pCurrentState = CStateAttack::Create();
		break;
	case Client::CStateManager::STATE_TYPE_EXTRA_ATTACK:
		if (*m_CurrentPlayerble == CPlayer::PLAYER_FEIYAN)
			m_pCurrentState = CStateExtraAttack::Create();
		else
			m_pCurrentState = CStateAttack::Create();
		break;
	case Client::CStateManager::STATE_TYPE_ELEMENTALART:
		if (*m_CurrentPlayerble == CPlayer::PLAYER_TIGHNARI)
			m_pCurrentState = CStateElementalArt_Tighanri::Create();
		else if (*m_CurrentPlayerble == CPlayer::PLAYER_NILOU)
			m_pCurrentState = CStateElementalArt_Nilou::Create();
		else if (*m_CurrentPlayerble == CPlayer::PLAYER_WANDERER)
			m_pCurrentState = CStateElementalArt_Wanderer::Create();
		else if (*m_CurrentPlayerble == CPlayer::PLAYER_FEIYAN)
			m_pCurrentState = CStateElementalArt_Feiyan::Create();
		break;
	case Client::CStateManager::STATE_TYPE_ELEMNETALBURST:
		m_pCurrentState = CStateElementalBurst::Create();
		break;
	case Client::CStateManager::STATE_TYPE_RUN:
		m_pCurrentState = CStateRun::Create();
		break;
	case Client::CStateManager::STATE_TYPE_SPRINT:
		m_pCurrentState = CStateSprint::Create();
		break;
	case Client::CStateManager::STATE_TYPE_JUMP:
		m_pCurrentState = CStateJump::Create();
		break;
	case Client::CStateManager::STATE_TYPE_FLY:
		m_pCurrentState = CStateFly::Create();
		break;
	case Client::CStateManager::STATE_TYPE_FALL_ATTACK:
		m_pCurrentState = CStateFallAttack::Create();
		break;
	case Client::CStateManager::STATE_TYPE_FALL_GROUND:
		m_pCurrentState = CStateFallGround::Create();
		break;
	case STATE_TYPE_HIT:
		m_pCurrentState = CStateHit::Create();
		break;
	case Client::CStateManager::STATE_TYPE_END:
		break;
	default:
		break;
	}

	m_StateType = Type;

	return m_pCurrentState->Enter(*this, CurrentState);
}

HRESULT CStateManager::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	PLAYER_STATE_DESC* pDesc = static_cast<PLAYER_STATE_DESC*>(pArg);
	m_CurrentPlayerble = pDesc->pPlayerbleType;
	m_CurrentDir = pDesc->pPlayerDir;

	return S_OK;
}

void CStateManager::Free()
{
	Safe_Release(m_pCurrentState);

	Safe_Release(m_pGameInstance);
}
