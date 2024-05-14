#pragma once
#include "Weapon.h"

BEGIN(Client)
class CHarbinger_Bow :
    public CWeapon
{
private:
    CHarbinger_Bow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CHarbinger_Bow(const CHarbinger_Bow& rhs);
    virtual ~CHarbinger_Bow() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

public:
    _bool m_isFinal = { false };

private:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();
    virtual void Change_Animation(const _float& fTimeDelta);

public:
    static CHarbinger_Bow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
