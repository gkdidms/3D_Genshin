#pragma once
#include "GameObject.h"

BEGIN(Engine)
class ENGINE_DLL CCamera abstract:
    public CGameObject
{
public:
    typedef struct tCameraDesc : public GAMEOBJECT_DESC
    {
        _float4 vEye, vFocus;
        _float fFovY, fAspect, fNear, fFar;

    }CAMERA_DESC;

protected:
    CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CCamera(const CCamera& rhs);
    virtual ~CCamera() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

protected:
    virtual void Turn(_float4x4* OrbitMatrix, _fvector vAxis, const _float& fTimeDelta);
    virtual void Rotation(_float4x4* OrbitMatrix, _fvector vAxis, _float fRadian);
    virtual void Zoom(const _float& fTimeDelta);

protected:
    _float m_fFovY;
    _float m_fAspect;
    _float m_fNear;
    _float m_fFar;

    _float4x4 m_WorldMatrix;

public:
    virtual CGameObject* Clone(void* pArg) = 0;
    virtual void Free() override;
};

END