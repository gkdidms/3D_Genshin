#pragma once
#include "Boss_Skill.h"

BEGIN(Client)
class CHarbinger_Blade_Range :
    public CBoss_Skill
{
private:
    CHarbinger_Blade_Range(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CHarbinger_Blade_Range(const CHarbinger_Blade_Range& rhs);
    virtual ~CHarbinger_Blade_Range() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    _uint m_iMaxNum = { 4 };
    _float m_fRange = { 3.f };

    _uint m_iCount = { 0 };

public:
    static CHarbinger_Blade_Range* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
