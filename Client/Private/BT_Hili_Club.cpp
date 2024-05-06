#include "BT_Hili_Club.h"

#include "GameInstance.h"

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
	if (*m_pState != CHili::HILL_ATTACK)
		return CNode::SUCCESS;

	if (m_pModelCom->Get_Animation_Finished(14))
		return CNode::SUCCESS;

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

CNode::NODE_STATE CBT_Hili_Club::Attack()
{
	return __super::Attack();
}

CNode::NODE_STATE CBT_Hili_Club::CheckDetect()
{
	return __super::CheckDetect();
}

CNode::NODE_STATE CBT_Hili_Club::CheckLookPlayer()
{
	return __super::CheckLookPlayer();
}

CNode::NODE_STATE CBT_Hili_Club::MoveToPlayer()
{
	return __super::MoveToPlayer();
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

	CSequence* pHitSeq = CSequence::Create();
	pHitSeq->Add_Children(CAction::Create(bind(&CBT_Hili_Club::CheckHit, this)));
	pHitSeq->Add_Children(CAction::Create(bind(&CBT_Hili_Club::Hit, this)));

	CSequence* pActionSeq = CSequence::Create();
	pActionSeq->Add_Children(CAction::Create(bind(&CBT_Hili_Club::CheckAttack, this)));
	pActionSeq->Add_Children(CAction::Create(bind(&CBT_Hili_Club::CheckDiscoverToPlayer, this)));
	pActionSeq->Add_Children(CAction::Create(bind(&CBT_Hili_Club::CheckRangePlayer, this)));
	pActionSeq->Add_Children(CAction::Create(bind(&CBT_Hili_Club::CheckAttackTime, this)));
	pActionSeq->Add_Children(CAction::Create(bind(&CBT_Hili_Club::Attack, this)));

	CSequence* pDetectSeq = CSequence::Create();
	pDetectSeq->Add_Children(CAction::Create(bind(&CBT_Hili_Club::CheckDetect, this)));
	pDetectSeq->Add_Children(CAction::Create(bind(&CBT_Hili_Club::CheckLookPlayer, this)));
	pDetectSeq->Add_Children(CAction::Create(bind(&CBT_Hili_Club::MoveToPlayer, this)));

	CSequence* pPreSeq = CSequence::Create();
	pPreSeq->Add_Children(CAction::Create(bind(&CBT_Hili_Club::MoveToPrePlace, this)));
	pPreSeq->Add_Children(CAction::Create(bind(&CBT_Hili_Club::StandBy, this)));

	pSelector->Add_Children(pDeathSeq);
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
