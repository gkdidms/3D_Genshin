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
    enum PLAYER_PART { PART_BODY, PART_WEAPON, PART_WEAPON_SUB, PART_END };
    enum BOSS_STATE {
        BOSS_BOW_IDLE,
        BOSS_BLADE_IDEL,
        BOSS_BOW_COMBO_ATTACK,
        BOSS_BOW_COVER_ATTACK_BS,
        BOSS_BOW_COVER_ATTACK_AS,
        BOSS_BOW_COVER_ATTACK_LOOP,
        BOSS_BOW_NORMAL_ATTACK_BS,
        BOSS_BOW_NORMAL_ATTACK_AS,
        BOSS_BOW_NORMAL_ATTACK_LOOP,
        BOSS_BOW_POWER_ATTACK_BS,
        BOSS_BOW_POWER_ATTACK_AS,
        BOSS_BOW_POWER_ATTACK_LOOP,
        BOSS_BOW_RANGE_ATTACK_BS,
        BOSS_BOW_RANGE_ATTACK_AS,
        BOSS_BOW_RANGE_ATTACK_LOOP_1,
        BOSS_BOW_RANGE_ATTACK_LOOP_2,
        BOSS_DEFEND_1,
        BOSS_DEFEND_2,
        BOSS_DEFEND_3,
        BOSS_DEFEND_ATTACK_1,
        BOSS_DEFEND_ATTACK_2,
        BOSS_BLADE_EXTRA_ATTACK,
        BOSS_BLADE_NORMAL_ATTACK_1,
        BOSS_BLADE_NORMAL_ATTACK_2,
        BOSS_BLADE_NORMAL_ATTACK_3,
        BOSS_BLADE_NORMAL_ATTACK_4,
        BOSS_HIT_H,
        BOSS_HIT_L,
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
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    CNavigation* m_pNavigationCom = { nullptr };

private:
    vector<class CGameObject*> m_PartObject[PAGE_END];

    PAGE_STATE m_CurrentPage = { ONE_PAGE };
    _uint m_CurrentState = { BOSS_END };
    _int m_iBossNavigationIndex = { -1 };


private:
    HRESULT Add_Components();
    HRESULT Ready_Body();
    HRESULT Ready_Weapon();

public:
    static CBoss* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free();
};
END
