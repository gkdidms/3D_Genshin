#include "Sequence.h"

CSequence::CSequence()
    : CNode{}
{
}

CNode::NODE_STATE CSequence::Evaluate()
{
    // Failure 상태와 Running 상태일때 return
    // Sccess일때 계속 루프
    for (auto& pChildren : m_Childrens)
    {
        CNode::NODE_STATE eState = pChildren->Evaluate();

        if (eState == CNode::FAILURE || eState == CNode::RUNNING)
            return eState;
    }

    return CNode::SUCCESS;
}

void CSequence::Add_Children(CNode* pChildren)
{
    m_Childrens.emplace_back(pChildren);
}

CSequence* CSequence::Create()
{
    return new CSequence();
}

void CSequence::Free()
{
    __super::Free();

    for (auto& pChildren : m_Childrens)
        Safe_Release(pChildren);
}


