#pragma once
#include "Base.h"

#include "Client_Defines.h"
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
        STATE_TYPE_RUN,
        STATE_TYPE_SPRINT,
        STATE_TYPE_END
    };
private:
    CState_Manager();
    virtual ~CState_Manager() = default;

public:
    PLAYER_STATE Set_CurrentState(STATE_TYPE Type);

public:
    const PLAYER_STATE* Get_CureentState() { return &m_PlayerState; }

public:
    HRESULT Initialize();

    PLAYER_STATE Enter() { return m_pCurrentState->Enter(); }
    PLAYER_STATE Update(PLAYER_STATE CurrentState) { return m_pCurrentState->Update(*this, CurrentState); }
    PLAYER_STATE Exit(PLAYER_STATE CurrentState) { return m_pCurrentState->Exit(*this, CurrentState); }

private:
    CGameInstance* m_pGameInstance = { nullptr };
    CState* m_pCurrentState = { nullptr };

    CState* m_States[STATE_TYPE_END];

    _uint m_iAnim = { 45 };
    _bool m_IsLoop = { false };

    PLAYER_STATE m_PlayerState = { PLAYER_END };

public:
    virtual void Free() override;
};
END
