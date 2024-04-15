#pragma once
#include "PartObject_Weapon.h"
class CPartObject_Ayus :
    public CPartObject_Weapon
{
private:
    CPartObject_Ayus(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CPartObject_Ayus(const CPartObject_Ayus& rhs);
    virtual ~CPartObject_Ayus() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

public:
    virtual HRESULT Add_Components() override;
    virtual HRESULT Bind_ResourceData() override;
    virtual void Change_Animation(const _float& fTimeDelta) override;

public:
    static CPartObject_Ayus* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};

