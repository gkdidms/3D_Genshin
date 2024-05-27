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
        _uint iEffectType;
        wstring strTextureFileName;
    }TOOL_EFFECT_DESC;

protected:
    CTool_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CTool_Effect(const CTool_Effect& rhs);
    virtual ~CTool_Effect() = default;

public:
    _uint Get_EffectType() { return m_iEffectType; }
    _uint Get_ShaderPass() { return m_iShaderPass; }
    _bool Get_Start() { return m_isStart; }
    wstring Get_FileName() { return m_strTextureFileName; }

public:
    void Set_ShaderPass(_uint iShaderPass) { m_iShaderPass = iShaderPass; }
    void isStart() { m_isStart = true; }

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

    _uint m_iEffectType = { 0 };
    wstring m_strTextureFileName = { L"" };

protected:
    _uint m_iShaderPass = { 0 };
    _bool m_isStart = { false };

protected:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();

public:
    virtual CGameObject* Clone(void* pArg) = 0;
    virtual void Free() override;

};
END
