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
    enum TEXTURE_MOVE_TYPE { INCREASE, SHRINK, BOUNCE, TRAIL, MOVE_END };
    typedef struct tEffectDefault : public CTool_Effect::TOOL_EFFECT_DESC
    {
    } EFFECT_DEFAULT_DESC;
private:
    CEffect_Default(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_Default(const CEffect_Default& rhs);
    virtual ~CEffect_Default() = default;

public:
    _uint Get_TextureMoveType() { return m_iTextureMoveType; }
    _float Get_TextureMoveSpeed() { return m_fTextureMoveSpeed; }

public:
    void Set_TextureMoveType(_uint iIndex) { m_iTextureMoveType = iIndex; }
    void Set_TextureMoveSpeed(_float fSpeed) { m_fTextureMoveSpeed = fSpeed; }



public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

    _uint m_iTextureMoveType = { MOVE_END };

private:
    
    _float m_fTextureMoveSpeed = { 1.f };

private:
    virtual HRESULT Add_Components() ;
    virtual HRESULT Bind_ResourceData() ;

public:
    static CEffect_Default* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
