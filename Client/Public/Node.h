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

protected:
    _uint Random(_uint iMaxNumber);
    _float Distance(_matrix TargetMatrix, _matrix FocusMatrix);
    _bool AngleOfView(_float fLookAngle, _fvector vTargetPos, _fvector vCurrentPos, _fvector vLook);

public:
    virtual void Free() override;
};

END