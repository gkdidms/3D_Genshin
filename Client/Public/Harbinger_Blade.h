#pragma once
#include "Weapon.h"

BEGIN(Client)
class CHarbinger_Blade :
    public CWeapon
{
private:
    CHarbinger_Blade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CHarbinger_Blade(const CHarbinger_Blade& rhs);
    virtual ~CHarbinger_Blade() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();
    virtual void Change_Animation(const _float& fTimeDelta);

public:
    static CHarbinger_Blade* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
