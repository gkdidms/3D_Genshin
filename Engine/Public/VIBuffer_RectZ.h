#pragma once
#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CVIBuffer_RectZ :
    public CVIBuffer
{
private:
    CVIBuffer_RectZ(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CVIBuffer_RectZ(const CVIBuffer_RectZ& rhs);
    virtual ~CVIBuffer_RectZ() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;

public:
    static CVIBuffer_RectZ* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CComponent* Clone(void* pArg);
    virtual void Free() override;
};
END
