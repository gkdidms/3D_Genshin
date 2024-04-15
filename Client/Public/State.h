#pragma once
#include "Base.h"

#include "Client_Defines.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)
class CState abstract:
    public CBase
{
protected:
    CState();
    virtual ~CState() = default;

public:
    virtual PLAYER_STATE Enter() = 0; // 처음 진입
    virtual PLAYER_STATE Update(class CState_Manager& pStateManager, PLAYER_STATE CurrentState) = 0; // 매 프레임마다 업데이트
    virtual PLAYER_STATE Exit(class CState_Manager& pStateManager, PLAYER_STATE CurrentState) = 0; // 상태가 끝났을때

protected:
    CGameInstance* m_pGameInstance = { nullptr };

    PLAYER_STATE m_PlayerState = { PLAYER_END };

protected:
    virtual void Free();
};
END
