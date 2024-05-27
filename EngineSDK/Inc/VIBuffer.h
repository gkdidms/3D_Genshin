#pragma once
#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CVIBuffer :
    public CComponent
{
protected:
    CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CVIBuffer(const CVIBuffer& rhs);
    virtual ~CVIBuffer() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Render();

public:
    HRESULT Create_Buffer(ID3D11Buffer** pOut);

protected:
    ID3D11Buffer* m_pVB;
    ID3D11Buffer* m_pIB;

protected:
    D3D11_BUFFER_DESC m_Buffer_Desc = {};
    D3D11_SUBRESOURCE_DATA m_ResourceData = {};

    _uint m_iNumVertexBuffers = { 0 };
    _uint m_iVertexStride = { 0 };
    _uint m_iNumVertices = { 0 };

    _uint m_iIndexStride = { 0 };
    _uint m_iNumIndices = { 0 };

    DXGI_FORMAT m_GIFormat = {};
    D3D11_PRIMITIVE_TOPOLOGY m_Primitive_Topology = {};

public:
    virtual void Free() override;
};
END
