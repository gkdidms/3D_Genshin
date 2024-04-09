#pragma once
#include "Camera.h"

#include "Client_Defines.h"

BEGIN(Client)
class CDefaultCamera :
    public CCamera
{
public:
    typedef struct tDefaultCamera : public CAMERA_DESC
    {
        const _float4x4* pPlayerMatrix;
    }DEFAULT_CAMERA_DESC;

private:
    CDefaultCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CDefaultCamera(const CDefaultCamera& rhs);
    virtual ~CDefaultCamera() = default;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(void* pArg) override;
    void Priority_Tick(const _float& fTimeDelta) override;
    void Tick(const _float& fTimeDelta) override;
    void Late_Tick(const _float& fTimeDelta) override;
    HRESULT Render() override;

private:
    const _float4x4* m_pTargetMatrix;

public:
    static CDefaultCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
