#pragma once
#include "GameObject.h"

#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CVIBuffer_Rect;
class CTextrue;
END

BEGIN(Client)
class CUI abstract:
    public CGameObject
{
protected:
    CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CUI(const CUI& rhs);
    virtual ~CUI() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

protected:
    CShader* m_pShaderCom = { nullptr };
    CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
    CTexture* m_pTextureCom = { nullptr };

protected:
    _float4x4 m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;
    _float m_fSizeX, m_fSizeY, m_fX, m_fY;

protected:
    virtual HRESULT Add_Components() = 0;
    virtual HRESULT Bind_ResourceData() = 0;

public:
    virtual void Free() override;
};
END
