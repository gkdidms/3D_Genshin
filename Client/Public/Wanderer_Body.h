#pragma once
#include "PartObject_Body.h"

BEGIN(Client)
class CWanderer_Body :
    public CPartObject_Body
{   
private:
    CWanderer_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CWanderer_Body(const CWanderer_Body& rhs);
    virtual ~CWanderer_Body() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;


private:
    _float m_fAirSpeed = { 5.f };
    _float m_fAirDropSpeed = { 15.f };
    _float m_fAirStartTime = { 0.f };

private:
    virtual HRESULT Add_Components() override;
    virtual HRESULT Bind_ResourceData() override;
    virtual void Change_Animation(const _float& fTimeDelta) override;




public:
    static CWanderer_Body* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override; 
};
END
