#include "StateAttack.h"

#include "GameInstance.h"
#include "StateManager.h"

CStateAttack::CStateAttack()
{
}

PLAYER_STATE CStateAttack::Enter(class CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	m_fTime = { 0.f };

	m_iMaxAttack = pStateManager.Get_Playerble() == CPlayer::PLAYER_NILOU || pStateManager.Get_Playerble() == CPlayer::PLAYER_TIGHNARI ? 4 : 3;

	return PLAYER_ATTACK_1;
}

PLAYER_STATE CStateAttack::Update(const _float& fTimeDelta, CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	m_fTime += fTimeDelta;

	if (m_fTime < m_fAttackTime)
		return CurrentState;

	if (m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_S) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::HOLD)
		return pStateManager.Set_CurrentState(CStateManager::STATE_TYPE_RUN);

	if (PLAYER_IDLE_PUT_AWAY == CurrentState) // attack -> idle 사이값
		return CurrentState;

	if (m_pGameInstance->GetKeyState(DIK_E) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CStateManager::STATE_TYPE_ELEMENTALART);

	if (m_pGameInstance->GetKeyState(DIK_Q) == CInput_Device::TAP)
		return pStateManager.Set_CurrentState(CStateManager::STATE_TYPE_ELEMNETALBURST);

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

PLAYER_STATE CStateAttack::Exit(CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	if (PLAYER_IDLE_PUT_AWAY == CurrentState)
		return pStateManager.Set_CurrentState(CStateManager::STATE_TYPE_IDEL);

	return PLAYER_IDLE_PUT_AWAY;
}

CStateAttack* CStateAttack::Create()
{
	return new CStateAttack();
}

void CStateAttack::Free()
{
	__super::Free();
}
