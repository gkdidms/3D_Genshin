#pragma once
#include "Base.h"

#include "Client_Defines.h"
#include "Player.h"
#include "State.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)
class CStateManager :
    public CBase
{
    DECLARE_SINGLETON(CStateManager)
public:
    enum STATE_TYPE {
        STATE_TYPE_IDEL,
        STATE_TYPE_ATTACK,
        STATE_TYPE_EXTRA_ATTACK,
        STATE_TYPE_ELEMENTALART,
        STATE_TYPE_ELEMNETALBURST,
        STATE_TYPE_RUN,
        STATE_TYPE_SPRINT,
        STATE_TYPE_JUMP,
        STATE_TYPE_FLY,
        STATE_TYPE_FALL_ATTACK,
        STATE_TYPE_FALL_GROUND,
        STATE_TYPE_HIT,
        STATE_TYPE_END
    };

    typedef struct tStateDesc{
        _uint* pPlayerbleType;
        _uint* pPlayerDir;
    } PLAYER_STATE_DESC;

private:
    CStateManager();
    virtual ~CStateManager() = default;

public:
    PLAYER_STATE Set_CurrentState(STATE_TYPE Type, PLAYER_STATE CurrentState = PLAYER_END);

public:
    _uint Get_Playerble() { return *m_CurrentPlayerble; }
    _bool isFly() { return m_StateType == STATE_TYPE_FLY; }
    _bool isJump() { return m_StateType == STATE_TYPE_JUMP; }
    _bool isDrop() { return m_StateType == STATE_TYPE_FALL_ATTACK; }
    _bool isAttack() { 
        return m_StateType == STATE_TYPE_ATTACK || m_StateType == STATE_TYPE_ELEMENTALART || m_StateType == STATE_TYPE_ELEMNETALBURST; }
    _bool isNormalAttack() { return m_StateType == STATE_TYPE_ATTACK; }
    _bool isElementalArt() { return m_StateType == STATE_TYPE_ELEMENTALART; }
    _bool isElementalBurst() { return m_StateType == STATE_TYPE_ELEMNETALBURST; }
    _bool isHit() { return m_StateType == STATE_TYPE_HIT; }

public:
    void Set_CollWindField(_bool isColl) { m_isCollWindField = isColl; }
    _bool isCollWindField() { return m_isCollWindField; }

public:
    HRESULT Initialize(void* pArg);

public:
    PLAYER_STATE Enter(PLAYER_STATE CurrentState) { return m_pCurrentState->Enter(*this, CurrentState); }
    PLAYER_STATE Update(const _float& fTimeDelta, PLAYER_STATE CurrentState) { return m_pCurrentState->Update(fTimeDelta, *this, CurrentState); }
    PLAYER_STATE Exit(PLAYER_STATE CurrentState) { return m_pCurrentState->Exit(*this, CurrentState); }

private:
    CGameInstance* m_pGameInstance = { nullptr };
    CState* m_pCurrentState = { nullptr };
    STATE_TYPE m_StateType = { STATE_TYPE_END };

    _uint* m_CurrentPlayerble = { nullptr };
    _uint* m_CurrentDir = { nullptr };

private:
    //vector<CState*> m_PlayerStates;

    _bool m_isCollWindField = { false };

private:
    void Change_Playerble();

public:
    virtual void Free() override;
};
END
