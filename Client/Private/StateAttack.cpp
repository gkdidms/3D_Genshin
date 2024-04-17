#include "StateAttack.h"

#include "GameInstance.h"
#include "State_Manager.h"

CStateAttack::CStateAttack()
{
}

HRESULT CStateAttack::Initialize(_uint iMaxAttack)
{
	m_iMaxAttack = iMaxAttack;

	return S_OK;
}

PLAYER_STATE CStateAttack::Enter(PLAYER_STATE CurrentState)
{
	return PLAYER_ATTACK_1;
}

PLAYER_STATE CStateAttack::Update(const _float& fTimeDelta, CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	m_fTime += fTimeDelta;

	if (m_fTime < m_fAttackTime)
		return CurrentState;

	if (m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_S) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::HOLD)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_RUN);

	if (PLAYER_IDLE_PUT_AWAY == CurrentState) // attack -> idle 사이값
		return CurrentState;

	if (m_pGameInstance->GetKeyState(DIK_E) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_ELEMENTALART);

	if (m_pGameInstance->GetKeyState(DIK_Q) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_ELEMNETALBURST);

	if (m_pGameInstance->GetMouseState(DIM_LB) == CInput_Device::TAP)
	{
		m_fTime = 0; // 초기화
		m_iAttackCount++;

		if (m_iAttackCount >= m_iMaxAttack)
			m_iAttackCount = 0;

		if (m_iAttackCount == 0)
			return PLAYER_ATTACK_1;
		else if (m_iAttackCount == 1)
			return PLAYER_ATTACK_2;
		else if (m_iAttackCount == 2)
			return PLAYER_ATTACK_3;
		else if (m_iAttackCount == 3)
			return PLAYER_ATTACK_4;
	}

	return CurrentState;
}

PLAYER_STATE CStateAttack::Exit(CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	if (PLAYER_IDLE_PUT_AWAY == CurrentState)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_IDEL);

	return PLAYER_IDLE_PUT_AWAY;
}

CStateAttack* CStateAttack::Create(_uint iMaxAttack)
{
	CStateAttack* pInstance = new CStateAttack();

	if (FAILED(pInstance->Initialize(iMaxAttack)))
		Safe_Release(pInstance);

	return pInstance;
}

void CStateAttack::Free()
{
	__super::Free();
}
