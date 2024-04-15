#pragma once
#include "State.h"
class CStateAttack_Wanderer :
    public CState
{
private:
    CStateAttack_Wanderer();
    virtual ~CStateAttack_Wanderer() = default;

public:
    // CState을(를) 통해 상속됨
    virtual PLAYER_STATE Enter() override;
    virtual PLAYER_STATE Update(class CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Exit(class CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;

private:
    _uint m_iAttackCount = { 0 };
    _uint m_iMaxAttack = { 3 };

public:
    static CStateAttack_Wanderer* Create();
    virtual void Free() override;
};

