#pragma once
#include "BT_Hili.h"

BEGIN(Client)
class CBT_Hili_Club :
    public CBT_Hili
{
private:
    CBT_Hili_Club();
    virtual ~CBT_Hili_Club() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(const _float& fTimeDelta) override;
	virtual CNode::NODE_STATE Evaluate() override;

private:
	virtual CNode::NODE_STATE CheckDeath() override;
	virtual CNode::NODE_STATE Death() override;

	virtual CNode::NODE_STATE CheckHit() override;
	virtual CNode::NODE_STATE Hit() override;

	virtual CNode::NODE_STATE CheckAttack() override;
	virtual CNode::NODE_STATE CheckDiscoverToPlayer() override;
	virtual CNode::NODE_STATE CheckRangePlayer() override;
	virtual CNode::NODE_STATE CheckAttackTime() override;
	virtual CNode::NODE_STATE Attack() override;

	virtual CNode::NODE_STATE CheckDetect() override;
	virtual CNode::NODE_STATE CheckLookPlayer() override;
	virtual CNode::NODE_STATE MoveToPlayer() override;

	virtual CNode::NODE_STATE MoveToPrePlace() override;
	virtual CNode::NODE_STATE StandBy() override;

private:
	virtual void Ready_Node() override;

public:
	static CBT_Hili_Club* Create(void* pArg);
	virtual void Free() override;
};

END