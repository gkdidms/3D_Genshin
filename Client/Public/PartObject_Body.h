#pragma once
#include "PartObject.h"

BEGIN(Client)
class CPartObject_Body abstract:
    public CPartObject
{
protected:
    CPartObject_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CPartObject_Body(const CPartObject_Body& rhs);
    virtual ~CPartObject_Body() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Tick(const _float& fTimeDelta);
    virtual void Tick(const _float& fTimeDelta);
    virtual void Late_Tick(const _float& fTimeDelta);
    virtual HRESULT Render();

protected:
    _bool m_IsFinished = { false };

    _float m_fRunSpeed = { 30.f };

protected:
    virtual HRESULT Add_Components() = 0;
    virtual HRESULT Bind_ResourceData() = 0;
    virtual void Change_Animation(const _float& fTimeDelta) = 0;

protected:
    virtual void Free() override;
};
END
