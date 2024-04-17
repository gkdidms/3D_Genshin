#pragma once
#include "State.h"

#include "Client_Defines.h"

BEGIN(Client)
class CStateElementalArt_Wanderer :
    public CState
{
private:
    CStateElementalArt_Wanderer();
    virtual ~CStateElementalArt_Wanderer() = default;

public:
    PLAYER_STATE Enter(PLAYER_STATE CurrentState) override;
    PLAYER_STATE Update(const _float& fTimeDelta, CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;
    PLAYER_STATE Exit(CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;

private:
    _uint m_iAttackCount = { 0 };
    _uint m_iMaxAttack = { 3 };

    _float m_fCurrentTime = { 0.f }; // 총 스킬 작동 시간 
    _float m_fFinishTime = { 7.f }; // 스킬 지속시간

public:
    static CStateElementalArt_Wanderer* Create();
    virtual void Free() override;
};
END
