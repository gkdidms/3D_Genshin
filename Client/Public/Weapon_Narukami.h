#pragma once
#include "Weapon.h"

BEGIN(Client)
class CWeapon_Narukami :
    public CWeapon
{
private:
    CWeapon_Narukami(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CWeapon_Narukami(const CWeapon_Narukami& rhs);
    virtual ~CWeapon_Narukami() = default;

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
    static CWeapon_Narukami* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END