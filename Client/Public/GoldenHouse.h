#pragma once
#include "Map.h"

BEGIN(Client)
class CGoldenHouse :
    public CMap
{
private:
    CGoldenHouse(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CGoldenHouse(const CGoldenHouse& rhs);
    virtual ~CGoldenHouse() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    HRESULT Add_Components();
    HRESULT Bind_ResourceData();

public:
    static CGoldenHouse* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};

END