#pragma once
#include "BT_Boss.h"

BEGIN(Client)
class CBT_Harbinger :
    public CBT_Boss
{
public:
    enum SKILL {
		SKILL_BOW_COVER, // 원거리 
		SKILL_BOW_NORMAL, // 원거리
		SKILL_BOW_POWER, // 원거리
		SKILL_BLADE_EXTRA, // 근거리
		SKILL_BLADE_NORMAL, // 근거리
		SKILL_BLADE_RANGE, // 원거리/근거리
		SKILL_DUALBLADE_NORMAL_1, // 원거리/근거리
		SKILL_DUALBLADE_NORMAL_2, // 원거리
		SKILL_DUALBLADE_CYCLE, // 원/근거리
		SKILL_DUALBLADE_STRIKE, // 근거리
		SKILL_DUALBLADE_SWEEP, // 근거리
		SKILL_DUALBLADE_HIRAISHIN, // 원거리
		SKILL_NORMAL, //근거리
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
	CBT_Boss::RANGE_TYPE m_RangeType = { RANGE_END };

private:
	_float m_fBowNormalTime = { 1.f };
	_float m_fSkritTime = { 1.f };
	_float m_fCurrentRunTime = { 0.f };

	//근거리 공격 범위
	_float m_fMeleeAtkRange = { 3.f };
	//원거리 공격 범위
	_float m_fRangedAtkRange = { 7.f };

	_uint m_iTeleportPer = { 30 };

private:
    HRESULT Ready_Node();

private:
	CNode::NODE_STATE Death();

	CNode::NODE_STATE Check_Hit();
	CNode::NODE_STATE Check_Intersect();
	CNode::NODE_STATE Hit();

	//공격 sequence
	CNode::NODE_STATE Check_Attack_Deley();
	CNode::NODE_STATE Check_Attack_Range(); // 근거리/ 원거리인지 체크
	CNode::NODE_STATE Check_MeleeAtk(); // 근거리 공격 중 어떤걸 선택할지
	CNode::NODE_STATE Check_RangeAtk(); // 원거리 공격 중 어떤걸 선택할지
	_bool Check_Rear_ToPlayer();

	//공격 selector (원거리/ 근거리 별로 나누기)
	CNode::NODE_STATE Bow_Cover_Attack();
	CNode::NODE_STATE Bow_Normal_Attack();
	CNode::NODE_STATE Bow_Power_Attack();
	CNode::NODE_STATE Blade_Range_Attack();
	CNode::NODE_STATE Blade_Normal_Attack();
	CNode::NODE_STATE Blade_Extra_Attack();
	CNode::NODE_STATE DualBlade_Cycle_Attack();
	CNode::NODE_STATE DualBlade_Normal_1();
	CNode::NODE_STATE DualBlade_Normal_2();
	CNode::NODE_STATE DualBlade_Strike();
	CNode::NODE_STATE DualBlade_Sweep();
	CNode::NODE_STATE DualBlade_Hiraishin();
	CNode::NODE_STATE Normal_Attack();  // Harbinger 전용

	//순간이동 Sequence
	CNode::NODE_STATE Check_Teleport();
	CNode::NODE_STATE Teleport();

	//standby
	CNode::NODE_STATE Standby();

public:
    static CBT_Harbinger* Create(void* pArg);
    virtual void Free();
};
END
