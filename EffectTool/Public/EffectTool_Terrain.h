#pragma once
#include "GameObject.h"

#include "EffectTool_Defines.h"

BEGIN(Engine)
class CShader;
class CVIBuffer_Terrain;
class CTexture;
END

BEGIN(Effect)
class CEffectTool_Terrain :
    public CGameObject
{
private:
    CEffectTool_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffectTool_Terrain(const CEffectTool_Terrain& rhs);
    virtual ~CEffectTool_Terrain() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Tick(const _float& fTimeDelta);
    virtual void Tick(const _float& fTimeDelta);
    virtual void Late_Tick(const _float& fTimeDelta);
    virtual HRESULT Render();

private:
    CVIBuffer_Terrain* m_pVIBufferCom = { nullptr };
    CShader* m_pShaderCom = { nullptr };
    CTexture* m_pTextureCom = { nullptr };

private:
    HRESULT Add_Components();
    HRESULT Bind_ResouceData();

    void Get_MousePos_On_Terrain();

public:
    static CEffectTool_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
