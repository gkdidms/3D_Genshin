#include "BT_Hili_CrossBow.h"

#include "GameInstance.h"
#include "Selector.h"
#include "Sequence.h"
#include "Action.h"

CBT_Hili_CrossBow::CBT_Hili_CrossBow()
	: CBT_Hili{}
{
}

HRESULT CBT_Hili_CrossBow::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_iAttackRange = { 10.f };
	m_iDetectRange = { 13.f };

	m_fAttackDelay = { 5.f };

	return S_OK;
}

void CBT_Hili_CrossBow::Tick(const _float& fTimeDelta)
{
	if (m_isAttack)
		m_fAttackTime += fTimeDelta;

	this->Evaluate();
}

CNode::NODE_STATE CBT_Hili_CrossBow::Evaluate()
{
	return m_pRootNode->Evaluate();
}

CNode::NODE_STATE CBT_Hili_CrossBow::CheckDeath()
{
	if (*m_pState != CHili::HILI_DIE)
		return CNode::SUCCESS;

	if (m_pModelCom->Get_Animation_Finished(43) || m_pModelCom->Get_Animation_Finished(44))
		return CNode::SUCCESS;

	return CNode::RUNNING;
}

CNode::NODE_STATE CBT_Hili_CrossBow::Death()
{
	return __super::Death();
}

CNode::NODE_STATE CBT_Hili_CrossBow::CheckHit()
{
	if (*m_pState != CHili::HILI_HIT)
		return CNode::SUCCESS;

	if (m_pModelCom->Get_Animation_Finished())
		return CNode::SUCCESS;

	return CNode::RUNNING;
}

CNode::NODE_STATE CBT_Hili_CrossBow::Hit()
{
	return __super::Hit();
}

CNode::NODE_STATE CBT_Hili_CrossBow::CheckAttack()
{
	if (*m_pState != CHili::HILI_NORMAL_ATK)
		return CNode::SUCCESS;

	if (m_pModelCom->Get_Animation_Finished())
		return CNode::SUCCESS;

	return CNode::RUNNING;
}

CNode::NODE_STATE CBT_Hili_CrossBow::CheckDiscoverToPlayer()
{
	return __super::CheckDiscoverToPlayer();
}

CNode::NODE_STATE CBT_Hili_CrossBow::CheckRangePlayer()
{
	return __super::CheckRangePlayer();
}

CNode::NODE_STATE CBT_Hili_CrossBow::CheckAttackTime()
{
	return __super::CheckAttackTime();
}

CNode::NODE_STATE CBT_Hili_CrossBow::Attack()
{
	return __super::Attack();
}

CNode::NODE_STATE CBT_Hili_CrossBow::CheckDetect()
{
	return __super::CheckDetect();
}

CNode::NODE_STATE CBT_Hili_CrossBow::CheckLookPlayer()
{
	return __super::CheckLookPlayer();
}

CNode::NODE_STATE CBT_Hili_CrossBow::MoveToPlayer()
{
	// 너무 가까우면 뒤로가고
	// 너무 멀리 있으면 앞으로 다가옴
	_vector vPrePos = XMLoadFloat4x4(&m_PreMatrix).r[3];
	_vector vCurrentPos = m_pTransformCom->Get_WorldMatrix().r[3];

	_vector vDistance = vPrePos - vCurrentPos;
	_float fDistance = sqrtf(XMVectorGetX(vDistance) * XMVectorGetX(vDistance) + XMVectorGetY(vDistance) * XMVectorGetY(vDistance) + XMVectorGetZ(vDistance) * XMVectorGetZ(vDistance));

	if (fDistance <= m_iDetectRange && fDistance > m_iAttackRange)
	{
		*m_pState = CHili::HILI_RUN;

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Hili_CrossBow::MoveToPrePlace()
{
	return __super::MoveToPrePlace();
}

CNode::NODE_STATE CBT_Hili_CrossBow::StandBy()
{
	return __super::StandBy();
}

void CBT_Hili_CrossBow::Ready_Node()
{
	//공격 확인
	CSelector* pSelector = CSelector::Create();

	CSequence* pDeathSeq = CSequence::Create();
	pDeathSeq->Add_Children(CAction::Create(bind(&CBT_Hili_CrossBow::CheckDeath, this)));
	pDeathSeq->Add_Children(CAction::Create(bind(&CBT_Hili_CrossBow::Death, this)));

	CSequence* pHitSeq = CSequence::Create();
	pHitSeq->Add_Children(CAction::Create(bind(&CBT_Hili_CrossBow::CheckHit, this)));
	pHitSeq->Add_Children(CAction::Create(bind(&CBT_Hili_CrossBow::Hit, this)));

	CSequence* pActionSeq = CSequence::Create();
	pActionSeq->Add_Children(CAction::Create(bind(&CBT_Hili_CrossBow::CheckAttack, this)));
	pActionSeq->Add_Children(CAction::Create(bind(&CBT_Hili_CrossBow::CheckDiscoverToPlayer, this)));
	pActionSeq->Add_Children(CAction::Create(bind(&CBT_Hili_CrossBow::CheckRangePlayer, this)));
	pActionSeq->Add_Children(CAction::Create(bind(&CBT_Hili_CrossBow::CheckAttackTime, this)));
	pActionSeq->Add_Children(CAction::Create(bind(&CBT_Hili_CrossBow::Attack, this)));

	CSequence* pDetectSeq = CSequence::Create();
	pDetectSeq->Add_Children(CAction::Create(bind(&CBT_Hili_CrossBow::CheckDetect, this)));
	pDetectSeq->Add_Children(CAction::Create(bind(&CBT_Hili_CrossBow::CheckLookPlayer, this)));
	pDetectSeq->Add_Children(CAction::Create(bind(&CBT_Hili_CrossBow::MoveToPlayer, this)));

	CSequence* pPreSeq = CSequence::Create();
	pPreSeq->Add_Children(CAction::Create(bind(&CBT_Hili_CrossBow::MoveToPrePlace, this)));
	pPreSeq->Add_Children(CAction::Create(bind(&CBT_Hili_CrossBow::StandBy, this)));

	pSelector->Add_Children(pDeathSeq);
	pSelector->Add_Children(pHitSeq);
	pSelector->Add_Children(pActionSeq);
	pSelector->Add_Children(pDetectSeq);
	pSelector->Add_Children(pPreSeq);

	m_pRootNode = pSelector;
}

CBT_Hili_CrossBow* CBT_Hili_CrossBow::Create(void* pArg)
{
	CBT_Hili_CrossBow* pInstance = new CBT_Hili_CrossBow();

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CBT_Hili_CrossBow::Free()
{
	__super::Free();
}
