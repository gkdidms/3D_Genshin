#pragma once
#include "PartObject_Body.h"
class CNilou_Body :
    public CPartObject_Body
{   
private:
    CNilou_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CNilou_Body(const CNilou_Body& rhs);
    virtual ~CNilou_Body() = default;

public:
    HRESULT Initialize_Prototype();
    HRESULT Initialize(void* pArg);
    void Priority_Tick(const _float& fTimeDelta);
    void Tick(const _float& fTimeDelta);
    void Late_Tick(const _float& fTimeDelta);
    HRESULT Render();

private:
    virtual HRESULT Add_Components() override;
    virtual HRESULT Bind_ResourceData() override;
    virtual void Change_Animation(const _float& fTimeDelta) override;

public:
    static CNilou_Body* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override; 
};

