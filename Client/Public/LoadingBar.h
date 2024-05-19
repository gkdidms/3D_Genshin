#pragma once
#include "GameObject.h"

#include "Client_Defines.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CShader;
class CTexture;
END

BEGIN(Client)
class CLoadingBar :
    public CGameObject
{
private:
    CLoadingBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CLoadingBar(const CLoadingBar& rhs);
    virtual ~CLoadingBar() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
    CShader* m_pShaderCom = { nullptr };
    CTexture* m_pTextureCom = { nullptr };

    _float4x4 m_matWorld, m_matView, m_matProj;
    _float m_fSizeX, m_fSizeY, m_fX, m_fY;

private:
    HRESULT Add_Components();
    HRESULT Bind_Render();

public:
    static CLoadingBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};

END