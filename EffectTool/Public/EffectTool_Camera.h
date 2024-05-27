#pragma once
#include "Camera.h"

#include "EffectTool_Defines.h"
BEGIN(Effect)
class CEffectTool_Camera :
    public CCamera
{
public:
    typedef struct tEffectToolCameraDesc : public CCamera::CAMERA_DESC
    {
        _float fSensor;
    } EFFECT_TOOL_CAMERA_DESC;

private:
    CEffectTool_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffectTool_Camera(const CEffectTool_Camera& rhs);
    virtual ~CEffectTool_Camera() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    _float m_fSensor = { 0.f };

    _bool m_isCameraMove = { true };

public:
    static CEffectTool_Camera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};

END