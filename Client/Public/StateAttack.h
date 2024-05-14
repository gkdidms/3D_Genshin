#pragma once
#include "State.h"

#include "Client_Defines.h"

BEGIN(Client)
class CStateAttack :
    public CState
{
private:
    CStateAttack();
    virtual ~CStateAttack() = default;

public:
    // CState을(를) 통해 상속됨
    virtual PLAYER_STATE Enter(class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Update(const _float& fTimeDelta, class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Exit(class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;

private:
    _uint m_iAttackCount = { 0 };
    _uint m_iMaxAttack = { 4 };

public:
    static CStateAttack* Create();
    virtual void Free() override;
};
END
