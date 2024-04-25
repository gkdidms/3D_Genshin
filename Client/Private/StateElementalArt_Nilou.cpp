#include "StateElementalArt_Nilou.h"

#include "GameInstance.h"
#include "State_Manager.h"

CStateElementalArt_Nilou::CStateElementalArt_Nilou()
{
}

PLAYER_STATE CStateElementalArt_Nilou::Enter(PLAYER_STATE CurrentState)
{
	return PLAYER_ELEMENTAL_1;
}

PLAYER_STATE CStateElementalArt_Nilou::Update(const _float& fTimeDelta, CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	m_fTime += fTimeDelta;
	m_fCurrentTime += fTimeDelta;

	if (m_fCurrentTime > m_fFinishTime)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_IDEL);

	if (m_fTime < m_fAttackTime)
		return CurrentState;

	if (CurrentState == PLAYER_ELEMENTAL_1_END ||
		CurrentState == PLAYER_ELEMENTAL_2_END ||
		CurrentState == PLAYER_ELEMENTAL_3_END ||
		CurrentState == PLAYER_ATTACK_1 || 
		CurrentState == PLAYER_ATTACK_2 || 
		CurrentState == PLAYER_ATTACK_3)
	{
		if (m_iSkillCount == m_iMaxSkill)
		{
			m_iAttackCount = 0;
			m_iElementalArtCount = 0;
		}

		PLAYER_STATE State = { PLAYER_END };
		if (m_pGameInstance->GetMouseState(DIM_LB) == CInput_Device::TAP)
		{
			m_fTime = 0.f;

			if (m_iAttackCount == 0)
				State = PLAYER_ATTACK_1;
			else if (m_iAttackCount == 1)
				State = PLAYER_ATTACK_2;
			else if (m_iAttackCount == 2)
				State = PLAYER_ATTACK_3;

			if (m_iSkillCount == m_iMaxSkill - 1) //기도의 달
				State = PALYER_ATTACK_SPEC;

			m_iAttackCount++;
		}
		else if (m_pGameInstance->GetKeyState(DIK_E) == CInput_Device::TAP)
		{
			m_fTime = 0.f;

			if (m_iElementalArtCount == 0)
				State = PLAYER_ELEMENTAL_2;
			else if (m_iElementalArtCount == 1)
				State = PLAYER_ELEMENTAL_3;

			if (m_iSkillCount == m_iMaxSkill - 1) // 수륜피해
				State = PLAYER_ELEMENTAL_SPEC;

			m_iElementalArtCount++;
		}
		m_iSkillCount++;
	}

	return CurrentState;
}

PLAYER_STATE CStateElementalArt_Nilou::Exit(CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	if (CurrentState == PLAYER_ELEMENTAL_1)
		return PLAYER_ELEMENTAL_1_END;
	if (CurrentState == PLAYER_ELEMENTAL_2)
		return PLAYER_ELEMENTAL_2_END;
	if (CurrentState == PLAYER_ELEMENTAL_3)
		return PLAYER_ELEMENTAL_3_END;

	return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_IDEL);
}

CStateElementalArt_Nilou* CStateElementalArt_Nilou::Create()
{
	return new CStateElementalArt_Nilou();
}

void CStateElementalArt_Nilou::Free()
{
	__super::Free();
}
