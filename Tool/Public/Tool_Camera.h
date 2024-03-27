#pragma once
#include "Camera.h"

#include "Tool_Defines.h"

BEGIN(Tool)
class CTool_Camera :
    public CCamera
{
public:
    typedef struct tToolCameraDesc: public CCamera::CAMERA_DESC {
        _float fSensor;
    }TOOLCAMERA_DESC;

private:
    CTool_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CTool_Camera(const CTool_Camera& rhs);
    virtual ~CTool_Camera() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Tick(const _float& fTimeDelta);
    virtual void Tick(const _float& fTimeDelta);
    virtual void Late_Tick(const _float& fTimeDelta);
    virtual HRESULT Render();

private:
    _float m_fSensor = { 0.f };
    _bool m_bCameraMove = { true };

public:
    static CTool_Camera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};

END