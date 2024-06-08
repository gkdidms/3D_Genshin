#include "BT_Harbinger.h"

#include "Boss.h"

#include "Weapon.h"

#include "Sequence.h"
#include "Selector.h"
#include "Action.h"

#pragma region Skill
#include "Harbinger_Blade_Range.h"
#include "Harbinger_Normal.h"
#pragma endregion


CBT_Harbinger::CBT_Harbinger()
{
}

HRESULT CBT_Harbinger::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Node()))
		return E_FAIL;

	m_fDelayTime = { 3.f };

	return S_OK;
}

void CBT_Harbinger::Tick(const _float& fTimeDelta)
{
	if (!m_isAttack)
		m_fCurrentTime += fTimeDelta;
	else if (m_Skill == SKILL_BOW_NORMAL || m_Skill == SKILL_DUALBLADE_NORMAL_2) // 공격중이고 활 일반 공격 중이면 저장함 
		m_fCurrentRunTime += fTimeDelta;
	else
		m_fAtkCurrentTime += fTimeDelta;

	this->Evaluate();
}

CNode::NODE_STATE CBT_Harbinger::Evaluate()
{
	return m_pRootNode->Evaluate();
}

HRESULT CBT_Harbinger::Ready_Node()
{
	//Death
	CSequence* pDeath = CSequence::Create();
	pDeath->Add_Children(CAction::Create(bind(&CBT_Harbinger::Death, this)));

	//Hit
	CSequence* pHit = CSequence::Create();
	pHit->Add_Children(CAction::Create(bind(&CBT_Harbinger::Check_Hit, this)));
	pHit->Add_Children(CAction::Create(bind(&CBT_Harbinger::Check_Intersect, this)));
	pHit->Add_Children(CAction::Create(bind(&CBT_Harbinger::Hit, this)));

	//Attack
	CSequence* pAttack = CSequence::Create();
	pAttack->Add_Children(CAction::Create(bind(&CBT_Harbinger::Check_Attack_Deley, this)));
	pAttack->Add_Children(CAction::Create(bind(&CBT_Harbinger::Check_Attack_Range, this)));
	pAttack->Add_Children(CAction::Create(bind(&CBT_Harbinger::Check_MeleeAtk, this)));
	pAttack->Add_Children(CAction::Create(bind(&CBT_Harbinger::Check_RangeAtk, this)));

	CSelector* pAttackSelect = CSelector::Create();
	pAttackSelect->Add_Children(CAction::Create(bind(&CBT_Harbinger::Bow_Cover_Attack, this)));
	pAttackSelect->Add_Children(CAction::Create(bind(&CBT_Harbinger::Bow_Normal_Attack, this)));
	pAttackSelect->Add_Children(CAction::Create(bind(&CBT_Harbinger::Bow_Power_Attack, this)));
	pAttackSelect->Add_Children(CAction::Create(bind(&CBT_Harbinger::Blade_Range_Attack, this)));
	//pAttackSelect->Add_Children(CAction::Create(bind(&CBT_Harbinger::Blade_Normal_Attack, this)));
	pAttackSelect->Add_Children(CAction::Create(bind(&CBT_Harbinger::Blade_Extra_Attack, this)));
	pAttackSelect->Add_Children(CAction::Create(bind(&CBT_Harbinger::DualBlade_Cycle_Attack, this)));
	pAttackSelect->Add_Children(CAction::Create(bind(&CBT_Harbinger::DualBlade_Normal_1, this)));
	//pAttackSelect->Add_Children(CAction::Create(bind(&CBT_Harbinger::DualBlade_Normal_2, this)));
	//pAttackSelect->Add_Children(CAction::Create(bind(&CBT_Harbinger::DualBlade_Strike, this)));
	//pAttackSelect->Add_Children(CAction::Create(bind(&CBT_Harbinger::DualBlade_Sweep, this)));
	//pAttackSelect->Add_Children(CAction::Create(bind(&CBT_Harbinger::DualBlade_Hiraishin, this)));
	pAttackSelect->Add_Children(CAction::Create(bind(&CBT_Harbinger::Normal_Attack, this)));

	pAttack->Add_Children(pAttackSelect);

	CSequence* pWalk = CSequence::Create();
	pWalk->Add_Children(CAction::Create(bind(&CBT_Harbinger::Check_Teleport, this)));
	pWalk->Add_Children(CAction::Create(bind(&CBT_Harbinger::Teleport, this)));

	CSequence* pUndo = CSequence::Create();
	pUndo->Add_Children(CAction::Create(bind(&CBT_Harbinger::Standby, this)));

	CSelector* pRootSelector = CSelector::Create();
	pRootSelector->Add_Children(pDeath);
	pRootSelector->Add_Children(pHit);
	pRootSelector->Add_Children(pAttack);
	pRootSelector->Add_Children(pWalk);
	pRootSelector->Add_Children(pUndo);

	m_pRootNode = pRootSelector;

	return S_OK;
}

CNode::NODE_STATE CBT_Harbinger::Death()
{
	if (m_pInfo->fHp <= 0)
	{
		*m_pState = CBoss::BOSS_DIE;
		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Harbinger::Check_Hit()
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

CNode::NODE_STATE CBT_Harbinger::Check_Intersect()
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

CNode::NODE_STATE CBT_Harbinger::Hit()
{
	*m_pState = CBoss::BOSS_HIT_H;

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_Harbinger::Check_Attack_Deley()
{
	
	if (!m_isAttack)
	{
		if (m_fDelayTime < m_fCurrentTime)
		{
			m_fCurrentTime = 0.f;
			m_pTransformCom->LookAt(XMLoadFloat4x4(m_pTargetMatrix).r[3]);

			m_isWalkStop = false;

			return CNode::SUCCESS;
		}
		m_isWalkStop = true;
		return CNode::FAILURE;
	}

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_Harbinger::Check_Attack_Range() // 원거리인가 근거리인가?
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
		

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Harbinger::Check_MeleeAtk()	// 너무 가까우면 근거리 공격을 하거나 뒤로 이동하거나 
{
	if (m_RangeType == RANGE)
		return CNode::SUCCESS;

	if (m_isAttack)
		return CNode::SUCCESS;

	_uint iResult = Random(100);

	if (iResult < m_iTeleportPer) // 뒤로 이동하는걸로
		return CNode::FAILURE;

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(LEVEL_STAGE_BOSS, L"Layer_Player", 0));

	//근거리 공격
	m_Skill = SKILL_BOW_NORMAL;

	/*
	_uint iSkillNum = Random(5);

	switch (iSkillNum)
	{
	case 0:
		m_Skill = SKILL_BLADE_EXTRA;
		break;
	case 1:
		m_Skill = SKILL_BLADE_RANGE;
		break;
	case 2:
		m_Skill = SKILL_DUALBLADE_NORMAL_1;
		break;
	case 3:
		m_Skill = SKILL_DUALBLADE_CYCLE;
		break;
	case 4:
		m_Skill = SKILL_NORMAL;
		break;
	default:
		break;
	}
	*/

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_Harbinger::Check_RangeAtk()	// 가까이 이동하거나 원거리 공격을 하거나 둘 중 하나
{
	if (m_RangeType == MELEE)
		return CNode::SUCCESS;

	if (m_isAttack)
		return CNode::SUCCESS;

	_uint iResult = Random(100);

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(LEVEL_STAGE_BOSS, L"Layer_Player", 0));

	if (iResult < m_iTeleportPer) // 걷기로 빠짐
		return CNode::FAILURE;

	m_Skill = SKILL_BOW_NORMAL;
	//_uint iSkillNum = Random(6);

	//switch (iSkillNum)
	//{
	//case 0:
	//	m_Skill = SKILL_BOW_COVER;
	//	break;
	//case 1:
	//	m_Skill = SKILL_BOW_NORMAL;
	//	break;
	//case 2:
	//	m_Skill = SKILL_BOW_POWER;
	//	break;
	//case 3:
	//	m_Skill = SKILL_BLADE_RANGE;
	//	break;
	//case 4:
	//	m_Skill = SKILL_DUALBLADE_NORMAL_1;
	//	break;
	//case 5:
	//	m_Skill = SKILL_DUALBLADE_CYCLE;
	//	break;
	//default:
	//	break;
	//}

	return CNode::SUCCESS;
}

_bool CBT_Harbinger::Check_Rear_ToPlayer()
{
	// 플레이어의 뒤를 봤을때 공격 해야함.
	_vector vTarget = XMLoadFloat4x4(m_pTargetMatrix).r[3];
	_vector vCurrentPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vLook = XMLoadFloat4x4(m_pTargetMatrix).r[2];

	return !AngleOfView(180.f, vCurrentPos, vTarget, vLook);
}

CNode::NODE_STATE CBT_Harbinger::Bow_Cover_Attack() //화살비
{
	if (m_isAttack && *m_pState == CBoss::BOSS_BOW_COVER_ATTACK)
	{
		if (m_pModelCom->Get_Animation_Finished())
		{
			// 끝나면
			m_isAttack = false;

			return CNode::SUCCESS;
		}

		return CNode::RUNNING;
	}

	if (m_Skill == SKILL_BOW_COVER)
	{
		*m_pState = CBoss::BOSS_BOW_COVER_ATTACK;
		m_isAttack = true;

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Harbinger::Bow_Normal_Attack() // 일반공격 / 최대 3번까지 가능
{
	if (m_isAttack && *m_pState == CBoss::BOSS_BOW_NORMAL_ATTACK_BS)
	{
		if (m_fBowNormalTime < m_fCurrentRunTime && m_isAtk)
		{
			m_pTransformCom->LookAt(XMLoadFloat4x4(m_pTargetMatrix).r[3]);

			++m_iCurrentAttackCount;
			m_fCurrentRunTime = 0.f;

			//화살 생성
			CBullet::BULLET_DESC BulletDesc{};
			BulletDesc.fAtk = 245;
			_matrix HandMatrix = XMMatrixIdentity();
			HandMatrix.r[3] = XMVectorSet(0.f, 2.3f, 0.6f, 1.f);
			XMStoreFloat4x4(&BulletDesc.HandCombinedTransformationMatrix, HandMatrix);
			BulletDesc.ParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
			BulletDesc.pTargetPos = XMLoadFloat4x4(m_pTargetMatrix).r[3];
			BulletDesc.fSpeedPecSec = 30.f;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STAGE_BOSS, TEXT("Prototype_GameObject_Skill_Harbinger_Bow_Normal"), TEXT("Layer_Bullet"), &BulletDesc)))
				return CNode::RUNNING;

			if (m_iAttackCount > m_iCurrentAttackCount)
				m_pModelCom->Anim_Reset();
			else m_isAtk = false;
		}
		if (m_pModelCom->Get_Animation_Finished())
		{
			m_fCurrentRunTime = 0.f;
			m_iCurrentAttackCount = 0;
			m_isAttack = false;

			return CNode::SUCCESS;
		}

		return CNode::RUNNING;
	}

	if (m_Skill == SKILL_BOW_NORMAL)
	{
		m_iAttackCount = Random(3);
		*m_pState = CBoss::BOSS_BOW_NORMAL_ATTACK_BS;
		m_isAttack = true;
		m_isAtk = true;

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Harbinger::Bow_Power_Attack() // 고래 소환 
{
	if (m_isAttack && *m_pState == CBoss::BOSS_BOW_POWER_ATTACK)
	{
		if (m_pModelCom->Get_Animation_Finished())
		{
			m_isAttack = false;
			
			return CNode::SUCCESS;
		}

		return CNode::RUNNING;
	}

	if (m_Skill == SKILL_BOW_POWER)
	{
		*m_pState = CBoss::BOSS_BOW_POWER_ATTACK;
		m_isAttack = true;

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Harbinger::Blade_Range_Attack() // 물 분출 (물이 튀어오름)
{
	if (m_isAttack && *m_pState == CBoss::BOSS_BLADE_RANGE_ATTACK)
	{
		if (1.5f <= m_fAtkCurrentTime && m_isAtk)
		{
			m_isAtk = false;

			CHarbinger_Blade_Range::BOSS_SKILL_DESC SkillDesc{};
			SkillDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
			SkillDesc.pParentLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));
			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STAGE_BOSS, TEXT("Prototype_GameObject_Skill_Harbinger_Bow_Range"), TEXT("Layer_Skill"), &SkillDesc)))
				return CNode::RUNNING;
		}

		if (m_pModelCom->Get_Animation_Finished())
		{
			m_isAttack = false;

			return CNode::SUCCESS;
		}

		return CNode::RUNNING;
	}

	if (m_Skill == SKILL_BLADE_RANGE)
	{
		*m_pState = CBoss::BOSS_BLADE_RANGE_ATTACK;
		m_isAttack = true;
		m_fAtkCurrentTime = 0.f;
		m_isAtk = true;

		return CNode::SUCCESS;
	}
	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Harbinger::Blade_Normal_Attack()
{
	if (m_isAttack && *m_pState == CBoss::BOSS_BLADE_NORMAL_ATTACK_1)
	{
		if (m_pModelCom->Get_Animation_Finished())
		{
			m_isAttack = false;

			return CNode::SUCCESS;
		}

		return CNode::RUNNING;
	}

	if (m_Skill == SKILL_BLADE_NORMAL)
	{
		*m_pState = CBoss::BOSS_BLADE_NORMAL_ATTACK_1;
		m_isAttack = true;

		return CNode::SUCCESS;
	}
	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Harbinger::Blade_Extra_Attack() //평타
{
	if (m_isAttack && m_Skill == SKILL_BLADE_EXTRA)
	{
		if (*m_pState == CBoss::BOSS_BLADE_EXTRA_ATTACK && m_pModelCom->Get_Animation_Finished())
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

CNode::NODE_STATE CBT_Harbinger::DualBlade_Cycle_Attack() // 번개 칼날 
{
	if (m_isAttack && m_Skill == SKILL_DUALBLADE_CYCLE)
	{
		if (m_pModelCom->Get_Animation_Finished())
		{
			m_isAttack = false;

			return CNode::SUCCESS;
		}

		return CNode::RUNNING;
	}

	if (m_Skill == SKILL_DUALBLADE_CYCLE)
	{
		m_isAttack = true;
		*m_pState = CBoss::BOSS_DUALBLADE_CYCLE_ATTACK;
		return CNode::SUCCESS;
	}
	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Harbinger::DualBlade_Normal_1() // 번개창
{
	if (m_isAttack && m_Skill == SKILL_DUALBLADE_NORMAL_1)
	{
		if (m_pModelCom->Get_Animation_Finished())
		{
			m_isAttack = false;

			return CNode::SUCCESS;
		}

		return CNode::RUNNING;
	}

	if (m_Skill == SKILL_DUALBLADE_NORMAL_1)
	{
		*m_pState = CBoss::BOSS_DUALBLADE_NORMAL_ATTACK_1;
		m_isAttack = true;

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Harbinger::DualBlade_Normal_2() // 돌진 3~5번 돌진을 한다.
{
	if (m_isAttack && m_Skill == SKILL_DUALBLADE_NORMAL_2)
	{
		if (m_fSkritTime < m_fCurrentRunTime)
		{
			++m_iCurrentAttackCount;

			if (m_iAttackCount > m_iCurrentAttackCount)
			{
				m_pTransformCom->LookAt(XMLoadFloat4x4(m_pTargetMatrix).r[3]);
				m_pModelCom->Anim_Reset();

				return CNode::SUCCESS;
			}

			if (m_pModelCom->Get_Animation_Finished())
			{
				m_fCurrentRunTime = 0.f;
				m_iCurrentAttackCount = 0;
				m_isAttack = false;

				return CNode::SUCCESS;
			}
		}
		return CNode::RUNNING;
	}

	if (m_Skill == SKILL_DUALBLADE_NORMAL_1)
	{
		m_iAttackCount = Random(5);
		*m_pState = CBoss::BOSS_DUALBLADE_NORMAL_ATTACK_2;
		m_isAttack = true;

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Harbinger::DualBlade_Strike()
{
	if (m_isAttack && m_Skill == SKILL_DUALBLADE_STRIKE)
	{
		if (*m_pState == CBoss::BOSS_DUALBLADE_STRIKE_ATTACK && m_pModelCom->Get_Animation_Finished())
		{
			m_isAttack = false;
			return CNode::SUCCESS;
		}

		return CNode::RUNNING;
	}

	if (m_Skill == SKILL_DUALBLADE_STRIKE)
	{
		m_isAttack = true;
		*m_pState = CBoss::BOSS_DUALBLADE_STRIKE_ATTACK;

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Harbinger::DualBlade_Sweep() // 뒷치기
{
	if (m_isAttack && m_Skill == SKILL_DUALBLADE_SWEEP)
	{
		if (m_pModelCom->Get_Animation_Finished())
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

CNode::NODE_STATE CBT_Harbinger::DualBlade_Hiraishin() // 번개창 낙하
{
	if (m_isAttack && m_Skill == SKILL_DUALBLADE_HIRAISHIN)
	{
		if (*m_pState == CBoss::BOSS_DUALBLADE_HIRAISHIN_BS && m_pModelCom->Get_Animation_Finished())
		{
			*m_pState = CBoss::BOSS_DUALBLADE_HIRAISHIN_AS;
		}
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

CNode::NODE_STATE CBT_Harbinger::Normal_Attack() // 3단 콤보 
{
	if (m_isAttack && m_Skill == SKILL_NORMAL)
	{
		if (m_pModelCom->Get_Animation_Finished())
		{
			m_isAttack = false;
			return CNode::SUCCESS;
		}

		return CNode::RUNNING;
	}

	if (m_Skill == SKILL_NORMAL)
	{
		m_isAttack = true;
		*m_pState = CBoss::BOSS_HARBINGER_NORMAL_ATTACK;

		return CNode::FAILURE;
	}
	return CNode::NODE_STATE();
}

//순간 이동도 있고 걷기도 있는데 이 둘을 어떻게 분리할것인가..?
CNode::NODE_STATE CBT_Harbinger::Check_Teleport()
{
	if (m_isWalkStop) return CNode::FAILURE;

	if (*m_pState != CBoss::BOSS_MOVE_BACK
		&& *m_pState != CBoss::BOSS_MOVE_FORWARD)
		return CNode::SUCCESS;

	if (m_pModelCom->Get_Animation_Finished())
	{
		m_isAttack = false;
		return CNode::FAILURE;
	}
		

	return CNode::RUNNING;
}

CNode::NODE_STATE CBT_Harbinger::Teleport()
{
	// 순간이동
	if (m_RangeType == RANGE)
		*m_pState = CBoss::BOSS_MOVE_FORWARD;
	else if (m_RangeType == MELEE)
		*m_pState = CBoss::BOSS_MOVE_BACK;

	m_isAttack = true;

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_Harbinger::Standby()
{
	*m_pState = CBoss::BOSS_IDLE;

	return CNode::SUCCESS;
}

CBT_Harbinger* CBT_Harbinger::Create(void* pArg)
{
	CBT_Harbinger* pInstance = new CBT_Harbinger();

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CBT_Harbinger::Free()
{
	__super::Free();
}
