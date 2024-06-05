#pragma once
#include "BlendObject.h"

#include "Client_Defines.h"

BEGIN(Client)
class CEffectObject abstract :
    public CBlendObject
{
public:
    enum EFFECT_TYPE { EFFECT_PARTICLE, EFFECT_IMG, EFFECT_MESH, EFFECT_TRAIL, EFFECT_END };

    typedef struct tEffectObjectDesc {
        const _float4x4* pParentMatrix;
        _char strTextureFilePath[MAX_PATH];
        string strMaskFilePath;
        string strNoiseFilePath;

        _uint iShaderPass;
        _bool isMask;
        _bool isNoise;
        _uint iEffectType;
        _int iTextureNum = { 1 };

        _float4 vColor = {};
        _uint iRendererType = { 0 };
        _bool isFrameLoop = { false };
        _float fStartTime = { 0.f };
        _float fDurationTime = { 0.f };
        _matrix WorldMatrix = { XMMatrixIdentity() };
    } EFFECT_OBJECT_DESC;

protected:
    CEffectObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffectObject(const CEffectObject& rhs);
    virtual ~CEffectObject() = default;

public:
    virtual HRESULT Initialize_Prototype(void* pArg);
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

protected:
    const _float4x4* m_ParentMatrix = {nullptr};
    _float4x4 m_WorldMatrix = {};

protected:
    CShader* m_pShaderCom = { nullptr };
    CTexture* m_pTextureCom = { nullptr };
    CTexture* m_pMaskTextureCom = { nullptr };
    CTexture* m_pNoiseTextureCom = { nullptr };

    wstring m_strTextureFilePath = { L"" }; // 텍스쳐
    wstring m_strMaskFilePath = { L"" }; // 마스크
    wstring m_strNoiseFilePath = { L"" }; // 노이즈

    _uint m_iShaderPass = { 0 };
    _uint m_iTextureNum = { 0 };

    _bool m_isMask = { false };
    _bool m_isNoise = { false };
    _uint m_iEffectType = { EFFECT_END };

    _float4 m_vColor = {};
    _uint m_iRendererType = { 0 };
    _bool m_isFrameLoop = { false };
    _float m_fStartTime = { 0.f };
    _float m_fDurationTime = { 0.f };

protected:
    _float m_fCurrentTime = { 0.f };
    _float m_fFrame = { 0.f };

protected:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();

public:
    virtual void Free() override;
};
END
