#pragma once

#include "BT_Hili.h"

BEGIN(Client)
class CBT_Hili_CrossBow :
    public CBT_Hili
{
private:
	CBT_Hili_CrossBow();
	virtual ~CBT_Hili_CrossBow() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(const _float& fTimeDelta) override;
	virtual CNode::NODE_STATE Evaluate() override;

private:
	virtual CNode::NODE_STATE CheckDeath();
	virtual CNode::NODE_STATE Death();

	virtual CNode::NODE_STATE CheckHit();
	virtual CNode::NODE_STATE Hit();

	virtual CNode::NODE_STATE CheckAttack();
	virtual CNode::NODE_STATE CheckDiscoverToPlayer();
	virtual CNode::NODE_STATE CheckRangePlayer();
	virtual CNode::NODE_STATE CheckAttackTime();
	virtual CNode::NODE_STATE Attack();

	virtual CNode::NODE_STATE CheckDetect();
	virtual CNode::NODE_STATE CheckLookPlayer();
	virtual CNode::NODE_STATE MoveToPlayer();

	virtual CNode::NODE_STATE MoveToPrePlace();
	virtual CNode::NODE_STATE StandBy();

private:
	virtual void Ready_Node() override;

public:
	static CBT_Hili_CrossBow* Create(void* pArg);
	virtual void Free();
};

END