#pragma once
#include "Base.h"

BEGIN(Engine)
class CPipeLine final:
    public CBase
{
public:
    enum D3DTRANSFORMSTATE { D3DTS_VIEW, D3DTS_PROJ, D3DTS_END };

private:
    CPipeLine();
    virtual ~CPipeLine() = default;

public:
    const _float4x4* Get_Transform_Float4x4(D3DTRANSFORMSTATE eState);
    _matrix Get_Transform_Matrix(D3DTRANSFORMSTATE eState);
    const _float4x4* Get_Transform_Inverse_Float4x4(D3DTRANSFORMSTATE eState);
    _matrix Get_Transform_Inverse_Matrix(D3DTRANSFORMSTATE eState);
    const _float4* Get_CamPosition_Float4();
    _vector Get_CamPosition();
    const _float4* Get_ComLook_Float4();
    _vector Get_CamLook();
    const _float* Get_CamFar() {
        return &m_fFar;
    }

public:
    void Set_Transform(D3DTRANSFORMSTATE eState, _fmatrix matTransform) {
        XMStoreFloat4x4(&m_TransformStateMatrix[eState], matTransform);
    }

    void Set_CamFar(_float fFar) {
        m_fFar = fFar;
    }

public:
    HRESULT Initialize();
    void Tick();

private:
    _float4x4 m_TransformStateMatrix[D3DTS_END];
    _float4x4 m_TransformStateInverseMatrix[D3DTS_END];
    _float4 m_vCamPosition;
    _float4 m_vCamLook;
    _float m_fFar;

public:
    static CPipeLine* Create();
    virtual void Free();
};

END