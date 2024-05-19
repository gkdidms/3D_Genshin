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
    // CState��(��) ���� ��ӵ�
    virtual PLAYER_STATE Enter(class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Update(const _float& fTimeDelta, class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Exit(class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;

private:
    static _uint m_iAttackCount;
    static _float m_iAttackTime;
    _uint m_iMaxAttack = { 0 };

public:
    static CStateAttack* Create();
    virtual void Free() override;
};
END
