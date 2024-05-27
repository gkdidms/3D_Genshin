#pragma once
#include "Base.h"

BEGIN(Engine)
class CRenderTarget :
    public CBase
{
private:
    CRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CRenderTarget() = default;

public:
    ID3D11RenderTargetView* Get_RTV() { return m_pRTV; }

public:
    HRESULT Initialize(_uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
    HRESULT Clear();
    HRESULT Bind_SVR(class CShader* pShader, const _char* pConstantName);

#ifdef _DEBUG
public:
    HRESULT Ready_Debug(_float fX, _float fY, _float fSizeX, _float fSizeY);
    HRESULT Render_Debug(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif // _DEBUG

private:
    ID3D11Device* m_pDevice = { nullptr };
    ID3D11DeviceContext* m_pContext = { nullptr };

    ID3D11RenderTargetView* m_pRTV = { nullptr };
    ID3D11ShaderResourceView* m_pSRV = { nullptr };
    ID3D11Texture2D* m_pTexture2D = { nullptr };

#ifdef _DEBUG
private:
    _float4x4 m_WorldMatrix;
#endif // _DEBUG


private:
    _float4 m_vClearColor;

public:
    static CRenderTarget* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
    virtual void Free() override;
};
END
