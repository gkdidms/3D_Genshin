#pragma once
#include "State.h"
class CStateIdle :
    public CState
{
private:
    CStateIdle();
    virtual ~CStateIdle() = default;

public:
    // CState��(��) ���� ��ӵ�
    virtual PLAYER_STATE Enter() override;
    virtual PLAYER_STATE Update(class CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Exit(class CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;

public:
    static CStateIdle* Create();
    virtual void Free() override;
};

