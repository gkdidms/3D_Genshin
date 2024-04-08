#pragma once
#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CTransform final:
    public CComponent
{
public:
    enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

    typedef struct
    {
        _float fSpeedPecSec;
        _float fRotatePecSec;
    }TRANSFORM_DESC;

private:
    CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CTransform(const CTransform& rhs);
    virtual ~CTransform() = default;

public:
    _vector Get_State(STATE eState);
    _float3 Get_Scaled() {
        return _float3(XMVector3Length(Get_State(STATE_RIGHT)).m128_f32[0],
            XMVector3Length(Get_State(STATE_UP)).m128_f32[0],
            XMVector3Length(Get_State(STATE_LOOK)).m128_f32[0]);
    }

    const _float4x4 Get_WorldFloat4x4() { return m_matWorld; }

    _matrix Get_WorldMatrix() { return XMLoadFloat4x4(&m_matWorld); }

public:
    void Set_WorldMatrix(_fmatrix WorldMatrix) { XMStoreFloat4x4(&m_matWorld, WorldMatrix); }

public:
    void Set_State(STATE eState, _fvector vState);

    void Scaling(_float fScaleX, _float fScaleY, _float fScaleZ);
    void Set_Scale(_float fScaleX, _float fScaleY, _float fScaleZ);

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;

public:
    HRESULT Bind_ShaderMatrix(class CShader* pShader, const _char* strConstantName);

public:
    void Go_Straight(const _float& fTimeDelta);
    void Go_Backwork(const _float& fTimeDelta);
    void Go_Left(const _float& fTimeDelta);
    void Go_Right(const _float& fTimeDelta);
    void LookAt(_fvector vTargetPosition);
    void Turn(_fvector vAxis, _float fTimeDelta);
    void Rotation(_fvector vAxis, _float fRadian);
    void Rotation(_float fRotationX, _float fRotationY, _float fRotationZ);

private:
    _float4x4 m_matWorld;
    _float m_fSpeedPerSec = { 0 };
    _float m_fRotationPerSec = { 0 };

public:
    static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CComponent* Clone(void* pArg);
    virtual void Free() override;
};

END