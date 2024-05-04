#include "Selector.h"

CSelector::CSelector()
	: CNode{}
{
}

CNode::NODE_STATE CSelector::Evaluate()
{
	for (auto& pChildren : m_Children)
	{
		CNode::NODE_STATE eResult = pChildren->Evaluate();
		
		if (eResult == CNode::SUCCESS || eResult == CNode::RUNNING)
			return eResult;
	}

	return CNode::FAILURE;
}

void CSelector::Add_Children(CNode* pChildren)
{
	m_Children.emplace_back(pChildren);
}

CSelector* CSelector::Create()
{
	return new CSelector();
}

void CSelector::Free()
{
	__super::Free();

	for (auto& pChildren : m_Children)
		Safe_Release(pChildren);
}
