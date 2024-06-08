#pragma once
#include "Boss_Body.h"
class CBody_EvilEye : // 번개 원소
    public CBoss_Body
{
private:
    CBody_EvilEye(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CBody_EvilEye(const CBody_EvilEye& rhs);
    virtual ~CBody_EvilEye() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    _float4x4 m_MoveToTargetMatrix = {};

    _float m_fDuration = { 1.f };
    _float m_fCurrentTime = { 0.f };

    _float m_fAtkDelayDuration = { 0.3f };
    _float m_fAtkCurrentTime = { 0.f };


private:
    virtual HRESULT Add_Components() override;
    virtual HRESULT Bind_ResourceData() override;
    virtual void Change_Animation();

public:
    static CBody_EvilEye* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};

