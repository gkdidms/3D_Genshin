#pragma once
#include "BlendObject.h"

#include "Client_Defines.h"
BEGIN(Engine)
class CVIBuffer_Trail;
class CCollider;
END

BEGIN(Engine)
class CBoss_Trail :
    public CBlendObject
{
public:
    typedef struct tBossTrailDesc {
        const _float4x4* pParentMatrix;
    } BOSS_TRAIL_DESC;

private:
    CBoss_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CBoss_Trail(const CBoss_Trail& rhs);
    virtual ~CBoss_Trail() = default;

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
    CVIBuffer_Trail* m_pTrailBufferCom = { nullptr };

    const _float4x4* m_pParentMatrix = { nullptr };

    _float m_fEffectDelayDuration = { 0.2f };
    _float m_fEffectDelayTime = { 0.f };
    _float m_fEffectLightningTime = { 0.f };

private:
    HRESULT Add_Componenets();
    HRESULT Bind_ResourceData();

public:
    static CBoss_Trail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free();
};
END
