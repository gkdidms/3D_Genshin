#pragma once
#include "Weapon.h"

BEGIN(Engine)
class CCollider;
END
BEGIN(Client)
class CWeapon_Regalis :
    public CWeapon
{
private:
    CWeapon_Regalis(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CWeapon_Regalis(const CWeapon_Regalis& rhs);
    virtual ~CWeapon_Regalis() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    _uint m_iPreState = {0};

    _float m_fCurrentTime = { 0.f };
    _bool m_isCreated = { false };

private:
    virtual HRESULT Add_Components() override;
    virtual HRESULT Bind_ResourceData() override;
    virtual void Change_Animation(const _float& fTimeDelta) override;
    HRESULT Create_TrailBuffer(const _float& fTimeDelta);

public:
    static CWeapon_Regalis* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END