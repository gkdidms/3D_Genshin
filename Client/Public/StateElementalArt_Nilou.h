#pragma once
#include "State.h"

BEGIN(Client)
class CStateElementalArt_Nilou :
    public CState
{
private:
    CStateElementalArt_Nilou();
    virtual ~CStateElementalArt_Nilou() = default;

public:
    virtual PLAYER_STATE Enter(PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Update(const _float& fTimeDelta, class CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Exit(class CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;

private:
    _uint m_iSkillCount = { 0 };
    _uint m_iMaxSkill = { 4 };

    _uint m_iAttackCount = { 0 };
    _uint m_iElementalArtCount = { 0 };

public:
    static CStateElementalArt_Nilou* Create();
    virtual void Free() override;
};

END