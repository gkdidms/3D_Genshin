#pragma once
#include "Node.h"

#include "BT_Boss.h"

BEGIN(Client)
class CBT_EvilEye :
   public CBT_Boss
{
public:
	enum SKILL {
		SKILL_RUSH,
		SKILL_BLADE_EXTRA,
		SKILL_BLADE_NORMAL,
		SKILL_DUALBLADE_STRIKE,
		SKILL_DUALBLADE_SWEEP,
		SKILL_DUALBLADE_HIRAISHIN,
		SKILL_END
	};


private:
	CBT_EvilEye();
	virtual ~CBT_EvilEye() = default;

public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(const _float& fTimeDelta);
	virtual virtual CNode::NODE_STATE Evaluate() override;

private:
	SKILL m_Skill = { SKILL_END };
	CBT_Boss::RANGE_TYPE m_RangeType = { RANGE_END };

private:
	_float m_fNormalTime = { 3.f };
	_float m_fCurrentRunTime = { false };

	_float m_fMeleeAtkRange = { 3.f };

private:
	HRESULT Ready_Node();

private:

	CNode::NODE_STATE Death();

	CNode::NODE_STATE Check_Hit();
	CNode::NODE_STATE Check_Intersect();
	CNode::NODE_STATE Hit();

	//µðÆÒ½º
	CNode::NODE_STATE Check_Defend();
	CNode::NODE_STATE Defend();
	CNode::NODE_STATE Attack_Defend();

	//°ø°Ý sequence
	CNode::NODE_STATE Check_Attack_Deley();
	CNode::NODE_STATE Check_Attack_Range();
	CNode::NODE_STATE Check_Melee_Attack();
	CNode::NODE_STATE Check_Range_Attack();
	_bool Check_Rear_ToPlayer();
	//°ø°Ý selector
	CNode::NODE_STATE Rush_Move();
	CNode::NODE_STATE Blade_Extra_Attack();
	CNode::NODE_STATE Blade_Normal_Attack();
	CNode::NODE_STATE DualBlade_Strike();
	CNode::NODE_STATE DualBlade_Sweep();
	CNode::NODE_STATE DualBlade_Hiraishin();

	//°È±â Sequence
	CNode::NODE_STATE Check_Walk();
	CNode::NODE_STATE Walk();

	//standby
	CNode::NODE_STATE Standby();

public:
	static CBT_EvilEye* Create(void* pArg);
	virtual void Free() override;
};
END
