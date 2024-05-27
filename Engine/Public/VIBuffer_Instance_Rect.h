#pragma once
#include "VIBuffer_Instance.h"

BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Instance_Rect final:
    public CVIBuffer_Instance
{
private:
    CVIBuffer_Instance_Rect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CVIBuffer_Instance_Rect(const CVIBuffer_Instance_Rect& rhs);
    virtual ~CVIBuffer_Instance_Rect() = default;

public:
    virtual HRESULT Initialize_Prototype(const INSTANCE_DESC& InstanceDesc);
    virtual HRESULT Initialize(void* pArg) override;

public:
    static CVIBuffer_Instance_Rect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const INSTANCE_DESC& InstanceDesc);
    virtual CComponent* Clone(void* pArg);
    virtual void Free() override;
};

END