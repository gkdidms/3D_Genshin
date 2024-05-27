#pragma once
#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Trail final:
    public CVIBuffer
{
public:
    typedef struct tVIBufferTrailDesc {
        _float fTimeAcc;
        _uint iMaxTrail;
        _float3 vInitPosA;
        _float3 vInitPosB;
    }VIBUFFER_TRAIL_DESC;

    typedef struct tTrailInfo {
        _float3 vPos[2];
    }TRAIL_INFO;

private:
    CVIBuffer_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CVIBuffer_Trail(const CVIBuffer_Trail& rhs);
    virtual ~CVIBuffer_Trail() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Render();

    void Add_Trail(const _float& fTimeDelta, const _matrix WorldMatrix);

private:
    _float m_fCurrentTime = { 0.f };
    _float m_fTimeAcc = { 0.f };
    _uint m_iMaxTrail = { 0 };
    _uint m_iCurrentTrail = { 0 };

    _uint m_iCurrentIndices = { 0 };

    _float3 m_vInitPosA;
    _float3 m_vInitPosB;

private:
    list<TRAIL_INFO> m_TrailInfos;

public:
    static CVIBuffer_Trail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CComponent* Clone(void* pArg);
    virtual void Free() override;
};
END
