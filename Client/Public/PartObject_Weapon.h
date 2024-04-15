#pragma once
#include "PartObject.h"
class CPartObject_Weapon :
    public CPartObject
{
protected:
    CPartObject_Weapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CPartObject_Weapon(const CPartObject_Weapon& rhs);
    virtual ~CPartObject_Weapon() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Tick(const _float& fTimeDelta);
    virtual void Tick(const _float& fTimeDelta);
    virtual void Late_Tick(const _float& fTimeDelta);
    virtual HRESULT Render();

protected:
    virtual HRESULT Add_Components() = 0;
    virtual HRESULT Bind_ResourceData() = 0;
    virtual void Change_Animation(const _float& fTimeDelta) = 0;

protected:
    virtual CGameObject* Clone(void* pArg) = 0;
    virtual void Free();
};

