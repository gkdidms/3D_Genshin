#pragma once
#include "EffectObject.h"

#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CModel;
END

BEGIN(Client)
class CEffect_Trail :
    public CEffectObject
{
public:
    typedef struct tEffectTrailDesc : public CEffectObject::EFFECT_OBJECT_DESC
    {
        _char strFileName[MAX_PATH];
        _char strFilePath[MAX_PATH];
        _char strMeshName[MAX_PATH];
        _char strMeshFilePath[MAX_PATH];
        _bool isMask;
        _char strMaskFilePath[MAX_PATH];
    }EFFECT_TRAIL_DESC;

private:
    CEffect_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_Trail(const CEffect_Trail& rhs);
    virtual ~CEffect_Trail() = default;

public:
    virtual HRESULT Initialize_Prototype(void* pArg) override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();

private:
    CShader* m_pShaderCom = { nullptr };
    CModel* m_pModelCom = { nullptr };
    CTexture* m_pTextureCom = { nullptr };
    CTexture* m_pMaskTextureCom = { nullptr };

    wstring m_strFilePath;
    _char m_szMeshFilePath[MAX_PATH] = {""};
    wstring m_strMaskFilePath;

    _bool m_isMask = { false };
    _float m_fTimeDelta = { 0.f };
    _vector m_vPlayerLook = {};

public:
    static CEffect_Trail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free();
};

END