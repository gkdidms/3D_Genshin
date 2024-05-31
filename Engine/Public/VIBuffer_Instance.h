#pragma once
#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Instance abstract :
    public CVIBuffer
{
public:
    typedef struct tVIBufferInstanceDesc
    {
        _uint iNumInstance;
        _float3 vOffsetPos;
        _float3 vPivotPos;
        _float3 vRange;
        _float2 vSize;
        _float2 vSpeed;
        _float2 vLifeTime;
        _float2 vPower;
        _bool isLoop;
    }INSTANCE_DESC;

protected:
    CVIBuffer_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CVIBuffer_Instance(const CVIBuffer_Instance& rhs);
    virtual ~CVIBuffer_Instance() = default;

public:
    virtual HRESULT Initialize_Prototype(const INSTANCE_DESC& InstanceDesc);
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Render() override;

protected:
    ID3D11Buffer* m_pVBInstance = { nullptr };
    D3D11_BUFFER_DESC	m_InstanceBufferDesc = {};
    INSTANCE_DESC m_InstanceDesc = {};

    _uint m_iNumInstance = { 0 };
    _uint m_iIndexCountPerInstance = { 0 };
    _uint m_iInstanceStride = { 0 };

    _float* m_pSpeeds = { nullptr };
    _float* m_pPower = { nullptr };
    _float3* m_pOriginalPositions = { nullptr };

    _float m_fAccelTime = { 0.f };

public:
    void Spread(_float fTimeDelta);
    void Drop(_float fTimeDelta);
    void Helix(_float fTimeDelta);
    
protected:
    random_device				m_RandomDevice;
    mt19937_64					m_RandomNumber;

public:
    virtual CComponent* Clone(void* pArg) = 0;
    virtual void Free() override;
};
END
