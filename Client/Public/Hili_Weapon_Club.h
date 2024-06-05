#pragma once
#include "Hili_Weapon.h"
class CHili_Weapon_Club :
    public CHili_Weapon
{
private:
    CHili_Weapon_Club(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CHili_Weapon_Club(const CHili_Weapon_Club& rhs);
    virtual ~CHili_Weapon_Club() = default;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(void* pArg) override;
    void Priority_Tick(const _float& fTimeDelta) override;
    void Tick(const _float& fTimeDelta) override;
    void Late_Tick(const _float& fTimeDelta) override;
    HRESULT Render() override;

private:
    virtual HRESULT Add_Components();
    void Coll_Player();

public:
    static CHili_Weapon_Club* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};

