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
    enum TEXTURE_MOVE_TYPE { INCREASE, SHRINK, SPREAT, TRAIL, MOVE_END };
    typedef struct tEffectImage : public CEffectObject::EFFECT_OBJECT_DESC {
        _uint iTextureMoveType = { MOVE_END };
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
    CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

    _uint m_iMoveType = { MOVE_END };

    _float4 m_vSpreatDir = {};

private:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();

public:
    static CEffect_Image* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free();
};

END