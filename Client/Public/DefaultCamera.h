#pragma once
#include "Camera.h"

#include "Client_Defines.h"
#include "Player.h"

BEGIN(Client)

class CDefaultCamera :
    public CCamera
{
public:
    typedef struct tDefaultCamera : public CAMERA_DESC
    {
        _float fSensor;
        const _float4x4* pPlayerMatrix;
        const _float4x4* pCameraLookMatrix;
    }DEFAULT_CAMERA_DESC;

private:
    CDefaultCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CDefaultCamera(const CDefaultCamera& rhs);
    virtual ~CDefaultCamera() = default;

public: // 원소폭팔
    void isEB(_bool isEB, CPlayer::PLAYERBLE_TYPE ePlayerble = CPlayer::PLAYER_TYPE_END);

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(void* pArg) override;
    void Priority_Tick(const _float& fTimeDelta) override;
    void Tick(const _float& fTimeDelta) override;
    void Late_Tick(const _float& fTimeDelta) override;
    HRESULT Render() override;

public: // 원소스킬
    void Nilou_EB(const _float& fTimeDelta);
    void Wanderer_EB(const _float& fTimeDelta);
    void Tighnari_EB(const _float& fTimeDelta);
    void Yae_EB(const _float& fTimeDelta);

private:
    void Turn(_fvector vAxis, const _float& fTimeDelta);
    void Rotation(_fvector vAxis, _float fRadian);
    void Zoom(const _float& fTimeDelta);

private:
    const _float4x4* m_pTargetMatrix;
    const _float4x4* m_pCameraLook;
    _float4x4 m_OrbitMatrix;
    _float m_fSensor = { 0.f };

private: // 원소스킬
    CPlayer::PLAYERBLE_TYPE m_ePlayerble = { CPlayer::PLAYER_TYPE_END };
    _bool m_isEB = { false };

    _float fEBTime = { 0.f };

public:
    static CDefaultCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
