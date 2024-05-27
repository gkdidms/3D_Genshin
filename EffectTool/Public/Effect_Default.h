#pragma once
#include "Tool_Effect.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CVIBuffer_Trail;
class CCollider;
END 

BEGIN(Effect)
class CEffect_Default :
    public CTool_Effect
{
public:
    typedef struct tEffectDefault : public CTool_Effect::TOOL_EFFECT_DESC
    {
        _char strTextureFilePath[MAX_PATH];
        _char strTrailFilePath[MAX_PATH];
        _bool isTrailBuffer;
        _int iNumTexture;
    } EFFECT_DEFAULT_DESC;
private:
    CEffect_Default(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_Default(const CEffect_Default& rhs);
    virtual ~CEffect_Default() = default;

public:
    wstring Get_TextureFilePath() { return m_strTextureFilePath; }
    wstring Get_TrailFilePath() { return m_strTrailTextureFilePath; }

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
    CVIBuffer_Trail* m_pTrailVIBufferCom = { nullptr };
    CTexture* m_pTrailTextureCom = { nullptr };

    wstring m_strTextureFilePath = { L"" };
    wstring m_strTrailTextureFilePath = { L"" };
    _int m_iNumTexture = { 0 };
    _bool m_isTrailBuffer = { false };

private:
    _float m_fDuration = { 5.f };
    _float m_fCurrentTime = { 0.f };

private:
    virtual HRESULT Add_Components() ;
    virtual HRESULT Bind_ResourceData() ;

public:
    static CEffect_Default* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
