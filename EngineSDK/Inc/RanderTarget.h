#pragma once
#include "Base.h"

BEGIN(Engine)
class CRenderTarget :
    public CBase
{
private:
    CRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CRenderTarget(const CRenderTarget& rhs);
    virtual ~CRenderTarget() = default;

public:
    HRESULT Initialize(_uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
    HRESULT Clear();
    HRESULT Bind_SVR(class CShader* pShader, const _char* pConstantName);

private:
    ID3D11Device* m_pDevice = { nullptr };
    ID3D11DeviceContext* m_pContext = { nullptr };

    ID3D11RenderTargetView* m_pRTV = { nullptr };
    ID3D11ShaderResourceView* m_pSRV = { nullptr };
    ID3D11Texture2D* m_pTexture2D = { nullptr };

private:
    _float4 m_vClearColor;

public:
    static CRenderTarget* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
    virtual void Free() override;
};
END
