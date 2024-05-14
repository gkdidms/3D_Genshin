#include "StateElementalArt_Nilou.h"

#include "GameInstance.h"
#include "StateManager.h"

CStateElementalArt_Nilou::CStateElementalArt_Nilou()
{
}

PLAYER_STATE CStateElementalArt_Nilou::Enter(class CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	m_fTime = { 0.f };
	m_fCurrentTime = { 0.f };

	m_iElementalArtCount++;
	return PLAYER_ELEMENTAL_1;
}

PLAYER_STATE CStateElementalArt_Nilou::Update(const _float& fTimeDelta, CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	m_fTime += fTimeDelta;
	m_fCurrentTime += fTimeDelta;

	if (m_fCurrentTime > m_fFinishTime)
		m_isFinished = true;

	if (m_isFinished)
	{
		Reset();
		return __super::ToIdle(pStateManager, CurrentState);
	}

	if (CurrentState == PALYER_ATTACK_SPEC || CurrentState == PLAYER_ELEMENTAL_SPEC)
		return CurrentState;
		
	if (m_fTime < m_fAttackTime)
		return CurrentState;

	PLAYER_STATE eState{ CurrentState };
	if (m_pGameInstance->GetMouseState(DIM_LB) == CInput_Device::TAP) // 일반 공격
	{
		m_fTime = 0.f;

		m_iAttackCount++;
		m_iSkillCount++;

		if (m_iAttackCount == 0)
			eState = PLAYER_ATTACK_1;
		else if (m_iAttackCount == 1)
			eState = PLAYER_ATTACK_2;
		else if (m_iAttackCount == 2)
			eState = PLAYER_ATTACK_3;

		if (m_iSkillCount == m_iMaxSkill) //기도의 달
		{
			eState = PALYER_ATTACK_SPEC;
		}
	}
	else if (m_pGameInstance->GetKeyState(DIK_E) == CInput_Device::TAP) // E 스킬
	{
		m_fTime = 0.f;

		m_iElementalArtCount++;
		m_iSkillCount++;

		if (m_iElementalArtCount == 0)
			eState = PLAYER_ELEMENTAL_2;
		else if (m_iElementalArtCount == 1)
			eState = PLAYER_ELEMENTAL_3;

		else if (m_iSkillCount == m_iMaxSkill) // 수륜피해
		{
			eState = PLAYER_ELEMENTAL_SPEC;
		}
	}

	
	return eState;
}

PLAYER_STATE CStateElementalArt_Nilou::Exit(CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	if (CurrentState == PLAYER_ELEMENTAL_1)
		return PLAYER_ELEMENTAL_1_END;
	if (CurrentState == PLAYER_ELEMENTAL_2)
		return PLAYER_ELEMENTAL_2_END;
	if (CurrentState == PLAYER_ELEMENTAL_3)
		return PLAYER_ELEMENTAL_3_END;

	if (CurrentState == PALYER_ATTACK_SPEC ||
		CurrentState == PLAYER_ELEMENTAL_SPEC)
		m_isFinished = true;

	return PLAYER_IDLE;
}

void CStateElementalArt_Nilou::Reset()
{
	m_iSkillCount = 0;
	m_iAttackCount = 0;
	m_iElementalArtCount = 0;

	m_isFinished = false;
}

CStateElementalArt_Nilou* CStateElementalArt_Nilou::Create()
{
	return new CStateElementalArt_Nilou();
}

void CStateElementalArt_Nilou::Free()
{
	__super::Free();
}
