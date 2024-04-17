#include "StateElementalArt_Yae.h"

#include "GameInstance.h"
#include "State_Manager.h"

CStateElementalArt_Yae::CStateElementalArt_Yae()
{
}

PLAYER_STATE CStateElementalArt_Yae::Enter(PLAYER_STATE CurrentState)
{
	m_iSkillCount++;

	return PLAYER_ELEMENTAL_1;
}

PLAYER_STATE CStateElementalArt_Yae::Update(const _float& fTimeDelta, CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	m_fTime += fTimeDelta;

	if (m_fTime > 3.f)
		m_fTime = 0;
	else
		return CurrentState;
		
	if (m_pGameInstance->GetKeyState(DIK_W) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_A) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_S) == CInput_Device::HOLD
		|| m_pGameInstance->GetKeyState(DIK_D) == CInput_Device::HOLD
		&& (CurrentState == PLAYER_ELEMENTAL_1_END || CurrentState == PLAYER_ELEMENTAL_2_END))
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_RUN);

	if (m_pGameInstance->GetKeyState(DIK_E) == CInput_Device::TAP && (CurrentState == PLAYER_ELEMENTAL_1_END || CurrentState == PLAYER_ELEMENTAL_2_END))
	{
		PLAYER_STATE State = PLAYER_END;

		if (m_iMaxSkill < m_iSkillCount)
			m_iSkillCount = 0;

		if (m_iSkillCount == 0)
			State = PLAYER_ELEMENTAL_1;
		else if (m_iSkillCount == 1)
			State = PLAYER_ELEMENTAL_2;

		m_iSkillCount++;

		return State;
	}

	return CurrentState;
}

PLAYER_STATE CStateElementalArt_Yae::Exit(CState_Manager& pStateManager, PLAYER_STATE CurrentState)
{
	if (CurrentState == PLAYER_ELEMENTAL_1)
		return PLAYER_ELEMENTAL_1_END;

	if (CurrentState == PLAYER_ELEMENTAL_1_END)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_IDEL);

	if (CurrentState == PLAYER_ELEMENTAL_2)
		return PLAYER_ELEMENTAL_2_END;

	if (CurrentState == PLAYER_ELEMENTAL_2_END)
		return pStateManager.Set_CurrentState(CState_Manager::STATE_TYPE_IDEL);

	return PLAYER_ELEMENTAL_END;
}

CStateElementalArt_Yae* CStateElementalArt_Yae::Create()
{
	CStateElementalArt_Yae* pInstance = new CStateElementalArt_Yae();

	return pInstance;
}

void CStateElementalArt_Yae::Free()
{
	__super::Free();
}
