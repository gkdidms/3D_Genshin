#pragma once
#include "EffectObject.h"

#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)
class CEffect_Image :
    public CEffectObject
{
public:
    enum MOVE_TYPE { INCREASE, SHRINK, MOVE_END };
    typedef struct tEffectImage : public CEffectObject::EFFECT_OBJECT_DESC {
        _char szFileName[MAX_PATH];
        _char szFilePath[MAX_PATH];

    } EFFECT_IMAGE_DESC;

private:
    CEffect_Image(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_Image(const CEffect_Image& rhs);
    virtual ~CEffect_Image() = default;

public:
    virtual HRESULT Initialize_Prototype(void* pArg);
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    CShader* m_pShaderCom = { nullptr };
    CTexture* m_pTextureCom = { nullptr };
    CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

    wstring m_strFilePath = { L"" };

    _uint m_MoveType = { MOVE_END };

private:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();

public:
    static CEffect_Image* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free();
};

END