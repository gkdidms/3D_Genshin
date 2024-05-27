#pragma once
#include "Player_Bullet.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_RectZ;
class CCollider;
class CVIBuffer_Trail;
END

BEGIN(Client)
class CTighnari_Normal :
    public CPlayer_Bullet
{
private:
    CTighnari_Normal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CTighnari_Normal(const CTighnari_Normal& rhs);
    virtual ~CTighnari_Normal() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    CShader* m_pShaderCom = { nullptr };
    CTexture* m_pTextureCom[2] = {nullptr};
    CVIBuffer_RectZ* m_pVIBufferCom = { nullptr };
    CVIBuffer_Trail* m_pTrailVIBufferCom = { nullptr };

private:
    _float m_fAccelTime = { 0.f };
    _float m_fPower = { 5.f };
    _float m_fHeight = { 0.f }; // 플레이어 y값 저장

private:
    HRESULT Add_Components();
    HRESULT Bind_ResourceData();
    _bool Move_Arrow(const _float& fTimeDelta);

public:
    static CTighnari_Normal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
