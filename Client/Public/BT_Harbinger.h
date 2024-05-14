#pragma once
#include "BT_Boss.h"

BEGIN(Client)
class CBT_Harbinger :
    public CBT_Boss
{
public:
    enum SKILL {
		SKILL_BOW_COVER, // ���Ÿ� 
		SKILL_BOW_NORMAL, // ���Ÿ�
		SKILL_BOW_POWER, // ���Ÿ�
		SKILL_BLADE_EXTRA, // �ٰŸ�
		SKILL_BLADE_NORMAL, // �ٰŸ�
		SKILL_BLADE_RANGE, // ���Ÿ�/�ٰŸ�
		SKILL_DUALBLADE_NORMAL_1, // ���Ÿ�/�ٰŸ�
		SKILL_DUALBLADE_NORMAL_2, // ���Ÿ�
		SKILL_DUALBLADE_CYCLE, // ��/�ٰŸ�
		SKILL_DUALBLADE_STRIKE, // �ٰŸ�
		SKILL_DUALBLADE_SWEEP, // �ٰŸ�
		SKILL_DUALBLADE_HIRAISHIN, // ���Ÿ�
		SKILL_NORMAL, //�ٰŸ�
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

	//�ٰŸ� ���� ����
	_float m_fMeleeAtkRange = { 3.f };
	//���Ÿ� ���� ����
	_float m_fRangedAtkRange = { 7.f };

	_uint m_iTeleportPer = { 30 };

private:
    HRESULT Ready_Node();

private:
	CNode::NODE_STATE Death();

	CNode::NODE_STATE Check_Hit();
	CNode::NODE_STATE Check_Intersect();
	CNode::NODE_STATE Hit();

	//���� sequence
	CNode::NODE_STATE Check_Attack_Deley();
	CNode::NODE_STATE Check_Attack_Range(); // �ٰŸ�/ ���Ÿ����� üũ
	CNode::NODE_STATE Check_MeleeAtk(); // �ٰŸ� ���� �� ��� ��������
	CNode::NODE_STATE Check_RangeAtk(); // ���Ÿ� ���� �� ��� ��������
	_bool Check_Rear_ToPlayer();

	//���� selector (���Ÿ�/ �ٰŸ� ���� ������)
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
	CNode::NODE_STATE Normal_Attack();  // Harbinger ����

	//�����̵� Sequence
	CNode::NODE_STATE Check_Teleport();
	CNode::NODE_STATE Teleport();

	//standby
	CNode::NODE_STATE Standby();

public:
    static CBT_Harbinger* Create(void* pArg);
    virtual void Free();
};
END
