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
    HRESULT Initialize(_uint iMaxAttack);

public:
    // CState을(를) 통해 상속됨
    virtual PLAYER_STATE Enter(PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Update(const _float& fTimeDelta, class CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Exit(class CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;

private:
    _uint m_iAttackCount = { 0 };
    _uint m_iMaxAttack = { 4 };

public:
    static CStateAttack* Create(_uint iMaxAttack);
    virtual void Free() override;
};
END
