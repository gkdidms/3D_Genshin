#pragma once
#include "GameObject.h"

#include "Tool_Defines.h"

BEGIN(Engine)
class CCell;
END

BEGIN(Tool)
class CTool_Navigation :
    public CGameObject
{
private:
    CTool_Navigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CTool_Navigation(const CTool_Navigation& rhs);
    virtual ~CTool_Navigation() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Tick(const _float& fTimeDelta);
    virtual void Tick(const _float& fTimeDelta);
    virtual void Late_Tick(const _float& fTimeDelta);
    virtual HRESULT Render();

public:
    vector<CCell*> m_Cells;

public:
    static CTool_Navigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};

END