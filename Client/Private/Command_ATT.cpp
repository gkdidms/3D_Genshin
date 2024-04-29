#include "Command_ATT.h"

CCommand_ATT::CCommand_ATT()
	: CCommand{}
{
}

void CCommand_ATT::Execute(_uint iIndex)
{
	m_iIndex = iIndex;
}

void CCommand_ATT::Undo()
{
}

CCommand_ATT* CCommand_ATT::Create()
{
	return new CCommand_ATT();
}

void CCommand_ATT::Free()
{
	__super::Free();
}
