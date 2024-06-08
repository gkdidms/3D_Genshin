#pragma once
#include "GameObject.h"

#include "Client_Defines.h"

BEGIN(Client)
class CBoss_Skill abstract:
    public CGameObject
{
public:
    typedef struct tBossSkillDesc {
        const _float4x4* pParentMatrix;
        const _float4x4* pTargetMatrix;
        _vector pParentLook;
    } BOSS_SKILL_DESC;

protected:
    CBoss_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CBoss_Skill() = default;
    
public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

protected:
    const _float4x4* m_pParentMatrix = { nullptr };
    const _float4x4* m_pTargetMatrix = { nullptr };
    _vector m_ParentLook = {};

protected:
    _float m_fDelayDuration = { 0.f };
    _float m_fDelayTime = { 0.f };

public:
    virtual void Free() override;
};
END
