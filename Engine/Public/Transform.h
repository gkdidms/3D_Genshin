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
        _float fPower;
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

    const _float4x4* Get_WorldFloat4x4() { return &m_WorldMatrix; }

    _matrix Get_WorldMatrix() { return XMLoadFloat4x4(&m_WorldMatrix); }

public:
    void Set_WorldMatrix(_fmatrix WorldMatrix) { XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix); }

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
    _bool Go_Run(const _matrix vMoveMatrix, class CNavigation* pNavigationCom, _bool isFlyMove = false);
    _bool Check_Radius_AfterRun(const _matrix vMoveMatrix, class CNavigation* pNavigationCom, _vector vRadius);
    _bool Go_Coll(const _fvector vMovePos, class CNavigation* pNavigationCom);
    
    _bool Jump(const _float& fTimeDelta, _float fHeight, _float fAccelTime);
    void Go_Straight(const _float& fTimeDelta);
    void Go_Backwork(const _float& fTimeDelta);
    void Go_Left(const _float& fTimeDelta);
    void Go_Right(const _float& fTimeDelta);
    void LookAt(_fvector vTargetPosition);
    void LookForCamera(_fvector vCamLook, _float fRadian);
    void Turn(_fvector vAxis, _float fTimeDelta);
    void Rotation(_fvector vAxis, _float fRadian);
    void Rotation(_float fRotationX, _float fRotationY, _float fRotationZ);
    void Get_Rotation(_fvector vAxis, _float fRadian, _float4x4* TargetMatrix);
    void Billboard(_float4 CamPosition);

public:
    void Get_RootTransformationMatrix(_fmatrix RootMatrix);

private:
    _float4x4 m_WorldMatrix;
    _float m_fSpeedPerSec = { 0 };
    _float m_fRotationPerSec = { 0 };
    _float m_fPower = { 5.f };
    _float m_fAccelTime = { 0.f };

public:
    static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CComponent* Clone(void* pArg);
    virtual void Free() override;
};

END