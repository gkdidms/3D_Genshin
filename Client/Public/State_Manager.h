#pragma once
#include "Base.h"

#include "Client_Defines.h"
#include "Player.h"
#include "State.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)
class CState_Manager :
    public CBase
{
    DECLARE_SINGLETON(CState_Manager)

public:
    enum STATE_TYPE {
        STATE_TYPE_IDEL,
        STATE_TYPE_ATTACK,
        STATE_TYPE_ELEMENTALART,
        STATE_TYPE_ELEMNETALBURST,
        STATE_TYPE_RUN,
        STATE_TYPE_SPRINT,
        STATE_TYPE_END
    };

private:
    CState_Manager();
    virtual ~CState_Manager() = default;

public:
    PLAYER_STATE Set_CurrentState(STATE_TYPE Type, PLAYER_STATE CurrentState = PLAYER_END);
    void Set_CurrentPlayerble(CPlayer::PLAYERBLE_TYPE PlayerType);

public:
    PLAYER_STATE Enter(PLAYER_STATE CurrentState) { return m_pCurrentState->Enter(CurrentState); }
    PLAYER_STATE Update(const _float& fTimeDelta, PLAYER_STATE CurrentState) { return m_pCurrentState->Update(fTimeDelta, *this, CurrentState); }
    PLAYER_STATE Exit(PLAYER_STATE CurrentState) { return m_pCurrentState->Exit(*this, CurrentState); }

private:
    CGameInstance* m_pGameInstance = { nullptr };
    CState* m_pCurrentState = { nullptr };

    CPlayer::PLAYERBLE_TYPE m_CurrentPlayerble = { CPlayer::PLAYER_WANDERER };

public:
    virtual void Free() override;
};
END
