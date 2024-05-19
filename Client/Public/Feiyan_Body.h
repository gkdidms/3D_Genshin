#pragma once
#include "PartObject_Body.h"

BEGIN(Client)
class CFeiyan_Body :
    public CPartObject_Body
{
private:
    CFeiyan_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CFeiyan_Body(const CFeiyan_Body& rhs);
    virtual ~CFeiyan_Body() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Tick(const _float& fTimeDelta);
    virtual void Tick(const _float& fTimeDelta);
    virtual void Late_Tick(const _float& fTimeDelta);
    virtual HRESULT Render();

private:
    virtual HRESULT Add_Components() ;
    virtual HRESULT Bind_ResourceData() ;
    virtual void Change_Animation(const _float& fTimeDelta);

public:
    static CFeiyan_Body* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};

END