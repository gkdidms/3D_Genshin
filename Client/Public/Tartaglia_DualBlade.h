#pragma once
#include "Weapon.h"

BEGIN(Client)
class CTartaglia_DualBlade :
    public CWeapon
{
private:
    CTartaglia_DualBlade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CTartaglia_DualBlade(const CTartaglia_DualBlade& rhs);
    virtual ~CTartaglia_DualBlade() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    _uint m_iPreState = { 0 };

private:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();
    virtual void Change_Animation(const _float& fTimeDelta);

public:
    static CTartaglia_DualBlade* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
