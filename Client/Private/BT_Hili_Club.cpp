#include "BT_Hili_Club.h"

#include "GameInstance.h"

#include "Player.h"
#include "Weapon.h"

#include "Selector.h"
#include "Sequence.h"
#include "Action.h"

CBT_Hili_Club::CBT_Hili_Club()
	: CBT_Hili{}
{
}

HRESULT CBT_Hili_Club::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_iAttackRange = 3.f;
	m_iDetectRange = 10.f;

	return S_OK;
}

void CBT_Hili_Club::Tick(const _float& fTimeDelta)
{
	if (m_isAttack)
		m_fAttackTime += fTimeDelta;

	this->Evaluate();
}

CNode::NODE_STATE CBT_Hili_Club::Evaluate()
{
	return m_pRootNode->Evaluate();
}

CNode::NODE_STATE CBT_Hili_Club::CheckDeath()
{
	if (*m_pState != CHili::HILI_DIE)
		return CNode::SUCCESS;

	if (m_pModelCom->Get_Animation_Finished())
		return CNode::SUCCESS;

	return CNode::RUNNING;
}

CNode::NODE_STATE CBT_Hili_Club::Death()
{
	return __super::Death();
}

CNode::NODE_STATE CBT_Hili_Club::CheckBone()
{
	if (*m_pState != CHili::HILI_BORN && *m_pState != CHili::HILI_TAUNT_1 && *m_pState != CHili::HILI_TAUNT_2)
		return CNode::FAILURE;

	if (m_pModelCom->Get_Animation_Finished())
		return CNode::SUCCESS;

	return CNode::RUNNING;
}

CNode::NODE_STATE CBT_Hili_Club::CheckTaunt()
{
	if (*m_pState == CHili::HILI_TAUNT_1 || *m_pState == CHili::HILI_TAUNT_2)
		return CNode::FAILURE;

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_Hili_Club::Taunt()
{
	_uint iResult = Random(2);

	*m_pState = iResult == 0 ? CHili::HILI_TAUNT_1 : CHili::HILI_TAUNT_2;

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_Hili_Club::CheckHit()
{
	if (*m_pState != CHili::HILI_HIT)
		return CNode::SUCCESS;

	if (m_pModelCom->Get_Animation_Finished(8) || m_pModelCom->Get_Animation_Finished(9))
		return CNode::SUCCESS;

	return CNode::RUNNING;
}

CNode::NODE_STATE CBT_Hili_Club::Hit()
{
	return __super::Hit();
}

CNode::NODE_STATE CBT_Hili_Club::CheckAttack()
{
	if (*m_pState != CHili::HILI_NORMAL_ATK && *m_pState != CHili::HILI_THUMP_ATK && *m_pState != CHili::HILI_TRIPLE_ATK)
		return CNode::SUCCESS;

	if (m_pModelCom->Get_Animation_Finished())
	{
		//m_isAttack = false;
		return CNode::SUCCESS;
	}

	return CNode::RUNNING;
}

CNode::NODE_STATE CBT_Hili_Club::CheckDiscoverToPlayer()
{
	return __super::CheckDiscoverToPlayer();
}

CNode::NODE_STATE CBT_Hili_Club::CheckRangePlayer()
{
	return __super::CheckRangePlayer();
}

CNode::NODE_STATE CBT_Hili_Club::CheckAttackTime()
{
	return __super::CheckAttackTime();
}

CNode::NODE_STATE CBT_Hili_Club::SelectAttack()
{
	if (!m_isAttack)
	{
		m_Skill = SKILL(Random(3));

		m_isAttack = true;
	}

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_Hili_Club::Normal_Attack()
{
	if (m_Skill == HILI_NORMAL_ATK)
	{
		*m_pState = CHili::HILI_NORMAL_ATK;
		return CNode::SUCCESS;
	}
		
	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Hili_Club::Thump_Attack()
{
	if (m_Skill == HILI_THUMP_ATK)
	{
		*m_pState = CHili::HILI_THUMP_ATK;
		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Hili_Club::Triple_Attack()
{
	if (m_Skill == HILI_TRIPLE_ATK)
	{
		*m_pState = CHili::HILI_TRIPLE_ATK;
		return CNode::SUCCESS;
	}
		
	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Hili_Club::CheckDetect()
{
	return __super::CheckDetect();
}

CNode::NODE_STATE CBT_Hili_Club::CheckLookPlayer()
{
	if (!m_isDiscovered)
	{
		_float vLookAngle = { 100.f };

		_vector vTargetPos = XMLoadFloat4x4(m_pTargetMatrix).r[3];
		_vector vHiliPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_vector vHiliLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

		if (AngleOfView(vLookAngle, vTargetPos, vHiliPos, vHiliLook))
		{
			m_isDiscovered = true;
			*m_pState = CHili::HILI_BORN; // 발견 
			return CNode::SUCCESS;
		}

		return CNode::FAILURE;
	}
	else m_pTransformCom->LookAt(XMLoadFloat4x4(m_pTargetMatrix).r[3]);

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_Hili_Club::MoveToPlayer()
{
	if (*m_pState != CHili::HILI_BORN)
		*m_pState = CHili::HILI_RUN;

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_Hili_Club::MoveToPrePlace()
{
	return __super::MoveToPrePlace();
}

CNode::NODE_STATE CBT_Hili_Club::StandBy()
{
	return __super::StandBy();
}

void CBT_Hili_Club::Ready_Node()
{
	//공격 확인
	CSelector* pSelector = CSelector::Create();

	CSequence* pDeathSeq = CSequence::Create();
	pDeathSeq->Add_Children(CAction::Create(bind(&CBT_Hili_Club::CheckDeath, this)));
	pDeathSeq->Add_Children(CAction::Create(bind(&CBT_Hili_Club::Death, this)));

	CSequence* pTauntSeq = CSequence::Create();
	pTauntSeq->Add_Children(CAction::Create(bind(&CBT_Hili_Club::CheckBone, this)));
	pTauntSeq->Add_Children(CAction::Create(bind(&CBT_Hili_Club::CheckTaunt, this)));
	pTauntSeq->Add_Children(CAction::Create(bind(&CBT_Hili_Club::Taunt, this)));

	CSequence* pHitSeq = CSequence::Create();
	pHitSeq->Add_Children(CAction::Create(bind(&CBT_Hili_Club::CheckHit, this)));
	pHitSeq->Add_Children(CAction::Create(bind(&CBT_Hili_Club::Hit, this)));

	CSequence* pActionSeq = CSequence::Create();
	pActionSeq->Add_Children(CAction::Create(bind(&CBT_Hili_Club::CheckAttack, this)));
	pActionSeq->Add_Children(CAction::Create(bind(&CBT_Hili_Club::CheckDiscoverToPlayer, this)));
	pActionSeq->Add_Children(CAction::Create(bind(&CBT_Hili_Club::CheckRangePlayer, this)));
	pActionSeq->Add_Children(CAction::Create(bind(&CBT_Hili_Club::CheckAttackTime, this)));
	pActionSeq->Add_Children(CAction::Create(bind(&CBT_Hili_Club::SelectAttack, this)));

	CSelector* pActionSeletor = CSelector::Create();
	pActionSeletor->Add_Children(CAction::Create(bind(&CBT_Hili_Club::Normal_Attack, this)));
	pActionSeletor->Add_Children(CAction::Create(bind(&CBT_Hili_Club::Thump_Attack, this)));
	pActionSeletor->Add_Children(CAction::Create(bind(&CBT_Hili_Club::Triple_Attack, this)));

	pActionSeq->Add_Children(pActionSeletor);

	CSequence* pDetectSeq = CSequence::Create();
	pDetectSeq->Add_Children(CAction::Create(bind(&CBT_Hili_Club::CheckDetect, this)));
	pDetectSeq->Add_Children(CAction::Create(bind(&CBT_Hili_Club::CheckLookPlayer, this)));
	pDetectSeq->Add_Children(CAction::Create(bind(&CBT_Hili_Club::MoveToPlayer, this)));

	CSequence* pPreSeq = CSequence::Create();
	pPreSeq->Add_Children(CAction::Create(bind(&CBT_Hili_Club::MoveToPrePlace, this)));
	pPreSeq->Add_Children(CAction::Create(bind(&CBT_Hili_Club::StandBy, this)));

	pSelector->Add_Children(pDeathSeq);
	pSelector->Add_Children(pTauntSeq);
	pSelector->Add_Children(pHitSeq);
	pSelector->Add_Children(pActionSeq);
	pSelector->Add_Children(pDetectSeq);
	pSelector->Add_Children(pPreSeq);

	m_pRootNode = pSelector;
}

CBT_Hili_Club* CBT_Hili_Club::Create(void* pArg)
{
	CBT_Hili_Club* pInstance = new CBT_Hili_Club();

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CBT_Hili_Club::Free()
{
	__super::Free();
}
