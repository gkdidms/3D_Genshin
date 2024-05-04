#pragma once
#include "Node.h"

BEGIN(Client)
class CSelector :
    public CNode
{
private:
    CSelector();
    virtual ~CSelector() = default;

public:
    NODE_STATE Evaluate() override;
    void Add_Children(CNode* pChildren);

private:
    vector<CNode*> m_Children;

public:
    static CSelector* Create();
    virtual void Free();
};
END
