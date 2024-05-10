#pragma once
#include "Monster.h"

BEGIN(Client)
class CHili abstract:
    public CMonster
{
public:
    enum HILI_WEAPON_TYPE { HILI_WEAPON_CLUB, HILI_WEAPON_CROSSBOW, HILI_NON_WEAPON, HILI_WEAPON_END };

    enum HILI_STATE {
        HILI_IDLE,
        HILI_BORN,
        HILI_TAUNT_1,
        HILI_TAUNT_2,
        HILI_NORMAL_ATK,
        HILI_THUMP_ATK,
        HILI_TRIPLE_ATK,
        HILI_HIT,
        HILI_DIE,
        HILI_RUN,
        HILI_WALK_BACK,
        HILI_WALK_FORWARD,
        HILI_WALK_RIGHT,
        HILI_WALK_LEFT,
        HILI_END
    };

    typedef struct tHiliDesc : public CMonster::MONSTER_DESC {
        HILI_WEAPON_TYPE eWeapon;
    } HILI_DESC;

    typedef struct tHiliInfo {
        _float fMaxHp = { 24354.f };
        _float fHP = { 24354.f };
        ELEMENTAL_TYPE eElementalType = { ELEMENTAL_END };
    }HILI_INFO;


protected:
    CHili(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CHili(const CHili& rhs);
    virtual ~CHili() = default;
    
public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(void* pArg) override;
    void Priority_Tick(const _float& fTimeDelta) override;
    void Tick(const _float& fTimeDelta) override;
    void Late_Tick(const _float& fTimeDelta) override;
    HRESULT Render() override;

protected:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();
    virtual void Change_Animation(const _float& fTimeDelta);

    virtual HRESULT Ready_Object();

    virtual void Check_Coll();

protected:
    HILI_STATE m_CurrentState = { HILI_END };
    HILI_WEAPON_TYPE m_Weapon = { HILI_WEAPON_END };
    class CHili_Weapon* m_pWeapon = { nullptr };
    HILI_INFO m_Info;

    _bool m_isColl = { false };

protected:
    class CBT_Hili* m_pBT = { nullptr };
 
public:
    virtual void Free() override;
};
END
