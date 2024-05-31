#pragma once

#include "BlendObject.h"

#include "EffectTool_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
END

BEGIN(Effect)
class CTool_Effect abstract:
    public CBlendObject
{
public:
    typedef struct tToolEffectDesc : public CGameObject::GAMEOBJECT_DESC {
        _char strTextureFilePath[MAX_PATH];
        string strMaskFilePath;
        string strNoiseFilePath;

        _bool isMask;
        _bool isNoise;
        _uint iEffectType;
        _int iTextureNum = { 1 };
    }TOOL_EFFECT_DESC;

protected:
    CTool_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CTool_Effect(const CTool_Effect& rhs);
    virtual ~CTool_Effect() = default;

public:
    _uint Get_EffectType() { return m_iEffectType; }
    _uint Get_ShaderPass() { return m_iShaderPass; }
    _uint Get_TextureNum() { return m_iNumTexture; }
    _float4 Get_Color() { return m_vColor; }
    _uint Get_RendererType() { return m_iRendererType; }

    wstring Get_TextureFilePath() { return m_strTextureFilePath; }
    wstring Get_MaskFilePath() { return m_strMaskPath; }
    wstring Get_NoiseFilePath() { return m_strNoisePath; }

    _bool isMask() { return m_isMask; }
    _bool isNoise() { return m_isNoise; }
    _bool isFrameLoop() { return m_isFrameLoop; }
    
public:
    void Set_ShaderPass(_uint iShaderPass) { m_iShaderPass = iShaderPass; }
    void Set_Color(_float4 vColor) { m_vColor = vColor; }
    void Set_RendererType(_int iRendererType) { m_iRendererType = iRendererType; }
    void Set_FrameLoop(_bool isLoop) { m_isFrameLoop = isLoop;}

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

protected:
    CShader* m_pShaderCom = { nullptr };
    CTexture* m_pTextureCom = { nullptr };
    CTexture* m_pMaskTextureCom = { nullptr };
    CTexture* m_pNoiseTexturCom = { nullptr };

    _uint m_iEffectType = { 0 };
    _int m_iNumTexture = { 1 };
    _float4 m_vColor = {};

    _uint m_iShaderPass = { 0 };
    _uint m_iRendererType = { 0 };

    _float m_fFrame = { 0.f };
    _bool m_isFrameLoop = { false };
    _bool m_isFrameStop = { false };

protected:
    wstring m_strTextureFilePath = { L"" };
    wstring m_strMaskPath = { L"" };
    wstring m_strNoisePath = { L"" };

    _bool m_isMask = { false };
    _bool m_isNoise = { false };

protected:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();

public:
    virtual CGameObject* Clone(void* pArg) = 0;
    virtual void Free() override;

};
END
