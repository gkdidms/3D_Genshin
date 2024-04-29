#pragma once

#include "Command.h"

class CCommand_ATT : public CCommand
{
private:
	CCommand_ATT();
	virtual ~CCommand_ATT() = default;

public:
	void Execute(_uint iIndex) override;
	void Undo() override;

private:
	_float m_iMaxAttCount = { 4.f };

public:
	static CCommand_ATT* Create();
	virtual void Free();
};

