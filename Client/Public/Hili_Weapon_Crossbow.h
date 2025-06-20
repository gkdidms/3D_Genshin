#pragma once
#include "Hili_Weapon.h"

class CHili_Weapon_Crossbow :
    public CHili_Weapon
{
private:
    CHili_Weapon_Crossbow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CHili_Weapon_Crossbow(const CHili_Weapon_Crossbow& rhs);
    virtual ~CHili_Weapon_Crossbow() = default;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(void* pArg) override;
    void Priority_Tick(const _float& fTimeDelta) override;
    void Tick(const _float& fTimeDelta) override;
    void Late_Tick(const _float& fTimeDelta) override;
    HRESULT Render() override;
    
private:
    _uint m_PreState;

private:
    virtual HRESULT Add_Components();
    HRESULT Create_Arrow();

public:
    static CHili_Weapon_Crossbow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};

