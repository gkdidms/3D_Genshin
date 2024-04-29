#pragma once
#include "State.h"

BEGIN(Client)
class CStateIdle :
    public CState
{
private:
    CStateIdle();
    virtual ~CStateIdle() = default;

public:
    // CState��(��) ���� ��ӵ�
    virtual PLAYER_STATE Enter(class CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Update(const _float& fTimeDelta, class CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Exit(class CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;

public:
    static CStateIdle* Create();
    virtual void Free() override;
};

END