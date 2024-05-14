#pragma once
#include "Camera.h"

#include "Client_Defines.h"
#include "Player.h"

BEGIN(Client)
class CCutCamera :
    public CCamera
{
public:
    typedef struct tCutCameraDesc : public CAMERA_DESC
    {
        _float fSensor;
        const _float4x4* pPlayerMatrix;
        const _float4x4* pCameraLookMatrix;
    }Cut_CAMERA_DESC;

private:
    CCutCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CCutCamera(const CCutCamera& rhs);
    virtual ~CCutCamera() = default;

public: // ¿ø¼ÒÆøÆÈ
    void isCutScene(_bool isCutScene, _uint ePlayerble = CPlayer::PLAYER_TYPE_END);

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    const _float4x4* m_pTargetMatrix;
    const _float4x4* m_pCameraLook;
    _float4x4 m_OrbitMatrix;
    _float m_fSensor = { 0.f };

    _float4 m_vEye = {};
    _float4 m_vFocus = {};

private: // ¿ø¼Ò½ºÅ³
    _uint m_ePlayerble = { CPlayer::PLAYER_TYPE_END };
    _bool m_isCutScene = { false };

    _float fEBTime = { 0.f };

private:
    virtual void Zoom(const _float& fTimeDelta) override;
    void Reset();

private: // Ä«¸Þ¶ó ÄÆ¾À ÀÌµ¿°ª Á¶Àý
    void Nilou_EB(const _float& fTimeDelta);
    void Wanderer_EB(const _float& fTimeDelta);
    void Tighnari_EB(const _float& fTimeDelta);
    void Yae_EB(const _float& fTimeDelta);

public:
    static CCutCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};

END