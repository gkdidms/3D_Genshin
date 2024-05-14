#pragma once
#include "State.h"

BEGIN(Client)
class CStateElementalArt_Yae :
    public CState
{
private:
    CStateElementalArt_Yae();
    virtual ~CStateElementalArt_Yae() = default;

public:
    virtual PLAYER_STATE Enter(class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Update(const _float& fTimeDelta, class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Exit(class CStateManager& pStateManager, PLAYER_STATE CurrentState) override;

private:
    _uint m_iSkillCount = { 0 };
    _uint m_iMaxSkill = { 2 };

    _float m_fEndTime = { 1.f };
public:
    static CStateElementalArt_Yae* Create();
    virtual void Free() override;
};

END