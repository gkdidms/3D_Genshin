#include "BT_EvilEye.h"

#include "GameInstance.h"
#include "Boss.h"
#include "Weapon.h"

#include "Sequence.h"
#include "Selector.h"
#include "Action.h"


CBT_EvilEye::CBT_EvilEye()
	: CBT_Boss{}
{
}

HRESULT CBT_EvilEye::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Node()))
		return E_FAIL;

	m_fDelayTime = { 2.f };

	return S_OK;
}

void CBT_EvilEye::Tick(const _float& fTimeDelta)
{
	if (!m_isAttack)
		m_fCurrentTime += fTimeDelta;
	else if (m_isAttack && m_Skill == SKILL_BLADE_NORMAL)
		m_fCurrentRunTime += fTimeDelta;

	this->Evaluate();
}

CNode::NODE_STATE CBT_EvilEye::Evaluate()
{
	return m_pRootNode->Evaluate();
}

HRESULT CBT_EvilEye::Ready_Node()
{
	//Death
	CSequence* pDeath = CSequence::Create();
	pDeath->Add_Children(CAction::Create(bind(&CBT_EvilEye::Death, this)));

	//Hit
	CSequence* pHit = CSequence::Create();
	pHit->Add_Children(CAction::Create(bind(&CBT_EvilEye::Check_Hit, this)));
	pHit->Add_Children(CAction::Create(bind(&CBT_EvilEye::Check_Intersect, this)));

	CSelector* pHitSelect = CSelector::Create();
	//디펜스 -> 히트
	CSequence* pDefend = CSequence::Create();
	pDefend->Add_Children(CAction::Create(bind(&CBT_EvilEye::Check_Defend, this)));

	CSelector* pDefendSelect = CSelector::Create();
	pDefendSelect->Add_Children(CAction::Create(bind(&CBT_EvilEye::Defend, this)));
	pDefendSelect->Add_Children(CAction::Create(bind(&CBT_EvilEye::Attack_Defend, this)));

	pDefend->Add_Children(pDefendSelect);
	pHitSelect->Add_Children(pDefend);
	pHitSelect->Add_Children(CAction::Create(bind(&CBT_EvilEye::Hit, this)));

	pHit->Add_Children(pHitSelect);

	//Attack
	CSequence* pAttack = CSequence::Create();
	//pAttack->Add_Children(CAction::Create(bind(&CBT_Tartaglia::Check_Attack, this)));
	pAttack->Add_Children(CAction::Create(bind(&CBT_EvilEye::Check_Attack_Deley, this)));
	pAttack->Add_Children(CAction::Create(bind(&CBT_EvilEye::Check_Attack_Range, this)));
	pAttack->Add_Children(CAction::Create(bind(&CBT_EvilEye::Check_Melee_Attack, this)));
	pAttack->Add_Children(CAction::Create(bind(&CBT_EvilEye::Check_Range_Attack, this)));

	CSelector* pAttackSelect = CSelector::Create();
	pAttackSelect->Add_Children(CAction::Create(bind(&CBT_EvilEye::Rush_Move, this)));
	pAttackSelect->Add_Children(CAction::Create(bind(&CBT_EvilEye::DualBlade_Sweep, this)));
	pAttackSelect->Add_Children(CAction::Create(bind(&CBT_EvilEye::DualBlade_Hiraishin, this)));
	pAttackSelect->Add_Children(CAction::Create(bind(&CBT_EvilEye::DualBlade_Strike, this)));
	pAttackSelect->Add_Children(CAction::Create(bind(&CBT_EvilEye::Blade_Extra_Attack, this)));
	pAttackSelect->Add_Children(CAction::Create(bind(&CBT_EvilEye::Blade_Normal_Attack, this)));

	pAttack->Add_Children(pAttackSelect);

	CSequence* pWalk = CSequence::Create();
	pWalk->Add_Children(CAction::Create(bind(&CBT_EvilEye::Check_Walk, this)));
	pWalk->Add_Children(CAction::Create(bind(&CBT_EvilEye::Walk, this)));

	CSequence* pUndo = CSequence::Create();
	pUndo->Add_Children(CAction::Create(bind(&CBT_EvilEye::Standby, this)));

	CSelector* pRootSelector = CSelector::Create();
	pRootSelector->Add_Children(pDeath);
	pRootSelector->Add_Children(pHit);
	pRootSelector->Add_Children(pAttack);
	pRootSelector->Add_Children(pWalk);
	pRootSelector->Add_Children(pUndo);

	m_pRootNode = pRootSelector;

	return S_OK;
}

CNode::NODE_STATE CBT_EvilEye::Death()
{
	if (m_pInfo->fHp <= 0)
	{
		*m_pState = CBoss::BOSS_DIE;
		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_EvilEye::Check_Hit()
{
	if (m_isAttack) return CNode::FAILURE;

	if (*m_pState != CBoss::BOSS_HIT_H
		&& *m_pState != CBoss::BOSS_HIT_L
		&& *m_pState != CBoss::BOSS_DEFEND_1
		&& *m_pState != CBoss::BOSS_DEFEND_2
		&& *m_pState != CBoss::BOSS_DEFEND_3
		&& *m_pState != CBoss::BOSS_DEFEND_ATTACK_1
		&& *m_pState != CBoss::BOSS_DEFEND_ATTACK_2)
		return CNode::SUCCESS;

	if (m_pModelCom->Get_Animation_Finished())
		return CNode::SUCCESS;

	return CNode::RUNNING;
}

CNode::NODE_STATE CBT_EvilEye::Check_Intersect()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(LEVEL_STAGE_BOSS, L"Layer_Player", 0));

	CCollider* pWeaponCollider = { nullptr };

	if (pPlayer->Get_CurrentWeapon() == CWeapon::WEAPON_SWORD)
	{
		pWeaponCollider = pPlayer->Get_SwordCollider();
	}
	else
	{
		// 화살 + 마법 이펙트들은 따로 사용 (임시용)
		pWeaponCollider = dynamic_cast<CCollider*>(m_pGameInstance->Get_GameObject_Component(LEVEL_STAGE_BOSS, L"Layer_Player", L"Com_Collider"));
	}

	if (m_pColliderCom->Intersect(pWeaponCollider))
	{
		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_EvilEye::Hit()
{
	*m_pState = CBoss::BOSS_HIT_H;

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_EvilEye::Check_Defend()
{
	//일정 확률로 나옴
	_uint iProbability = 70;
	_uint RandomNumber = Random(100);

	//이미 방어하고 있는 상태가 아니라면 30% 확률로 hit
	if (!m_pInfo->isDefend && RandomNumber > 70)
		return CNode::FAILURE;

	// 플레이어가 일반공격을 햇는지 체크
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(LEVEL_STAGE_BOSS, L"Layer_Player", 0));
	_uint eWeaponType = pPlayer->Get_CurrentWeapon();

	if ((pPlayer->Get_PlayerState() == PLAYER_ATTACK_1
		|| pPlayer->Get_PlayerState() == PLAYER_ATTACK_2
		|| pPlayer->Get_PlayerState() == PLAYER_ATTACK_3
		|| pPlayer->Get_PlayerState() == PLAYER_ATTACK_4) && eWeaponType == CWeapon::WEAPON_SWORD)
	{
		iProbability = 10;

		RandomNumber = Random(100);

		// 90% 확률로 방어 / 10% 확률로 카운터
		m_pInfo->isDefend = RandomNumber <= iProbability ? true : false;

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_EvilEye::Defend()
{
	if (!m_pInfo->isDefend) return CNode::FAILURE;

	//방어막
	*m_pState = CBoss::BOSS_DEFEND_1;

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_EvilEye::Attack_Defend()
{
	// 확률적으로 카운터
	*m_pState = CBoss::BOSS_DEFEND_ATTACK_1;

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_EvilEye::Check_Attack_Deley()
{
	if (m_isFirst)
	{
		m_Skill = SKILL_DUALBLADE_STRIKE;
		m_isFirst = false;
		m_pTransformCom->LookAt(XMLoadFloat4x4(m_pTargetMatrix).r[3]);

		return CNode::SUCCESS;
	}
	else
	{
		if (!m_isAttack)
		{
			if (m_fDelayTime < m_fCurrentTime)
			{
				CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(LEVEL_STAGE_BOSS, L"Layer_Player", 0));

				if (Check_Rear_ToPlayer())
				{
					//몬스터가 플레이어 뒤에 있다면??
					m_Skill = SKILL_DUALBLADE_SWEEP;
				}
				else if (pPlayer->Get_BossSign())
				{
					//단류 표식이 되어있다면 단류가 있어야만 사용 가능한 스킬들 사용

				}
				else
				{
					m_Skill = SKILL(Random(SKILL_END));
				}

				m_fCurrentTime = 0.f;
				m_pTransformCom->LookAt(XMLoadFloat4x4(m_pTargetMatrix).r[3]);

				m_isWalkStop = false;

				return CNode::SUCCESS;
			}
			return CNode::FAILURE;
		}
	}

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_EvilEye::Check_Attack_Range()
{
	if (m_isAttack)
		return CNode::SUCCESS;

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(LEVEL_STAGE_BOSS, L"Layer_Player", 0));
	_float fDistance = Distance(XMLoadFloat4x4(m_pTargetMatrix), XMLoadFloat4x4(m_pTransformCom->Get_WorldFloat4x4()));

	if (fDistance < m_fMeleeAtkRange)
	{
		m_RangeType = MELEE;
		return CNode::SUCCESS;
	}
	else {
		m_RangeType = RANGE;
		return CNode::SUCCESS;
	}

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_EvilEye::Check_Melee_Attack()
{
	if (m_isAttack)
		return CNode::SUCCESS;

	if (m_RangeType == RANGE)
		return CNode::SUCCESS;

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(LEVEL_STAGE_BOSS, L"Layer_Player", 0));

	//근거리 공격
	if (Check_Rear_ToPlayer())
	{
		//몬스터가 플레이어 뒤에 있다면??
		m_Skill = SKILL_DUALBLADE_SWEEP;
	}
	else if (pPlayer->Get_BossSign())
	{
		//단류 표식이 되어있다면 단류가 있어야만 사용 가능한 스킬들 사용
	}
	else
	{
		m_Skill = SKILL_BLADE_NORMAL;
	}

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_EvilEye::Check_Range_Attack()
{
	if (m_isAttack)
		return CNode::SUCCESS;

	if (m_RangeType == MELEE)
		return CNode::SUCCESS;

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(LEVEL_STAGE_BOSS, L"Layer_Player", 0));

	if (pPlayer->Get_BossSign())
	{
		//단류 표식이 되어있다면?
	}
	else
	{
		_uint iSkillNum = Random(4);

		switch (iSkillNum)
		{
		case 0:
			m_Skill = SKILL_RUSH;
			break;
		case 1:
			m_Skill = SKILL_BLADE_EXTRA;
			break;
		case 2:
			m_Skill = SKILL_DUALBLADE_STRIKE;
			break;
		case 3:
			m_Skill = SKILL_DUALBLADE_HIRAISHIN;
			break;
		default:
			break;
		}
	}

	return CNode::SUCCESS;
}

//몬스터가 플레이어 뒤에 있는지 확인
_bool CBT_EvilEye::Check_Rear_ToPlayer()
{
	_vector vTarget = XMLoadFloat4x4(m_pTargetMatrix).r[3];
	_vector vCurrentPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vLook = XMLoadFloat4x4(m_pTargetMatrix).r[2];

	return !AngleOfView(180.f, vCurrentPos, vTarget, vLook);
}

CNode::NODE_STATE CBT_EvilEye::Rush_Move()
{
	if (m_isAttack && m_Skill == SKILL_RUSH)
	{
		if (*m_pState == CBoss::BOSS_RUSH_BS && m_pModelCom->Get_Animation_Finished())
		{
			*m_pState = CBoss::BOSS_RUSH_AS;
		}
		else if (*m_pState == CBoss::BOSS_RUSH_AS && m_pModelCom->Get_Animation_Finished())
		{
			m_Skill = SKILL_END;
			m_isAttack = false;

			return CNode::SUCCESS;
		}

		return CNode::RUNNING;
	}

	//대쉬
	if (m_Skill == SKILL_RUSH)
	{
		m_pTransformCom->LookAt(XMLoadFloat4x4(m_pTargetMatrix).r[3]);

		*m_pState = CBoss::BOSS_RUSH_BS;

		m_isAttack = true;
		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_EvilEye::Blade_Extra_Attack()
{
	if (m_isAttack && m_Skill == SKILL_BLADE_EXTRA)
	{
		if (*m_pState == CBoss::BOSS_BLADE_EXTRA_ATTACK && m_pModelCom->Get_Animation_Finished(5))
		{
			m_isAttack = false;
			return CNode::SUCCESS;
		}

		return CNode::RUNNING;
	}

	// 플레이어와 멀리 떨어져있는 경우 가깝게 이동하여 공격함
	if (m_Skill == SKILL_BLADE_EXTRA)
	{
		*m_pState = CBoss::BOSS_BLADE_EXTRA_ATTACK;
		m_isAttack = true;

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_EvilEye::Blade_Normal_Attack() // 끊어서 이어줘야 함.
{
	if (m_isAttack && m_Skill == SKILL_BLADE_NORMAL)
	{
		if (m_fNormalTime < m_fCurrentRunTime)
		{
			++m_iBladeNormalAttackMaxCount;

			if (m_iBladeNormalAttackMax > m_iBladeNormalAttackMaxCount)
			{
				if (*m_pState == CBoss::BOSS_BLADE_NORMAL_ATTACK_1)
					*m_pState = CBoss::BOSS_BLADE_NORMAL_ATTACK_2;
				else if (*m_pState == CBoss::BOSS_BLADE_NORMAL_ATTACK_2)
					*m_pState = CBoss::BOSS_BLADE_NORMAL_ATTACK_3;
				else if (*m_pState == CBoss::BOSS_BLADE_NORMAL_ATTACK_3)
					*m_pState = CBoss::BOSS_BLADE_NORMAL_ATTACK_4;
				else if (*m_pState == CBoss::BOSS_BLADE_NORMAL_ATTACK_4)
					*m_pState = CBoss::BOSS_BLADE_NORMAL_ATTACK_1;

				return CNode::SUCCESS;
			}

			if (m_pModelCom->Get_Animation_Finished())
			{
				m_isAttack = false;
				m_iBladeNormalAttackMaxCount = 0;
				m_fCurrentRunTime = 0.f;

				return CNode::SUCCESS;
			}
		}
		return CNode::RUNNING;
	}

	if (m_Skill == SKILL_BLADE_NORMAL)
	{
		m_iBladeNormalAttackMax = Random(4);
		*m_pState = CBoss::BOSS_BLADE_NORMAL_ATTACK_1;

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_EvilEye::DualBlade_Strike() // 돌진 찌르기 3~5
{
	if (m_isAttack && m_Skill == SKILL_DUALBLADE_STRIKE)
	{
		if (*m_pState == CBoss::BOSS_DUALBLADE_STRIKE_ATTACK_BS && m_pModelCom->Get_Animation_Finished())
			*m_pState = CBoss::BOSS_DUALBLADE_STRIKE_ATTACK_LOOP;
		else if (*m_pState == CBoss::BOSS_DUALBLADE_STRIKE_ATTACK_LOOP && m_pModelCom->Get_LoopAnimation_Finished())
		{
			m_pTransformCom->LookAt(XMLoadFloat4x4(m_pTargetMatrix).r[3]);
			++m_iCurrentAttackCount;
			
			if (m_iAttackCount <= m_iCurrentAttackCount)
			{
				*m_pState = CBoss::BOSS_DUALBLADE_STRIKE_ATTACK_AS;
			}
		}
		else if (*m_pState == CBoss::BOSS_DUALBLADE_STRIKE_ATTACK_AS && m_pModelCom->Get_Animation_Finished())
		{
			m_iCurrentAttackCount = 0; // 초기화
			m_isAttack = false;
			return CNode::SUCCESS;
		}

		return CNode::RUNNING;
	}

	if (m_Skill == SKILL_DUALBLADE_STRIKE)
	{
		m_iAttackCount = Random(2) + 3;
		m_isAttack = true;
		*m_pState = CBoss::BOSS_DUALBLADE_STRIKE_ATTACK_BS;

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_EvilEye::DualBlade_Sweep() // 후방 치기 -> 타탈이 뒤에 존재하면 높은 확률로 나옴
{
	if (m_isAttack && m_Skill == SKILL_DUALBLADE_SWEEP)
	{
		if ((*m_pState == CBoss::BOSS_DUALBLADE_SWEEP_ATTACK_L || *m_pState == CBoss::BOSS_DUALBLADE_SWEEP_ATTACK_R) && m_pModelCom->Get_Animation_Finished())
		{
			m_isAttack = false;
			return CNode::SUCCESS;
		}

		return CNode::RUNNING;
	}

	if (m_Skill == SKILL_DUALBLADE_SWEEP)
	{
		// 플레이어 방향에 따라서 달라짐 
		m_pTransformCom->LookAt(XMLoadFloat4x4(m_pTargetMatrix).r[3]);

		_vector vTargetPos = XMLoadFloat4x4(m_pTargetMatrix).r[3];
		_vector vHiliPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_vector vHiliLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

		_vector vDesc = XMVector3Normalize(vTargetPos - vHiliPos);
		_float fDot = XMVectorGetX(XMVector3Dot(vDesc, vHiliLook));

		if (fDot > 0)
			*m_pState = CBoss::BOSS_DUALBLADE_SWEEP_ATTACK_R;
		else
			*m_pState = CBoss::BOSS_DUALBLADE_SWEEP_ATTACK_L;

		m_isAttack = true;

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_EvilEye::DualBlade_Hiraishin() // 단류 포식이 있을 경우에만 사용 -> 번개창 낙하
{
	if (m_isAttack && m_Skill == SKILL_DUALBLADE_HIRAISHIN)
	{
		if (*m_pState == CBoss::BOSS_DUALBLADE_HIRAISHIN_BS && m_pModelCom->Get_Animation_Finished())
			*m_pState = CBoss::BOSS_DUALBLADE_HIRAISHIN_LOOP;
		else if (*m_pState == CBoss::BOSS_DUALBLADE_HIRAISHIN_LOOP && m_pModelCom->Get_LoopAnimation_Finished())
			*m_pState = CBoss::BOSS_DUALBLADE_HIRAISHIN_AS;
		else if (*m_pState == CBoss::BOSS_DUALBLADE_HIRAISHIN_AS && m_pModelCom->Get_Animation_Finished())
		{
			m_isAttack = false;
			return CNode::SUCCESS;
		}
		return CNode::RUNNING;
	}

	if (m_Skill == SKILL_DUALBLADE_HIRAISHIN)
	{
		m_isAttack = true;
		*m_pState = CBoss::BOSS_DUALBLADE_HIRAISHIN_BS;

		return CNode::SUCCESS;
	}
	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_EvilEye::Check_Walk()
{
	if (m_isWalkStop) return CNode::FAILURE;

	if (*m_pState != CBoss::BOSS_WALK_L
		&& *m_pState != CBoss::BOSS_WALK_R
		&& *m_pState != CBoss::BOSS_WALK_L_TO_R
		&& *m_pState != CBoss::BOSS_WALK_R_To_L)
		return CNode::SUCCESS;

	if (m_pModelCom->Get_Animation_Finished())
		return CNode::SUCCESS;

	return CNode::RUNNING;
}

CNode::NODE_STATE CBT_EvilEye::Walk()
{
	if (*m_pState == CBoss::BOSS_WALK_L && m_pModelCom->Get_Animation_Finished())
		*m_pState = CBoss::BOSS_WALK_L_TO_R;
	else if (*m_pState == CBoss::BOSS_WALK_R && m_pModelCom->Get_Animation_Finished())
		*m_pState = CBoss::BOSS_WALK_R_To_L;
	else if (*m_pState == CBoss::BOSS_WALK_L_TO_R && m_pModelCom->Get_Animation_Finished())
		*m_pState = CBoss::BOSS_WALK_R;
	else if (*m_pState == CBoss::BOSS_WALK_R_To_L && m_pModelCom->Get_Animation_Finished())
		*m_pState = CBoss::BOSS_WALK_L;

	if (*m_pState != CBoss::BOSS_WALK_L
		&& *m_pState != CBoss::BOSS_WALK_R
		&& *m_pState != CBoss::BOSS_WALK_L_TO_R
		&& *m_pState != CBoss::BOSS_WALK_R_To_L)
	{
		m_pTransformCom->LookAt(XMLoadFloat4x4(m_pTargetMatrix).r[3]);

		_vector vTargetPos = XMLoadFloat4x4(m_pTargetMatrix).r[3];
		_vector vHiliPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_vector vHiliLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

		_vector vDesc = XMVector3Normalize(vTargetPos - vHiliPos);
		_float fDot = XMVectorGetX(XMVector3Dot(vDesc, vHiliLook));

		if (fDot > 0)
			*m_pState = CBoss::BOSS_WALK_R;
		else if (fDot == 0)
			return CNode::FAILURE;
		else if (fDot < 0)
			*m_pState = CBoss::BOSS_WALK_L;
	}

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_EvilEye::Standby()
{
	*m_pState = CBoss::BOSS_IDLE;

	return CNode::SUCCESS;
}

CBT_EvilEye* CBT_EvilEye::Create(void* pArg)
{
	CBT_EvilEye* pInstance = new CBT_EvilEye();

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CBT_EvilEye::Free()
{
	__super::Free();
}
