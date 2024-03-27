#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Terrain final:
    public CVIBuffer
{
private:
    CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CVIBuffer_Terrain(const CVIBuffer_Terrain& rhs);
    virtual ~CVIBuffer_Terrain() = default;

public:
    _float3* Get_VtxPos() { return m_pVtxPos; }
    _uint Get_Width() { return m_iNumVerticesX; }
    _uint Get_Height() { return m_iNumVerticesZ; }

public:
    virtual HRESULT Initialize_Prototype(const wstring strTerrainFilePath);
    virtual HRESULT Initialize(void* pArg) override;

private:
    _uint m_iNumVerticesX = { 0 };
    _uint m_iNumVerticesZ = { 0 };

    _float3* m_pVtxPos;

public:
    static CVIBuffer_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring strTerrainFilePath);
    virtual CComponent* Clone(void* pArg) override;
    virtual void Free() override;
};

END