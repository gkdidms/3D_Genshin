#pragma once
#include "Base.h"

BEGIN(Engine)

class CGraphic_Device :
    public CBase
{
private:
    CGraphic_Device();
    virtual ~CGraphic_Device() = default;

public:
    _float Compute_ProjZ(const POINT& ptWindowPos);

public:
    HRESULT Initialize(const ENGINE_DESC& EngineDesc,
        _Inout_ ID3D11Device** ppDevice, _Inout_ ID3D11DeviceContext** ppContext);

    HRESULT Clear_BackBuffer_View(_float4 vClearColor);

    HRESULT Clear_DepthStencil_View();

    HRESULT Present();

private:
    ID3D11Device* m_pDevice = { nullptr };
    ID3D11DeviceContext* m_pDeviceContext = { nullptr };
    IDXGISwapChain* m_pSwapChain = { nullptr };

    ID3D11RenderTargetView* m_pBackBufferRTV = { nullptr };

    ID3D11DepthStencilView* m_pDepthStencilView = { nullptr };

private:
    HRESULT Ready_SwapChain(HWND hWnd, _bool isWindowed, _uint iWinCX, _uint iWinCY);
    HRESULT Ready_BackBufferRenderTargetView();
    HRESULT Ready_DepthStencilRenderTargetView(_uint iWinCX, _uint iWinCY);

public:
    static CGraphic_Device* Create(const ENGINE_DESC& EngineDesc, _Out_ ID3D11Device** ppDevice, _Out_ ID3D11DeviceContext** ppDeviceContextOut);
    virtual void Free() override;
};

END
