#pragma once
#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL CComponent abstract:
    public CBase
{
protected:
    CComponent(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CComponent(const CComponent& rhs);
    virtual ~CComponent() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick();

protected:
    ID3D11Device* m_pDevice = { nullptr };
    ID3D11DeviceContext* m_pContext = { nullptr };

public:
    virtual CComponent* Clone(void* pArg) = 0;
    virtual void Free();
};
END
