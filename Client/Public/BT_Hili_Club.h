#pragma once
#include "BT_Hili.h"

BEGIN(Client)
class CBT_Hili_Club :
    public CBT_Hili
{
public:
	enum SKILL {
		HILI_NORMAL_ATK,
		HILI_THUMP_ATK,
		HILI_TRIPLE_ATK,
		SKILL_END
	};
private:
    CBT_Hili_Club();
    virtual ~CBT_Hili_Club() = default;

public:
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(const _float& fTimeDelta) override;
	virtual CNode::NODE_STATE Evaluate() override;

private:
	SKILL m_Skill = { SKILL_END };

private:
	virtual CNode::NODE_STATE CheckDeath() override;
	virtual CNode::NODE_STATE Death() override;

	virtual CNode::NODE_STATE CheckBone();
	virtual CNode::NODE_STATE CheckTaunt();
	virtual CNode::NODE_STATE Taunt();

	virtual CNode::NODE_STATE CheckHit() override;
	virtual CNode::NODE_STATE Hit() override;

	virtual CNode::NODE_STATE CheckAttack() override;
	virtual CNode::NODE_STATE CheckDiscoverToPlayer() override;
	virtual CNode::NODE_STATE CheckRangePlayer() override;
	virtual CNode::NODE_STATE CheckAttackTime() override;
	CNode::NODE_STATE SelectAttack();

	CNode::NODE_STATE Normal_Attack();
	CNode::NODE_STATE Thump_Attack();
	CNode::NODE_STATE Triple_Attack();

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