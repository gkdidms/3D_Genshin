#pragma once
#include "Node.h"
#include "Slime_Root.h"

BEGIN(Client)
class CAction :
    public CNode
{
private:
    CAction();
    virtual ~CAction() = default;

public:
    HRESULT Initialize(function<CNode::NODE_STATE()> Action);
    virtual NODE_STATE Evaluate();

private:
    function<CNode::NODE_STATE()> m_pAction = {nullptr};

public:
    static CAction* Create(function<CNode::NODE_STATE()> Action);
    virtual void Free() override;
};

END