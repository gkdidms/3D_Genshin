#pragma once
#include "GameObject.h"

#include "Client_Defines.h"

BEGIN(Engine)
class CVIBuffer_Terrain;
class CShader;
class CTexture;
END

BEGIN(Client)
class CTerrain :
    public CGameObject
{
private:
    CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CTerrain(const CTerrain& rhs);
    virtual ~CTerrain() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    CVIBuffer_Terrain* m_pVIBufferCom = { nullptr };
    CShader* m_pShaderCom = { nullptr };
    CTexture* m_pTextureCom = { nullptr };

    _float4x4 m_matView, m_matProj;

private:
    HRESULT Add_Components();
    HRESULT Bind_Render();

public:
    static CTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};

END
