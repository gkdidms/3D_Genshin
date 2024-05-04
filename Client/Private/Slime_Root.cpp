#include "Slime_Root.h"

#include "GameInstance.h"

#include "Sequence.h"
#include "Selector.h"
#include "Action.h"

CSlime_Root::CSlime_Root()
    : CNode{}
{
}

HRESULT CSlime_Root::Initialize(void* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;

    SLIME_LARGE_ROOT_DESC* pDesc = static_cast<SLIME_LARGE_ROOT_DESC*>(pArg);

    m_pState = pDesc->pState;
    m_pModelCom = pDesc->pModel;
    m_pTargetMatrix = pDesc->pTargetMatrix;
    m_pTransformCom = pDesc->pTransformCom;

    Ready_Node();

    return S_OK;
}

void CSlime_Root::Tick()
{
    this->Evaluate();
}

CNode::NODE_STATE CSlime_Root::Evaluate()
{
    return m_pNode->Evaluate();
}

void CSlime_Root::Ready_Node()
{
    //공격 확인
    CSelector* pSelector = CSelector::Create();

    CSequence* pHitSeq = CSequence::Create();
    pHitSeq->Add_Children(CAction::Create(bind(&CSlime_Root::CheckHit, this)));
    pHitSeq->Add_Children(CAction::Create(bind(&CSlime_Root::Hit, this)));

    CSelector* pActionSelector = CSelector::Create();
    pActionSelector->Add_Children(CAction::Create(bind(&CSlime_Root::AttackJump, this)));
    pActionSelector->Add_Children(CAction::Create(bind(&CSlime_Root::AttackNormal, this)));

    CSequence* pActionSeq = CSequence::Create();
    pActionSeq->Add_Children(CAction::Create(bind(&CSlime_Root::CheckAttack, this)));
    pActionSeq->Add_Children(CAction::Create(bind(&CSlime_Root::CheckLookPlayer, this)));
    pActionSeq->Add_Children(CAction::Create(bind(&CSlime_Root::CheckAttackRange, this)));
    pActionSeq->Add_Children(pActionSelector);

    CSequence* pDetectSeq = CSequence::Create();
    pDetectSeq->Add_Children(CAction::Create(bind(&CSlime_Root::CheckDetectPlayer, this)));
    pDetectSeq->Add_Children(CAction::Create(bind(&CSlime_Root::CheckMovePlayer, this)));
    pDetectSeq->Add_Children(CAction::Create(bind(&CSlime_Root::MoveCloserPlayer, this)));

    CSequence* pPreSeq = CSequence::Create();
    pPreSeq->Add_Children(CAction::Create(bind(&CSlime_Root::Undo, this)));

    pSelector->Add_Children(pHitSeq);
    pSelector->Add_Children(pActionSeq);
    pSelector->Add_Children(pDetectSeq);
    pSelector->Add_Children(pPreSeq);

    m_pNode = pSelector;
}

CNode::NODE_STATE CSlime_Root::CheckAttack()
{
    if (*m_pState != CSlime_Large::SLIME_LARGE_ATTACK_1 && *m_pState != CSlime_Large::SLIME_LARGE_ATTACK_2 && *m_pState != CSlime_Large::SLIME_LARGE_JUMP_ATK)
        return CNode::SUCCESS;

    if (m_pModelCom->Get_Animation_Finished(23) || m_pModelCom->Get_Animation_Finished(25) || m_pModelCom->Get_Animation_Finished(26))
    {
        return CNode::SUCCESS;
    }

    return CNode::RUNNING;
}

CNode::NODE_STATE CSlime_Root::CheckLookPlayer()
{
    _vector vTargetPos = XMLoadFloat4x4(m_pTargetMatrix).r[3];
    _vector vSlimePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

    _vector vDesc = XMVector3Normalize(vTargetPos - vSlimePos);
    _float fDot = XMVectorGetX(XMVector3Dot(vDesc, vSlimePos));

    _float fAngle = XMConvertToDegrees(acosf(fDot));

    if (fAngle < 60.f)
        return CNode::SUCCESS;

    return CNode::FAILURE;
}

CNode::NODE_STATE CSlime_Root::CheckAttackRange()
{
    _vector vTargetPos = XMLoadFloat4x4(m_pTargetMatrix).r[3];
    _vector vSlimePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

    _vector vDistance = vTargetPos - vSlimePos;
    _float fDistance = sqrtf(XMVectorGetX(vDistance) * XMVectorGetX(vDistance) + XMVectorGetY(vDistance) * XMVectorGetY(vDistance) + XMVectorGetZ(vDistance) * XMVectorGetZ(vDistance));

    if (fDistance <= m_iAttackRange)
        return CNode::SUCCESS;

    return CNode::FAILURE;
}

CNode::NODE_STATE CSlime_Root::AttackJump()
{
    *m_pState = CSlime_Large::SLIME_LARGE_JUMP_ATK;

    return CNode::SUCCESS;
}

CNode::NODE_STATE CSlime_Root::AttackNormal()
{
    *m_pState = CSlime_Large::SLIME_LARGE_ATTACK_1;

    return CNode::SUCCESS;
}

CNode::NODE_STATE CSlime_Root::CheckDetectPlayer()
{
    _vector vTargetPos = XMLoadFloat4x4(m_pTargetMatrix).r[3];
    _vector vSlimePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

    _vector vDistance = vTargetPos - vSlimePos;
    _float fDistance = sqrtf(XMVectorGetX(vDistance) * XMVectorGetX(vDistance) + XMVectorGetY(vDistance) * XMVectorGetY(vDistance) + XMVectorGetZ(vDistance) * XMVectorGetZ(vDistance));

    if (fDistance <= m_iDetectRange)
        return CNode::SUCCESS;

    return CNode::FAILURE;
}

CNode::NODE_STATE CSlime_Root::CheckMovePlayer()
{
    if (*m_pState != CSlime_Large::SLIME_LARGE_JUMP)
        return CNode::SUCCESS;

    if (m_pModelCom->Get_Animation_Finished(21))
        return CNode::SUCCESS;

    return CNode::RUNNING;
}

CNode::NODE_STATE CSlime_Root::MoveCloserPlayer()
{
    *m_pState = CSlime_Large::SLIME_LARGE_JUMP;

    return CNode::SUCCESS;
}

CNode::NODE_STATE CSlime_Root::CheckHit()
{
    if (*m_pState != CSlime_Large::SLIME_LARGE_HIT_H && *m_pState != CSlime_Large::SLIME_LARGE_HIT_L)
        return CNode::SUCCESS;

    if (m_pModelCom->Get_Animation_Finished(13))
        return CNode::SUCCESS;

    return CNode::RUNNING;
}

CNode::NODE_STATE CSlime_Root::Hit()
{
    return CNode::FAILURE;
}

CNode::NODE_STATE CSlime_Root::Undo()
{
    *m_pState = CSlime_Large::SLIME_LARGE_JUMP;

    return CNode::SUCCESS;
}


CSlime_Root* CSlime_Root::Create(void* pArg)
{
    CSlime_Root* pInstance = new CSlime_Root();

    if (FAILED(pInstance->Initialize(pArg)))
        Safe_Release(pInstance);

    return pInstance;
}

void CSlime_Root::Free()
{
    __super::Free();

    Safe_Release(m_pNode);
}
