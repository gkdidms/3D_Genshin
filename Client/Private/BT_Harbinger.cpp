#include "BT_Harbinger.h"

#include "Boss.h"

#include "Weapon.h"

CBT_Harbinger::CBT_Harbinger()
{
}

HRESULT CBT_Harbinger::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Node()))
		return E_FAIL;

	return S_OK;
}

void CBT_Harbinger::Tick(const _float& fTimeDelta)
{
	this->Evaluate();
}

CNode::NODE_STATE CBT_Harbinger::Evaluate()
{
	return m_pRootNode->Evaluate();
}

HRESULT CBT_Harbinger::Ready_Node()
{
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
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, L"Layer_Player", 0));

	CCollider* pWeaponCollider = { nullptr };

	if (pPlayer->Get_CurrentWeapon() == CWeapon::WEAPON_SWORD)
	{
		pWeaponCollider = pPlayer->Get_SwordCollider();
	}
	else
	{
		// 화살 + 마법 이펙트들은 따로 사용 (임시용)
		pWeaponCollider = dynamic_cast<CCollider*>(m_pGameInstance->Get_GameObject_Component(LEVEL_GAMEPLAY, L"Layer_Player", L"Com_Collider"));
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

CNode::NODE_STATE CBT_Harbinger::Check_Defend()
{
	//일정 확률로 나옴
	_uint iProbability = 80;
	_uint RandomNumber = Random(100);

	//이미 방어하고 있는 상태가 아니라면 20% 확률로 hit
	if (!m_pInfo->isDefend && RandomNumber > iProbability)
		return CNode::FAILURE;

	// 플레이어가 일반공격을 햇는지 체크
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, L"Layer_Player", 0));
	_uint eWeaponType = pPlayer->Get_CurrentWeapon();

	if ((pPlayer->Get_PlayerState() == PLAYER_ATTACK_1
		|| pPlayer->Get_PlayerState() == PLAYER_ATTACK_2
		|| pPlayer->Get_PlayerState() == PLAYER_ATTACK_3
		|| pPlayer->Get_PlayerState() == PLAYER_ATTACK_4) && eWeaponType == CWeapon::WEAPON_SWORD)
	{
		iProbability = 90;

		RandomNumber = Random(100);

		// 90% 확률로 방어 / 10% 확률로 카운터
		m_pInfo->isDefend = RandomNumber <= iProbability ? true : false;

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Harbinger::Defend()
{
	if (!m_pInfo->isDefend) return CNode::FAILURE;

	//방어막
	*m_pState = CBoss::BOSS_DEFEND_1;

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_Harbinger::Attack_Defend()
{
	// 확률적으로 카운터
	*m_pState = CBoss::BOSS_DEFEND_ATTACK_1;

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_Harbinger::Check_Attack_Deley()
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
		if (!m_isAttack && m_fDelayTime < m_fCurrentTime)
		{
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, L"Layer_Player", 0));

			_vector vTarget = XMLoadFloat4x4(m_pTargetMatrix).r[3];
			_vector vCurrentPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

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
	}

	return CNode::FAILURE;
}

_bool CBT_Harbinger::Check_Rear_ToPlayer()
{
	_vector vTarget = XMLoadFloat4x4(m_pTargetMatrix).r[3];
	_vector vCurrentPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

	return !AngleOfView(180.f, vTarget, vCurrentPos, vLook);
}

CNode::NODE_STATE CBT_Harbinger::Bow_Cover_Attack()
{
	return CNode::NODE_STATE();
}

CNode::NODE_STATE CBT_Harbinger::Bow_Power_Attack()
{
	return CNode::NODE_STATE();
}

CNode::NODE_STATE CBT_Harbinger::Blade_Range_Attack()
{
	return CNode::NODE_STATE();
}

CNode::NODE_STATE CBT_Harbinger::Blade_Extra_Attack()
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

CNode::NODE_STATE CBT_Harbinger::DualBlade_Cycle_Attack()
{
	return CNode::NODE_STATE();
}

CNode::NODE_STATE CBT_Harbinger::DualBlade_Strike()
{
	if (m_isAttack && m_Skill == SKILL_DUALBLADE_STRIKE)
	{
		if (*m_pState == CBoss::BOSS_DUALBLADE_STRIKE_ATTACK_BS && m_pModelCom->Get_Animation_Finished())
			*m_pState = CBoss::BOSS_DUALBLADE_STRIKE_ATTACK_LOOP;
		else if (*m_pState == CBoss::BOSS_DUALBLADE_STRIKE_ATTACK_LOOP && m_pModelCom->Get_LoopAnimation_Finished())
			*m_pState = CBoss::BOSS_DUALBLADE_STRIKE_ATTACK_AS;
		else if (*m_pState == CBoss::BOSS_DUALBLADE_STRIKE_ATTACK_AS && m_pModelCom->Get_Animation_Finished())
		{
			m_isAttack = false;
			return CNode::SUCCESS;
		}

		return CNode::RUNNING;
	}

	if (m_Skill == SKILL_DUALBLADE_STRIKE)
	{
		m_isAttack = true;
		*m_pState = CBoss::BOSS_DUALBLADE_STRIKE_ATTACK_BS;

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Harbinger::DualBlade_Sweep()
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

CNode::NODE_STATE CBT_Harbinger::DualBlade_Hiraishin()
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

CNode::NODE_STATE CBT_Harbinger::Normal_Attack()
{
	return CNode::NODE_STATE();
}

CNode::NODE_STATE CBT_Harbinger::Check_Walk()
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

CNode::NODE_STATE CBT_Harbinger::Walk()
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
