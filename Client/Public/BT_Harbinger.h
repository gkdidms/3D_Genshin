#pragma once
#include "BT_Boss.h"

BEGIN(Client)
class CBT_Harbinger :
    public CBT_Boss
{
public:
    enum SKILL {
		SKILL_RUSH,
		SKILL_BOW_COMBO,
		SKILL_BOW_COVER,
		SKILL_BLADE_EXTRA,
		SKILL_BLADE_RANGE,
		SKILL_DUALBLADE_CYCLE,
		SKILL_DUALBLADE_STRIKE,
		SKILL_DUALBLADE_SWEEP,
		SKILL_DUALBLADE_HIRAISHIN,
		SKILL_NORMAL,
		SKILL_END
    };

private:
    CBT_Harbinger();
    virtual ~CBT_Harbinger() = default;

public:
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(const _float& fTimeDelta);
    virtual virtual CNode::NODE_STATE Evaluate() override;

private:
    SKILL m_Skill = { SKILL_END };

private:
    HRESULT Ready_Node();

private:
	CNode::NODE_STATE Death();

	CNode::NODE_STATE Check_Hit();
	CNode::NODE_STATE Check_Intersect();
	CNode::NODE_STATE Hit();

	//디팬스
	CNode::NODE_STATE Check_Defend();
	CNode::NODE_STATE Defend();
	CNode::NODE_STATE Attack_Defend();

	//공격 sequence
	CNode::NODE_STATE Check_Attack_Deley();
	_bool Check_Rear_ToPlayer();

	//공격 selector
	CNode::NODE_STATE Bow_Cover_Attack();
	CNode::NODE_STATE Bow_Power_Attack();
	CNode::NODE_STATE Blade_Range_Attack();
	CNode::NODE_STATE Blade_Extra_Attack();
	CNode::NODE_STATE DualBlade_Cycle_Attack();
	CNode::NODE_STATE DualBlade_Strike();
	CNode::NODE_STATE DualBlade_Sweep();
	CNode::NODE_STATE DualBlade_Hiraishin();
	CNode::NODE_STATE Normal_Attack();  // Harbinger 전용

	//걷기 Sequence
	CNode::NODE_STATE Check_Walk();
	CNode::NODE_STATE Walk();

	//standby
	CNode::NODE_STATE Standby();

public:
    static CBT_Harbinger* Create(void* pArg);
    virtual void Free();
};
END
