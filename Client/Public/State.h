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
    virtual PLAYER_STATE Enter(class CState_Manager& pStateManager, PLAYER_STATE CurrentState) = 0; // ó�� ����
    virtual PLAYER_STATE Update(const _float& fTimeDelta, class CState_Manager& pStateManager, PLAYER_STATE CurrentState) = 0; // �� �����Ӹ��� ������Ʈ
    virtual PLAYER_STATE Exit(class CState_Manager& pStateManager, PLAYER_STATE CurrentState) = 0; // ���°� ��������

protected:
    CGameInstance* m_pGameInstance = { nullptr };

    PLAYER_STATE m_PlayerState = { PLAYER_END };

protected:
    virtual PLAYER_STATE ToIdle(class CState_Manager& pStateManager, PLAYER_STATE CurrentState);
    virtual PLAYER_STATE ToRun(class CState_Manager& pStateManager, PLAYER_STATE CurrentState);
    virtual PLAYER_STATE ToSprint(class CState_Manager& pStateManager, PLAYER_STATE CurrentState);
    virtual PLAYER_STATE ToJump(class CState_Manager& pStateManager, PLAYER_STATE CurrentState);
    virtual PLAYER_STATE ToFly(class CState_Manager& pStateManager, PLAYER_STATE CurrentState);
    virtual PLAYER_STATE ToAttack(class CState_Manager& pStateManager, PLAYER_STATE CurrentState);
    virtual PLAYER_STATE ToElementalArt(class CState_Manager& pStateManager, PLAYER_STATE CurrentState);
    virtual PLAYER_STATE ToElementalBurst(class CState_Manager& pStateManager, PLAYER_STATE CurrentState);

protected:
    _float m_fTime = { 0.f };
    _float m_fAttackTime = { 0.5f }; // ���� ��� ���ӽð� 

public:
    virtual void Free();
};
END
