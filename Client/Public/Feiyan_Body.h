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

    HRESULT Create_Bullet(const wstring& strLayerTag);

public:
    static CFeiyan_Body* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};

END