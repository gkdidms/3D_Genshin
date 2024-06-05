#pragma once
#include "GameObject.h"

#include "Client_Defines.h"

BEGIN(Engine)
class CNavigation;
class CCollider;
END

BEGIN(Client)
class CBoss :
    public CGameObject
{
public:
    enum PAGE_STATE { ONE_PAGE, TWO_PAGE, FINAL_PAGE, PAGE_END };
    enum PLAYER_PART { PART_BODY, PART_WEAPON, PART_WEAPON_BLADE_L, PART_WEAPON_BOW, PART_WEAPON_DUALBLADE, PART_END };
    enum BOSS_STATE {
        BOSS_IDLE,
        BOSS_BOW_IDLE,
        BOSS_BLADE_IDEL,
        BOSS_BOW_COMBO_ATTACK,
        BOSS_BOW_COVER_ATTACK,
        BOSS_BOW_COVER_ATTACK_BS,
        BOSS_BOW_COVER_ATTACK_AS,
        BOSS_BOW_COVER_ATTACK_LOOP,
        BOSS_BOW_NORMAL_ATTACK_BS, // BOSS_BOW_NORMAL_ATTACK
        BOSS_BOW_NORMAL_ATTACK_AS,
        BOSS_BOW_NORMAL_ATTACK_LOOP,
        BOSS_BOW_POWER_ATTACK,
        BOSS_BOW_POWER_ATTACK_BS,
        BOSS_BOW_POWER_ATTACK_AS,
        BOSS_BOW_POWER_ATTACK_LOOP,
        BOSS_BOW_RANGE_ATTACK_BS,
        BOSS_BOW_RANGE_ATTACK_AS,
        BOSS_BOW_RANGE_ATTACK_LOOP_1,
        BOSS_BOW_RANGE_ATTACK_LOOP_2,
        BOSS_RUSH_BS,
        BOSS_RUSH_AS,
        BOSS_DEFEND_1,
        BOSS_DEFEND_2,
        BOSS_DEFEND_3,
        BOSS_DEFEND_ATTACK_1,
        BOSS_DEFEND_ATTACK_2,
        BOSS_BLADE_EXTRA_ATTACK,
        BOSS_BLADE_RANGE_ATTACK,
        BOSS_BLADE_NORMAL_ATTACK_1, // BOSS_BLADE_NORMAL_ATTACK
        BOSS_BLADE_NORMAL_ATTACK_2,
        BOSS_BLADE_NORMAL_ATTACK_3,
        BOSS_BLADE_NORMAL_ATTACK_4,
        BOSS_DUALBLADE_CYCLE_ATTACK,
        BOSS_DUALBLADE_STRIKE_ATTACK,
        BOSS_DUALBLADE_STRIKE_ATTACK_BS,
        BOSS_DUALBLADE_STRIKE_ATTACK_AS,
        BOSS_DUALBLADE_STRIKE_ATTACK_LOOP,
        BOSS_DUALBLADE_SWEEP_ATTACK_L,
        BOSS_DUALBLADE_SWEEP_ATTACK_R,
        BOSS_DUALBLADE_HIRAISHIN_BS, // BOSS_DUALBLADE_HIRAISHIN
        BOSS_DUALBLADE_HIRAISHIN_AS,
        BOSS_DUALBLADE_HIRAISHIN_LOOP,
        BOSS_DUALBLADE_NORMAL_ATTACK_1,
        BOSS_DUALBLADE_NORMAL_ATTACK_2,
        BOSS_HARBINGER_NORMAL_ATTACK,
        BOSS_HIT_H,
        BOSS_HIT_L,
        BOSS_TURN_L,
        BOSS_TURN_R,
        BOSS_WALK_R,
        BOSS_WALK_L,
        BOSS_WALK_R_To_L,
        BOSS_WALK_L_TO_R,
        BOSS_MOVE_BACK,
        BOSS_MOVE_FORWARD,
        BOSS_DIE,
        BOSS_END
    };
    typedef struct tBossDesc : public CGameObject::GAMEOBJECT_DESC{
        _int iBossNavigationIndex;
        const _float4x4* TargetMatrix;
        _float4x4 WorldMatrix;
    } BOSS_DESC;

private:
    CBoss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CBoss(const CBoss& rhs);
    virtual ~CBoss() = default;

public:
    _uint Get_Page() { return m_CurrentPage; }

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    CCollider* m_pColliderCom = { nullptr };
    CNavigation* m_pNavigationCom = { nullptr };

    const _float4x4* m_pTargetMatrix;

    class CBT_Boss* m_pBT[PAGE_END];

private:
    vector<class CGameObject*> m_PartObject[PAGE_END];

    PAGE_STATE m_CurrentPage = { ONE_PAGE };
    _uint m_CurrentState = { BOSS_END };
    _int m_iBossNavigationIndex = { -1 };

    _bool m_isColl = { false };

private:
    HRESULT Add_Components();
    HRESULT Ready_Body();
    HRESULT Ready_Weapon();
    HRESULT Ready_Object();
    void Change_StateRank();
    void Check_Coll(const _float& fTimeDelta);

public:
    static CBoss* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free();
};
END
