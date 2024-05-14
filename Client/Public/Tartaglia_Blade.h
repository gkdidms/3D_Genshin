#pragma once
#include "Weapon.h"

BEGIN(Client)
class CTartaglia_Blade :
    public CWeapon
{
public:
    typedef struct tTartagliaBladeDesc : public CWeapon::WEAPON_DESC
    {
        ELEMENTAL_TYPE eElementalType;
    } TARTAGLIA_BLADE_DESC;
private:
    CTartaglia_Blade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CTartaglia_Blade(const CTartaglia_Blade& rhs);
    virtual ~CTartaglia_Blade() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

public:
    ELEMENTAL_TYPE m_ElementalType = { ELEMENTAL_END };

private:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();
    virtual void Change_Animation(const _float& fTimeDelta);

public:
    static CTartaglia_Blade* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
