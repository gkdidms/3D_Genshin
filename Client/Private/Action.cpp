#include "Action.h"

CAction::CAction()
	: CNode{}
{
}

HRESULT CAction::Initialize(function<CNode::NODE_STATE()> Action)
{
	m_pAction = Action;

	return S_OK;
}

CNode::NODE_STATE CAction::Evaluate()
{
	return m_pAction();
}

CAction* CAction::Create(function<CNode::NODE_STATE()> Action)
{
	CAction* pInstance = new CAction();

	if (FAILED(pInstance->Initialize(Action)))
		Safe_Release(pInstance);

	return pInstance;
}

void CAction::Free()
{
	__super::Free();
}
