#pragma once
#include "Base.h"
#include "Client_Defines.h"

BEGIN(Client)
class CNode abstract:
    public CBase
{
public:
    enum NODE_STATE { FAILURE, RUNNING, SUCCESS, NODE_END };

protected:
    CNode();
    virtual ~CNode() = default;

public:
    virtual NODE_STATE Evaluate() = 0;

public:
    virtual void Free() override;
};

END