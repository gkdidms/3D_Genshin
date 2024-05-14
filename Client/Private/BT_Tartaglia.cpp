#include "BT_Tartaglia.h"

#include "GameInstance.h"
#include "Player.h"

#include "Sequence.h"
#include "Selector.h"
#include "Action.h"

#include "Weapon.h"

CBT_Tartaglia::CBT_Tartaglia()
	: CBT_Boss{}
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CBT_Tartaglia::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Node()))
		return E_FAIL;

	m_fDelayTime = 3.f;

	return S_OK;
}

void CBT_Tartaglia::Tick(const _float& fTimeDelta)
{
	if (!m_isAttack)
		m_fCurrentTime += fTimeDelta;

	this->Evaluate();
}

CNode::NODE_STATE CBT_Tartaglia::Evaluate()
{
	return m_pRootNode->Evaluate();
}

HRESULT CBT_Tartaglia::Ready_Node()
{
	//Death
	CSequence* pDeath = CSequence::Create();
	pDeath->Add_Children(CAction::Create(bind(&CBT_Tartaglia::Death, this)));

	//Hit
	CSequence* pHit = CSequence::Create();
	pHit->Add_Children(CAction::Create(bind(&CBT_Tartaglia::Check_Hit, this)));
	pHit->Add_Children(CAction::Create(bind(&CBT_Tartaglia::Check_Intersect, this)));

	CSelector* pHitSelect = CSelector::Create();
	//���潺 -> ��Ʈ
	CSequence* pDefend = CSequence::Create();
	pDefend->Add_Children(CAction::Create(bind(&CBT_Tartaglia::Check_Defend, this)));

	CSelector* pDefendSelect = CSelector::Create();
	pDefendSelect->Add_Children(CAction::Create(bind(&CBT_Tartaglia::Defend, this)));
	pDefendSelect->Add_Children(CAction::Create(bind(&CBT_Tartaglia::Attack_Defend, this)));

	pDefend->Add_Children(pDefendSelect);
	pHitSelect->Add_Children(pDefend);
	pHitSelect->Add_Children(CAction::Create(bind(&CBT_Tartaglia::Hit, this)));

	pHit->Add_Children(pHitSelect);

	//Attack
	CSequence* pAttack = CSequence::Create();
	//pAttack->Add_Children(CAction::Create(bind(&CBT_Tartaglia::Check_Attack, this)));
	pAttack->Add_Children(CAction::Create(bind(&CBT_Tartaglia::Check_Attack_Deley, this)));

	CSelector* pAttackSelect = CSelector::Create();
	pAttackSelect->Add_Children(CAction::Create(bind(&CBT_Tartaglia::Rush_Move, this)));
	pAttackSelect->Add_Children(CAction::Create(bind(&CBT_Tartaglia::Bow_ComboAttack, this)));
	pAttackSelect->Add_Children(CAction::Create(bind(&CBT_Tartaglia::Bow_CoverAttack, this)));
	pAttackSelect->Add_Children(CAction::Create(bind(&CBT_Tartaglia::Bow_NormalAttack, this)));
	pAttackSelect->Add_Children(CAction::Create(bind(&CBT_Tartaglia::Bow_PowerAttack, this)));
	pAttackSelect->Add_Children(CAction::Create(bind(&CBT_Tartaglia::Bow_RangeAttack, this)));
	pAttackSelect->Add_Children(CAction::Create(bind(&CBT_Tartaglia::Blade_ExtraAttack, this)));
	pAttackSelect->Add_Children(CAction::Create(bind(&CBT_Tartaglia::Blade_NormalAttack, this)));

	pAttack->Add_Children(pAttackSelect);

	CSequence* pWalk = CSequence::Create();
	pWalk->Add_Children(CAction::Create(bind(&CBT_Tartaglia::Check_Walk, this)));
	pWalk->Add_Children(CAction::Create(bind(&CBT_Tartaglia::Walk, this)));

	CSequence* pUndo = CSequence::Create();
	pUndo->Add_Children(CAction::Create(bind(&CBT_Tartaglia::Standby, this)));

	CSelector* pRootSelector = CSelector::Create();
	pRootSelector->Add_Children(pDeath);
	pRootSelector->Add_Children(pHit);
	pRootSelector->Add_Children(pAttack);
	pRootSelector->Add_Children(pWalk);
	pRootSelector->Add_Children(pUndo);

	m_pRootNode = pRootSelector;

	return S_OK;
}

CNode::NODE_STATE CBT_Tartaglia::Death() // ����
{
	if (m_pInfo->fHp <= 0)
	{
		*m_pState = CBoss::BOSS_DIE;
		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Tartaglia::Check_Hit() // hit/defend ��� ������?
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

	if (m_pModelCom->Get_Animation_Finished(27) 
		|| m_pModelCom->Get_Animation_Finished(28) 
		|| m_pModelCom->Get_Animation_Finished(29) 
		|| m_pModelCom->Get_Animation_Finished(30)
		|| m_pModelCom->Get_Animation_Finished(0) 
		|| m_pModelCom->Get_Animation_Finished(1) 
		|| m_pModelCom->Get_Animation_Finished(2) 
		|| m_pModelCom->Get_Animation_Finished(3) 
		|| m_pModelCom->Get_Animation_Finished(4))
		return CNode::SUCCESS;

	return CNode::RUNNING;
}

CNode::NODE_STATE CBT_Tartaglia::Check_Intersect()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, L"Layer_Player", 0));

	CCollider* pWeaponCollider = { nullptr };

	if (pPlayer->Get_CurrentWeapon() == CWeapon::WEAPON_SWORD)
	{
		pWeaponCollider = pPlayer->Get_SwordCollider();
	}
	else
	{
		// ȭ�� + ���� ����Ʈ���� ���� ��� (�ӽÿ�)
		pWeaponCollider = dynamic_cast<CCollider*>(m_pGameInstance->Get_GameObject_Component(LEVEL_GAMEPLAY, L"Layer_Player", L"Com_Collider"));
	}

	if (m_pColliderCom->Intersect(pWeaponCollider))
	{
		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Tartaglia::Hit() 
{
	*m_pState = CBoss::BOSS_HIT_H;

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_Tartaglia::Check_Defend() // �������� �Ϲݰ��� �� �� �Ұ�����, �ƴϸ� ī���͸� �Ұ�����?
{
	if (m_pInfo->isDefend)
		CNode::SUCCESS;

	//���� Ȯ���� ����
	_uint iProbability = 70;
	_uint RandomNumber = Random(100);

	//�̹� ����ϰ� �ִ� ���°� �ƴ϶�� 30% Ȯ���� hit
	if (!m_pInfo->isDefend && RandomNumber > 70)
		return CNode::FAILURE;

	// �÷��̾ �Ϲݰ����� �޴��� üũ
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, L"Layer_Player", 0));
	_uint eWeaponType = pPlayer->Get_CurrentWeapon();

	if ((pPlayer->Get_PlayerState() == PLAYER_ATTACK_1
		|| pPlayer->Get_PlayerState() == PLAYER_ATTACK_2
		|| pPlayer->Get_PlayerState() == PLAYER_ATTACK_3
		|| pPlayer->Get_PlayerState() == PLAYER_ATTACK_4) && eWeaponType == CWeapon::WEAPON_SWORD)
	{
		iProbability = 30;

		RandomNumber = Random(100);

		// 70% Ȯ���� ��� / 30% Ȯ���� ī����
		m_pInfo->isDefend = RandomNumber <= iProbability ? true : false;

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Tartaglia::Defend()
{
	if (!m_pInfo->isDefend) return CNode::FAILURE;

	if (m_pInfo->fDefendHp <= 0)
	{
		m_pInfo->isDefend = false;

		return CNode::FAILURE;
	}
	//��
	*m_pState = CBoss::BOSS_DEFEND_1;

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_Tartaglia::Attack_Defend()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, L"Layer_Player", 0));
	// ���� �������� �÷��̾ �� ������ ������ �����̶�� ...
	// Ȯ�������� ī����
	// �Ϲ� ������ �� 1
	// �� ���� ��ų�� ������ �� 2
	*m_pState = CBoss::BOSS_DEFEND_ATTACK_2;
	//*m_pState = CBoss::BOSS_DEFEND_ATTACK_2 : CBoss::BOSS_DEFEND_ATTACK_1;

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_Tartaglia::Check_Attack_Deley()
{
	//���� ��ų �����ϱ�
	// 1. ù ������ ������ ComboAttack();
	// 2. Blade ������ ���� Ȯ���� ����� ��.
	// 3. ��ų �߰��߰��� �����̰� �ʿ���. 
	if (m_isFirst)
	{
		m_Skill = SKILL_BOW_NORMAL;
		m_isFirst = false;
		m_pTransformCom->LookAt(XMLoadFloat4x4(m_pTargetMatrix).r[3]);
	}
	else
	{
		if (!m_isAttack)
		{
			if (m_fDelayTime < m_fCurrentTime)
			{
				_float fDistance = Distance(XMLoadFloat4x4(m_pTargetMatrix), XMLoadFloat4x4(m_pTransformCom->Get_WorldFloat4x4()));

				_uint iProbability = 20;
				_uint RandomNumber = Random(100);
				_bool isBlade = RandomNumber <= iProbability;

				if (isBlade)
					m_Skill = SKILL_BLADE_EXTRA;
				else
				{
					if (fDistance < 5.f) m_Skill = SKILL_BOW_NORMAL;
					else 
						m_Skill = SKILL(Random(SKILL_BLADE_EXTRA));
				}

				m_fCurrentTime = 0.f;
				m_pTransformCom->LookAt(XMLoadFloat4x4(m_pTargetMatrix).r[3]);

				m_isWalkStop = true;

				return CNode::SUCCESS;
			}

			m_isWalkStop = false;
			return CNode::FAILURE;
		}
	}

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_Tartaglia::Rush_Move()
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

	//�뽬
	if (m_Skill == SKILL_RUSH)
	{
		m_pTransformCom->LookAt(XMLoadFloat4x4(m_pTargetMatrix).r[3]);

		*m_pState = CBoss::BOSS_RUSH_BS;

		m_isAttack = true;
		return CNode::SUCCESS;
	}
	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Tartaglia::Bow_ComboAttack() // �ܷ� ǥ�� 
{
	if (m_isAttack && m_Skill == SKILL_BOW_COMBO)
	{
		if (*m_pState == CBoss::BOSS_BOW_COMBO_ATTACK && m_pModelCom->Get_Animation_Finished(12))
		{
			m_Skill = SKILL_END;
			m_isAttack = false;

			return CNode::SUCCESS;
		}
		return CNode::RUNNING;
	}

	if (m_Skill == SKILL_BOW_COMBO)
	{
		*m_pState = CBoss::BOSS_BOW_COMBO_ATTACK;
		m_isAttack = true;
		return CNode::SUCCESS;
	}
	
	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Tartaglia::Bow_CoverAttack() // �ܷ� ǥ���� ���� ��쿡�� (������ ��) 4~8
{
	if (m_isAttack && m_Skill == SKILL_BOW_COVER)
	{
		if (*m_pState == CBoss::BOSS_BOW_COVER_ATTACK_BS && m_pModelCom->Get_Animation_Finished(14))
			*m_pState = CBoss::BOSS_BOW_COVER_ATTACK_LOOP;
		else if (*m_pState == CBoss::BOSS_BOW_COVER_ATTACK_LOOP && m_pModelCom->Get_LoopAnimation_Finished())
		{
			m_pTransformCom->LookAt(XMLoadFloat4x4(m_pTargetMatrix).r[3]);

			if (m_iAttackCount < m_iCurrentAttackCount)
			{
				m_iAttackCount = 0;
				m_iCurrentAttackCount = 0;
				*m_pState = CBoss::BOSS_BOW_COVER_ATTACK_AS;
			}
			else
				m_iCurrentAttackCount++;
		}
		else if (*m_pState == CBoss::BOSS_BOW_COVER_ATTACK_AS && m_pModelCom->Get_Animation_Finished(13))
		{
			// CoverAttack ������ �����ٸ�
			m_isAttack = false;

			return CNode::SUCCESS;
		}

		return CNode::RUNNING;
	}

	if (m_Skill == SKILL_BOW_COVER)
	{
		*m_pState = CBoss::BOSS_BOW_COVER_ATTACK_BS;
		m_iAttackCount = Random(4) + 4;
		m_isAttack = true;

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Tartaglia::Bow_NormalAttack() // �ܷ� �Ϲ� ���� (6�� ����)
{
	if (m_isAttack && m_Skill == SKILL_BOW_NORMAL)
	{
		if (*m_pState == CBoss::BOSS_BOW_NORMAL_ATTACK_BS && m_pModelCom->Get_Animation_Finished(17))
			*m_pState = CBoss::BOSS_BOW_NORMAL_ATTACK_LOOP;
		else if (*m_pState == CBoss::BOSS_BOW_NORMAL_ATTACK_LOOP && m_pModelCom->Get_LoopAnimation_Finished())
		{
			m_pTransformCom->LookAt(XMLoadFloat4x4(m_pTargetMatrix).r[3]);
			if (m_iAttackCount < m_iCurrentAttackCount)
			{
				*m_pState = CBoss::BOSS_BOW_NORMAL_ATTACK_AS;
				m_iAttackCount = 0;
				m_iCurrentAttackCount = 0;
			}
			else
			{
				++m_iCurrentAttackCount;
			}
		}
		else if (*m_pState == CBoss::BOSS_BOW_NORMAL_ATTACK_AS && m_pModelCom->Get_Animation_Finished(16))
		{
			m_isAttack = false;
			return CNode::SUCCESS;
		}

		return CNode::RUNNING;
	}

	if (m_Skill == SKILL_BOW_NORMAL)
	{
		*m_pState = CBoss::BOSS_BOW_NORMAL_ATTACK_BS;

		m_iAttackCount = Random(5) + 3;
		m_isAttack = true;

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Tartaglia::Bow_PowerAttack() // �� ��ȯ, �ܷ� ǥ�� 
{
	if (m_isAttack && m_Skill == SKILL_BOW_POWER)
	{
		if (*m_pState == CBoss::BOSS_BOW_POWER_ATTACK_BS && m_pModelCom->Get_Animation_Finished(20))
			*m_pState = CBoss::BOSS_BOW_POWER_ATTACK_LOOP;
		else if (*m_pState == CBoss::BOSS_BOW_POWER_ATTACK_LOOP && m_pModelCom->Get_LoopAnimation_Finished())
			*m_pState = CBoss::BOSS_BOW_POWER_ATTACK_AS;
		else if (*m_pState == CBoss::BOSS_BOW_POWER_ATTACK_AS && m_pModelCom->Get_Animation_Finished(19))
		{
			m_isAttack = false;
			return CNode::SUCCESS;
		}

		return CNode::RUNNING;
	}

	if (m_Skill == SKILL_BOW_POWER)
	{
		*m_pState = CBoss::BOSS_BOW_POWER_ATTACK_BS;
		m_isAttack = true;

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Tartaglia::Bow_RangeAttack() //�ĵ� ��ȯ 3~5
{
	if (m_isAttack && m_Skill == SKILL_BOW_RENGE)
	{
		if (*m_pState == CBoss::BOSS_BOW_RANGE_ATTACK_BS && m_pModelCom->Get_Animation_Finished(23))
			*m_pState = CBoss::BOSS_BOW_RANGE_ATTACK_LOOP_1;
		else if (m_pModelCom->Get_LoopAnimation_Finished())
		{
			m_pTransformCom->LookAt(XMLoadFloat4x4(m_pTargetMatrix).r[3]);

			if (*m_pState == CBoss::BOSS_BOW_RANGE_ATTACK_LOOP_1)
			{
				*m_pState = CBoss::BOSS_BOW_RANGE_ATTACK_LOOP_2;
			}
			else if (*m_pState == CBoss::BOSS_BOW_RANGE_ATTACK_LOOP_2)
			{
				*m_pState = CBoss::BOSS_BOW_RANGE_ATTACK_LOOP_1;
				if (m_iAttackCount < m_iCurrentAttackCount)
				{
					m_iAttackCount = 0;
					m_iCurrentAttackCount = 0;
					*m_pState = CBoss::BOSS_BOW_RANGE_ATTACK_AS;
				}
				else
					++m_iCurrentAttackCount;
			}
		}
		else if (*m_pState == CBoss::BOSS_BOW_RANGE_ATTACK_AS && m_pModelCom->Get_Animation_Finished(22))
		{
			m_isAttack = false;
			return CNode::SUCCESS;
		}

		return CNode::RUNNING;
	}

	if (m_Skill == SKILL_BOW_RENGE)
	{
		*m_pState = CBoss::BOSS_BOW_RANGE_ATTACK_BS;

		m_iAttackCount = 3 + Random(2);
		m_isAttack = true;

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Tartaglia::Blade_ExtraAttack() // �� �Ⱦ� - �÷��̾� ������ �ٰ��� ����
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

	// �÷��̾�� �ָ� �������ִ� ��� ������ �̵��Ͽ� ������
	if (m_Skill == SKILL_BLADE_EXTRA)
	{
		*m_pState = CBoss::BOSS_BLADE_EXTRA_ATTACK;
		m_isAttack = true;

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Tartaglia::Blade_NormalAttack() // �� �Ⱦ�
{
	if (m_isAttack && m_Skill == SKILL_BLADE_NORMAL)
	{
		if (m_iBladeNormalAttackMax <= m_iBladeNormalAttackMaxCount)
		{
			m_isAttack = false;
			m_iBladeNormalAttackMax = 0;

			return CNode::SUCCESS;
		}
		else {
			if (*m_pState == CBoss::BOSS_BLADE_NORMAL_ATTACK_1 && m_pModelCom->Get_Animation_Finished(6))
			{
				*m_pState = CBoss::BOSS_BLADE_NORMAL_ATTACK_2;
				m_iBladeNormalAttackMaxCount++;
			}
			else if (*m_pState == CBoss::BOSS_BLADE_NORMAL_ATTACK_2 && m_pModelCom->Get_Animation_Finished(7))
			{
				*m_pState = CBoss::BOSS_BLADE_NORMAL_ATTACK_3;
				m_iBladeNormalAttackMaxCount++;
			}
			else if (*m_pState == CBoss::BOSS_BLADE_NORMAL_ATTACK_3 && m_pModelCom->Get_Animation_Finished(8))
			{
				*m_pState = CBoss::BOSS_BLADE_NORMAL_ATTACK_4;
				m_iBladeNormalAttackMaxCount++;
			}
			else if (*m_pState == CBoss::BOSS_BLADE_NORMAL_ATTACK_4 && m_pModelCom->Get_Animation_Finished(9))
			{
				*m_pState = CBoss::BOSS_BLADE_NORMAL_ATTACK_1;
				m_iBladeNormalAttackMaxCount++;
			}
		}

		return CNode::RUNNING;
	}

	if (m_Skill == SKILL_BLADE_NORMAL)
	{
		m_iBladeNormalAttackMax = Random(6);
		*m_pState = CBoss::BOSS_BLADE_NORMAL_ATTACK_1;

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Tartaglia::Check_Walk()
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

CNode::NODE_STATE CBT_Tartaglia::Walk()
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

CNode::NODE_STATE CBT_Tartaglia::Standby()
{
	*m_pState = CBoss::BOSS_IDLE;

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

}
