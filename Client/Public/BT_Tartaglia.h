#pragma once
#include "BT_Boss.h"

#include "Boss.h"
#include "Body_Tartaglia.h"

BEGIN(Engine)
class CGameInstance;
class CModel;
class CCollider;
class CTrasform;
END

BEGIN(Client)
class CBT_Tartaglia :
    public CBT_Boss
{
public:
    enum SKILL {
        SKILL_RUSH,
        SKILL_BOW_COMBO,
        SKILL_BOW_COVER,
        SKILL_BOW_NORMAL,
        SKILL_BOW_RENGE,
        SKILL_BOW_POWER,
        SKILL_BLADE_EXTRA,
        SKILL_BLADE_NORMAL,
        SKILL_END
    };

private:
    CBT_Tartaglia();
    virtual ~CBT_Tartaglia() = default;

public:
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Tick(const _float& fTimeDelta) override;
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

    //공격 selector
    CNode::NODE_STATE Rush_Move();
    CNode::NODE_STATE Bow_ComboAttack(); // 일반공격
    CNode::NODE_STATE Bow_CoverAttack(); // 
    CNode::NODE_STATE Bow_NormalAttack();
    CNode::NODE_STATE Bow_PowerAttack();
    CNode::NODE_STATE Bow_RangeAttack();
    CNode::NODE_STATE Blade_ExtraAttack();
    CNode::NODE_STATE Blade_NormalAttack();

    //걷기 Sequence
    CNode::NODE_STATE Check_Walk();
    CNode::NODE_STATE Walk();

    //standby
    CNode::NODE_STATE Standby();

public:
    static CBT_Tartaglia* Create(void* pArg);
    virtual void Free() override;
};
END
