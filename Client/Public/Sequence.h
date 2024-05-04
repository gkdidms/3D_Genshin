#pragma once
#include "Node.h"

BEGIN(Client)
class CSequence :
    public CNode
{
private:
    CSequence();
    virtual ~CSequence() = default;

public:
    virtual NODE_STATE Evaluate() override;
    void Add_Children(CNode* pChildren);

private:
    vector<CNode*> m_Childrens;

public:
    static CSequence* Create();
    virtual void Free() override;
};

END