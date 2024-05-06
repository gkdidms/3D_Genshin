#pragma once
#include "Node.h"

#include "Boss.h"
#include "Body_Tartaglia.h"

BEGIN(Engine)
class CGameInstance;
class CModel;
class CCollider;
END

BEGIN(Client)
class CBT_Tartaglia :
    public CNode
{
public:
    typedef struct tBTTartagliaDesc {
        const CModel* pModel;
        CCollider* pCollider;
        CBoss::BOSS_STATE* pState;
        CBody_Tartaglia::TARTAGLIA_INFO* pInfo;
    }BT_TARTABGLIA_DESC;

    enum SKILL {
        SKILL_BOW_COMBO,
        SKILL_BOW_COVER,
        SKILL_BOW_NORMAL,
        SKILL_BOW_POWER,
        SKILL_BOW_RENGE,
        SKILL_BLADE_EXTRA,
        SKILL_BLADE_NORMAL,
        SKILL_END
    };

private:
    CBT_Tartaglia();
    virtual ~CBT_Tartaglia() = default;

public:
    HRESULT Initialize(void* pArg);
    void Tick(const _float& fTimeDelta);
    virtual CNode::NODE_STATE Evaluate() override;

private:
    CGameInstance* m_pGameInstance = { nullptr };
    CNode* m_pRootNode = { nullptr };
    CBoss::BOSS_STATE* m_pState = { nullptr };
    CBody_Tartaglia::TARTAGLIA_INFO* m_pInfo = { nullptr };
    const CModel* m_pModel;
    CCollider* m_pCollider = { nullptr };

private:
    SKILL m_Skill = { SKILL_END };

    _bool m_isAttack = { false };

    _uint m_iBladeNormalAttackMax = { 0 };
    _uint m_iBladeNormalAttackMaxCount = { 0 };

private:
    HRESULT Ready_Node();
    _uint Random(_uint iMaxNumber);

private:
    CNode::NODE_STATE Death();

    CNode::NODE_STATE Check_Hit();
    CNode::NODE_STATE Hit();

    //디팬스
    CNode::NODE_STATE Check_Defend();
    CNode::NODE_STATE Defend();
    CNode::NODE_STATE Attack_Defend();

    //공격 sequence
    CNode::NODE_STATE Check_Attack();
    CNode::NODE_STATE Check_Attack_Deley();

    //공격 selector
    CNode::NODE_STATE Bow_ComboAttack();
    CNode::NODE_STATE Bow_CoverAttack();
    CNode::NODE_STATE Bow_NormalAttack();
    CNode::NODE_STATE Bow_PowerAttack();
    CNode::NODE_STATE Bow_RangeAttack();
    CNode::NODE_STATE Blade_ExtraAttack();
    CNode::NODE_STATE Blade_NormalAttack();

    //standby
    CNode::NODE_STATE Standby();

public:
    static CBT_Tartaglia* Create(void* pArg);
    virtual void Free() override;
};
END
