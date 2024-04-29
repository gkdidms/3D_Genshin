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
    virtual PLAYER_STATE Enter(class CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Update(const _float& fTimeDelta, class CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;
    virtual PLAYER_STATE Exit(class CState_Manager& pStateManager, PLAYER_STATE CurrentState) override;

private:
    _bool m_isFinished = { false };

    _uint m_iMaxSkill = { 3 };
    _float m_fFinishTime = { 10.f }; // 스킬 지속시간

    _uint m_iSkillCount = { 0 };
    _int m_iAttackCount = { -1 };
    _int m_iElementalArtCount = { -1 };
    _float m_iCurrentTime = { 0 };

private:
    void Reset();

public:
    static CStateElementalArt_Nilou* Create();
    virtual void Free() override;
};

END