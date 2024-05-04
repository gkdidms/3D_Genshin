#include "BT_Hili.h"

#include "GameInstance.h"

#include "Selector.h"
#include "Sequence.h"
#include "Action.h"

CBT_Hili::CBT_Hili()
	: CNode{},
	m_pGameInstance { CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CBT_Hili::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	BT_HILI_DESC* pDesc = static_cast<BT_HILI_DESC*>(pArg);

	m_pModelCom = pDesc->pModel;
	m_pTargetMatrix = pDesc->pTargetMatrix;
	m_pTransformCom = pDesc->pTransformCom;
	m_pState = pDesc->pState;
	m_pWeaponType = pDesc->pWeaponType;
	m_pColliderCom = pDesc->pCollider;
	m_pInfo = pDesc->pInfo;

	m_PreMatrix = *m_pTransformCom->Get_WorldFloat4x4();

	Ready_Node();

	return S_OK;
}

void CBT_Hili::Tick()
{
	this->Evaluate();
}

CNode::NODE_STATE CBT_Hili::Evaluate()
{
	return m_pRootNode->Evaluate();
}

void CBT_Hili::Ready_Node()
{
	//공격 확인
	CSelector* pSelector = CSelector::Create();

	CSequence* pDeathSeq = CSequence::Create();
	pDeathSeq->Add_Children(CAction::Create(bind(& CBT_Hili::CheckDeath, this)));
	pDeathSeq->Add_Children(CAction::Create(bind(&CBT_Hili::Death, this)));

	CSequence* pHitSeq = CSequence::Create();
	pHitSeq->Add_Children(CAction::Create(bind(&CBT_Hili::CheckHit, this)));
	pHitSeq->Add_Children(CAction::Create(bind(&CBT_Hili::Hit, this)));

	CSequence* pActionSeq = CSequence::Create();
	pActionSeq->Add_Children(CAction::Create(bind(&CBT_Hili::CheckAttack, this)));
	pActionSeq->Add_Children(CAction::Create(bind(&CBT_Hili::CheckDiscoverToPlayer, this)));
	pActionSeq->Add_Children(CAction::Create(bind(&CBT_Hili::CheckRangePlayer, this)));
	pActionSeq->Add_Children(CAction::Create(bind(&CBT_Hili::Attack, this)));

	CSequence* pDetectSeq = CSequence::Create();
	pDetectSeq->Add_Children(CAction::Create(bind(&CBT_Hili::CheckDetect, this)));
	pDetectSeq->Add_Children(CAction::Create(bind(&CBT_Hili::CheckLookPlayer, this)));
	pDetectSeq->Add_Children(CAction::Create(bind(&CBT_Hili::MoveCloserToPlayer, this)));

	CSequence* pPreSeq = CSequence::Create();
	pPreSeq->Add_Children(CAction::Create(bind(&CBT_Hili::MoveToPrePlace, this)));
	pPreSeq->Add_Children(CAction::Create(bind(&CBT_Hili::StandBy, this)));

	pSelector->Add_Children(pHitSeq);
	pSelector->Add_Children(pActionSeq);
	pSelector->Add_Children(pDetectSeq);
	pSelector->Add_Children(pPreSeq);

	m_pRootNode = pSelector;
}

CBT_Hili* CBT_Hili::Create(void* pArg)
{
	CBT_Hili* pInstance = new CBT_Hili();

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CBT_Hili::Free()
{
	__super::Free();

	Safe_Release(m_pRootNode);
	Safe_Release(m_pGameInstance);
}

CNode::NODE_STATE CBT_Hili::CheckDeath()
{
	if (*m_pState != CHili::HILI_DIE)
		return CNode::SUCCESS;

	if (m_pModelCom->Get_Animation_Finished())
		return CNode::SUCCESS;

	return CNode::RUNNING;
}

CNode::NODE_STATE CBT_Hili::Death()
{
	// 피가 다 떨어지면 죽음
	if (m_pInfo->fHP < 0.f)
	{
		*m_pState = CHili::HILI_DIE;
		return CNode::SUCCESS;
	}
		
	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Hili::CheckHit()
{
	if (*m_pState != CHili::HILI_HIT)
		return CNode::SUCCESS;

	if (m_pModelCom->Get_Animation_Finished(8) || m_pModelCom->Get_Animation_Finished(9))
		return CNode::SUCCESS;

	return CNode::RUNNING;
}

CNode::NODE_STATE CBT_Hili::Hit()
{
	//충돌하면 히트
	//*m_pState = CHili::HILL_HIT;
	CCollider* pTargetColl = dynamic_cast<CCollider*>(m_pGameInstance->Get_GameObject_Component(LEVEL_GAMEPLAY, L"Layer_Player", L"Com_Collider"));
	if (m_pColliderCom->Intersect(pTargetColl))
	{
		*m_pState = CHili::HILI_HIT;
		m_pInfo->fHP -= 10.f;

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Hili::CheckAttack()
{
	if (*m_pState != CHili::HILL_ATTACK)
		return CNode::SUCCESS;

	if (m_pModelCom->Get_Animation_Finished(14))
		return CNode::SUCCESS;

	return CNode::RUNNING;
}

CNode::NODE_STATE CBT_Hili::CheckDiscoverToPlayer()
{
	if (m_isDiscovered)
	{
		m_pTransformCom->LookAt(XMLoadFloat4x4(m_pTargetMatrix).r[3]);

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Hili::CheckRangePlayer()
{
	_vector vTargetPos = XMLoadFloat4x4(m_pTargetMatrix).r[3];
	_vector vHiliPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_vector vDistance = vTargetPos - vHiliPos;
	_float fDistance = sqrtf(XMVectorGetX(vDistance) * XMVectorGetX(vDistance) + XMVectorGetY(vDistance) * XMVectorGetY(vDistance) + XMVectorGetZ(vDistance) * XMVectorGetZ(vDistance));

	if (fDistance <= m_iAttackRange)
		return CNode::SUCCESS;

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Hili::Attack()
{
	*m_pState = CHili::HILL_ATTACK;

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_Hili::CheckDetect()
{
	_vector vTargetPos = XMLoadFloat4x4(m_pTargetMatrix).r[3];
	_vector vSlimePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_vector vDistance = vTargetPos - vSlimePos;
	_float fDistance = sqrtf(XMVectorGetX(vDistance) * XMVectorGetX(vDistance) + XMVectorGetY(vDistance) * XMVectorGetY(vDistance) + XMVectorGetZ(vDistance) * XMVectorGetZ(vDistance));

	if (fDistance <= m_iDetectRange)
		return CNode::SUCCESS;

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Hili::CheckLookPlayer()
{
	_float vLookAngle = { 100.f };

	_vector vTargetPos = XMLoadFloat4x4(m_pTargetMatrix).r[3];
	_vector vHiliPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector vHiliLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

	_vector vDesc = XMVector3Normalize(vTargetPos - vHiliPos);
	_float fDot = XMVectorGetX(XMVector3Dot(vDesc, vHiliLook));

	_float fAngle = XMConvertToDegrees(acosf(fDot));

	if (fAngle < vLookAngle * 0.5f)
	{
		m_isDiscovered = true;
		return CNode::SUCCESS;
	}


	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Hili::MoveCloserToPlayer() // 플레이어 가까이 다가감
{
	m_pTransformCom->LookAt(XMLoadFloat4x4(m_pTargetMatrix).r[3]);

	*m_pState = CHili::HILI_RUN;

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_Hili::MoveToPrePlace()
{
	// 원래 자리로 돌아감
	_vector vPrePos = XMLoadFloat4x4(&m_PreMatrix).r[3];
	_vector vCurrentPos = m_pTransformCom->Get_WorldMatrix().r[3];

	_vector vDistance = vPrePos - vCurrentPos;
	_float fDistance = sqrtf(XMVectorGetX(vDistance) * XMVectorGetX(vDistance) + XMVectorGetY(vDistance) * XMVectorGetY(vDistance) + XMVectorGetZ(vDistance) * XMVectorGetZ(vDistance));

	if (fDistance <= 0.3f)
		return CNode::SUCCESS;

	m_pTransformCom->LookAt(vPrePos);
	*m_pState = CHili::HILI_WALK_FORWARD;

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Hili::StandBy()
{
	*m_pState = CHili::HILI_IDLE;

	return CNode::SUCCESS;
}
