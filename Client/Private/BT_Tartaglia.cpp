#include "BT_Tartaglia.h"

#include "GameInstance.h"

CBT_Tartaglia::CBT_Tartaglia()
	: CNode{},
	m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CBT_Tartaglia::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	BT_TARTABGLIA_DESC* pDesc = static_cast<BT_TARTABGLIA_DESC*>(pArg);
	m_pInfo = pDesc->pInfo;
	m_pState = pDesc->pState;
	m_pModel = pDesc->pModel;
	m_pCollider = pDesc->pCollider;

	if (FAILED(Ready_Node()))
		return E_FAIL;

	return S_OK;
}

void CBT_Tartaglia::Tick(const _float& fTimeDelta)
{
	this->Evaluate();
}

CNode::NODE_STATE CBT_Tartaglia::Evaluate()
{
	return m_pRootNode->Evaluate();
}

HRESULT CBT_Tartaglia::Ready_Node()
{

	return S_OK;
}

_uint CBT_Tartaglia::Random(_uint iMaxNumber)
{
	srand(unsigned(time(NULL)));

	return rand() % iMaxNumber;
}

CNode::NODE_STATE CBT_Tartaglia::Death() // 죽음
{
	if (m_pInfo->fHp <= 0)
	{
		*m_pState = CBoss::BOSS_DIE;
		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Tartaglia::Check_Hit() // hit 모션 중인지?
{
	if (*m_pState != CBoss::BOSS_HIT_H && *m_pState != CBoss::BOSS_HIT_L)
		return CNode::FAILURE;

	if (m_pModel->Get_Animation_Finished(27) || m_pModel->Get_Animation_Finished(28) || m_pModel->Get_Animation_Finished(29) || m_pModel->Get_Animation_Finished(30))
		return CNode::SUCCESS;

	return CNode::RUNNING;
}

CNode::NODE_STATE CBT_Tartaglia::Hit() //충돌하면 hit_H
{
	CCollider* pPlayerCollider = dynamic_cast<CCollider*>(m_pGameInstance->Get_GameObject_Component(LEVEL_GAMEPLAY, L"Layer_Player", L"Com_Collider"));
	if (m_pCollider->Intersect(pPlayerCollider))
	{
		*m_pState = CBoss::BOSS_HIT_H;

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Tartaglia::Check_Defend() // 방어중인지 확인 
{
	if (*m_pState != CBoss::BOSS_DEFEND_1 && *m_pState != CBoss::BOSS_DEFEND_2 && *m_pState != CBoss::BOSS_DEFEND_3 && *m_pState != CBoss::BOSS_DEFEND_ATTACK_1 && *m_pState != CBoss::BOSS_DEFEND_ATTACK_2)
		return CNode::FAILURE;

	if (m_pModel->Get_Animation_Finished(0) || m_pModel->Get_Animation_Finished(1) || m_pModel->Get_Animation_Finished(2) || m_pModel->Get_Animation_Finished(3) || m_pModel->Get_Animation_Finished(4))
		return CNode::SUCCESS;

	return CNode::RUNNING;
}

CNode::NODE_STATE CBT_Tartaglia::Defend()
{
	//방어막
	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_Tartaglia::Attack_Defend()
{
	// 확률적으로 카운터
	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Tartaglia::Check_Attack()
{
	if (*m_pState == CBoss::BOSS_BOW_COVER_ATTACK_AS && m_pModel->Get_Animation_Finished(13))
	{
		// CoverAttack 공격이 끝낫다면
		m_isAttack = false;
		return CNode::FAILURE;
	}

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_Tartaglia::Check_Attack_Deley()
{
	//랜덤으로 스킬
	m_Skill = SKILL(Random(SKILL_END));

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_Tartaglia::Bow_ComboAttack()
{
	if (*m_pState == CBoss::BOSS_BOW_COMBO_ATTACK)
	{
		if (m_pModel->Get_Animation_Finished(12))
		{
			m_isAttack = false;
			return CNode::FAILURE;
		}
		else
			return CNode::RUNNING;
	}

	if (m_Skill == SKILL_BOW_COMBO)
	{
		*m_pState = CBoss::BOSS_BOW_COMBO_ATTACK;
		return CNode::SUCCESS;
	}
	
	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Tartaglia::Bow_CoverAttack()
{
	if (m_isAttack == true)
	{
		if (*m_pState == CBoss::BOSS_BOW_COVER_ATTACK_BS && m_pModel->Get_Animation_Finished(14))
			*m_pState = CBoss::BOSS_BOW_COVER_ATTACK_LOOP;
		else if (*m_pState == CBoss::BOSS_BOW_COVER_ATTACK_LOOP && m_pModel->Get_LoopAnimation_Finished())
			*m_pState = CBoss::BOSS_BOW_COVER_ATTACK_AS;

		return CNode::RUNNING;
	}

	if (m_Skill = SKILL_BOW_COVER)
	{
		*m_pState = CBoss::BOSS_BOW_COVER_ATTACK_BS;
		m_isAttack = true;

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Tartaglia::Bow_NormalAttack()
{
	if (m_isAttack == true)
	{
		if (*m_pState == CBoss::BOSS_BOW_NORMAL_ATTACK_BS && m_pModel->Get_Animation_Finished(17))
			*m_pState = CBoss::BOSS_BOW_NORMAL_ATTACK_LOOP;
		else if (*m_pState == CBoss::BOSS_BOW_NORMAL_ATTACK_LOOP && m_pModel->Get_LoopAnimation_Finished())
			*m_pState = CBoss::BOSS_BOW_NORMAL_ATTACK_AS;

		return CNode::RUNNING;
	}

	if (m_Skill == SKILL_BOW_NORMAL)
	{
		*m_pState = CBoss::BOSS_BOW_NORMAL_ATTACK_BS;
		m_isAttack = true;

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Tartaglia::Bow_PowerAttack()
{
	if (m_isAttack == true)
	{
		if (*m_pState == CBoss::BOSS_BOW_POWER_ATTACK_BS && m_pModel->Get_Animation_Finished(20))
			*m_pState = CBoss::BOSS_BOW_POWER_ATTACK_LOOP;
		else if (*m_pState == CBoss::BOSS_BOW_POWER_ATTACK_LOOP && m_pModel->Get_LoopAnimation_Finished())
			*m_pState = CBoss::BOSS_BOW_POWER_ATTACK_AS;

		return CNode::RUNNING;
	}
	
	if (m_Skill == SKILL_BOW_POWER)
	{
		*m_pState = CBoss::BOSS_BOW_NORMAL_ATTACK_BS;
		m_isAttack = true;

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Tartaglia::Bow_RangeAttack()
{
	if (m_isAttack == true)
	{
		if (*m_pState == CBoss::BOSS_BOW_RANGE_ATTACK_BS && m_pModel->Get_Animation_Finished(20))
			*m_pState = CBoss::BOSS_BOW_RANGE_ATTACK_LOOP_2;
		else if (*m_pState == CBoss::BOSS_BOW_RANGE_ATTACK_LOOP_2 && m_pModel->Get_LoopAnimation_Finished())
			*m_pState = CBoss::BOSS_BOW_RANGE_ATTACK_AS;

		return CNode::RUNNING;
	}

	if (m_Skill == SKILL_BOW_RENGE)
	{
		*m_pState = CBoss::BOSS_BOW_RANGE_ATTACK_BS;
		m_isAttack = true;

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Tartaglia::Blade_ExtraAttack() // 잘 안씀
{
	// 플레이어와 멀리 떨어져있는 경우 가깝게 이동하여 공격함
	if (m_Skill == SKILL_BLADE_EXTRA)
	{
		*m_pState = CBoss::BOSS_BLADE_EXTRA_ATTACK;
		m_isAttack = true;

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Tartaglia::Blade_NormalAttack() // 잘 안씀
{
	if (m_isAttack)
	{
		if (m_iBladeNormalAttackMax > m_iBladeNormalAttackMaxCount)
		{
			if (*m_pState == CBoss::BOSS_BLADE_NORMAL_ATTACK_1 && m_pModel->Get_Animation_Finished(6))
			{

				*m_pState = CBoss::BOSS_BLADE_NORMAL_ATTACK_2;
			}
			else if (*m_pState == CBoss::BOSS_BLADE_NORMAL_ATTACK_2 && m_pModel->Get_Animation_Finished(7))
			{
				*m_pState = CBoss::BOSS_BLADE_NORMAL_ATTACK_3;
			}
			else if (*m_pState == CBoss::BOSS_BLADE_NORMAL_ATTACK_3 && m_pModel->Get_Animation_Finished(8))
			{
				*m_pState = CBoss::BOSS_BLADE_NORMAL_ATTACK_4;
			}
			else if (*m_pState == CBoss::BOSS_BLADE_NORMAL_ATTACK_4 && m_pModel->Get_Animation_Finished(9))
				*m_pState = CBoss::BOSS_BLADE_NORMAL_ATTACK_1;

			m_iBladeNormalAttackMaxCount++;
		}
		
	}

	if (m_Skill == SKILL_BLADE_NORMAL)
	{
		m_iBladeNormalAttackMax = Random(6);
		*m_pState = CBoss::BOSS_BLADE_NORMAL_ATTACK_1;

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Tartaglia::Standby()
{
	*m_pState = CBoss::BOSS_BOW_IDLE;

	return CNode::SUCCESS;
}

CBT_Tartaglia* CBT_Tartaglia::Create(void* pArg)
{
	CBT_Tartaglia* pInstance = new CBT_Tartaglia();

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CBT_Tartaglia::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pRootNode);
}
