#pragma once
#include "Player_Bullet.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CModel;
class CCollider;
class CVIBuffer_Trail;
END

BEGIN(Client)
class CTighnari_Burst :
    public CPlayer_Bullet
{
public:
    typedef struct tTighnariBurstDesc : public CPlayer_Bullet::BULLET_DESC {
        _uint iIndex;
    }TIGHNARI_BURST_DESC;

private:
    CTighnari_Burst(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CTighnari_Burst(const CTighnari_Burst& rhs);
    virtual ~CTighnari_Burst() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    CShader* m_pShaderCom = { nullptr };
    CShader* m_pTrailShaderCom = { nullptr };
    CModel* m_pModelCom = { nullptr };
    CTexture* m_pTextureCom[2] = {nullptr};
    CVIBuffer_Trail* m_pTrailVIBufferCom = { nullptr };

private:
    _float m_fAccelTime = { 0.f };
    _float m_fPower = { 7.f };
    _float m_fHeight = { 0.f }; // 플레이어 y값 저장
    _uint m_iIndex = { 0 };

private:
    HRESULT Add_Components();
    HRESULT Bind_ResourceData();
    HRESULT Bind_TrailResourceData();
    _bool Move_Arrow(const _float& fTimeDelta);

public:
    static CTighnari_Burst* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
