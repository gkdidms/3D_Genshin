#include "..\public\Graphic_Device.h"

CGraphic_Device::CGraphic_Device()
	: m_pDevice{ nullptr }
	, m_pDeviceContext{ nullptr }
{

}

HRESULT CGraphic_Device::Initialize(const ENGINE_DESC& EngineDesc, _Inout_ ID3D11Device** ppDevice, _Inout_ ID3D11DeviceContext** ppContext)
{
	_uint		iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL			FeatureLV;

	/* 그래픽 장치를 초기화한다. */
	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, iFlag, nullptr, 0, D3D11_SDK_VERSION, &m_pDevice, &FeatureLV, &m_pDeviceContext)))
		return E_FAIL;

	/* SwapChain 전면과 후면버퍼를 번갈아가며 화면에 보여준다.(Present) */

	/* 백버퍼를 생성하기 위한 texture2D 만든거야. */
	if (FAILED(Ready_SwapChain(EngineDesc.hWnd, EngineDesc.isWindowed, EngineDesc.iWinSizeX, EngineDesc.iWinSizeY)))
		return E_FAIL;

	/* 스왑체인이 들고 있는 텍스쳐 2D를 가져와서 이를 바탕으로 백버퍼 렌더타겟 뷰를 만든다.*/
	if (FAILED(Ready_BackBufferRenderTargetView()))
		return E_FAIL;

	if (FAILED(Ready_DepthStencilRenderTargetView(EngineDesc.iWinSizeX, EngineDesc.iWinSizeY)))
		return E_FAIL;

	/* 장치에 바인드해놓을 렌더타겟들과 뎁스스텐실뷰를 세팅한다. */
	/* 장치는 동시에 최대 8개의 렌더타겟을 들고 있을 수 있다. */
	ID3D11RenderTargetView* pRTVs[1] = {
		m_pBackBufferRTV
	};

	m_pDeviceContext->OMSetRenderTargets(1, pRTVs,
		m_pDepthStencilView);

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = (_float)EngineDesc.iWinSizeX;
	ViewPortDesc.Height = (_float)EngineDesc.iWinSizeY;
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;

	m_pDeviceContext->RSSetViewports(1, &ViewPortDesc);

	*ppDevice = m_pDevice;
	*ppContext = m_pDeviceContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);

	return S_OK;
}

HRESULT CGraphic_Device::Clear_BackBuffer_View(_float4 vClearColor)
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	/* 백버퍼를 초기화한다.  */
	m_pDeviceContext->ClearRenderTargetView(m_pBackBufferRTV, (_float*)&vClearColor);

	return S_OK;
}

HRESULT CGraphic_Device::Clear_DepthStencil_View()
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	return S_OK;
}

HRESULT CGraphic_Device::Present()
{
	if (nullptr == m_pSwapChain)
		return E_FAIL;

	/* 전면 버퍼와 후면 버퍼를 교체하여 후면 버퍼를 전면으로 보여주는 역할을 한다. */
	/* 후면 버퍼를 직접 화면에 보여줄게. */
	return m_pSwapChain->Present(0, 0);
}

_float CGraphic_Device::Compute_ProjZ(const POINT& ptWindowPos)
{
	//D3D11_MAPPED_SUBRESOURCE		SubResources{};

	//_float	fProjZ = { 0.0f };

	//m_pDeviceContext->CopyResource(m_pDepthTexture, m_pHitScreenTexture);

	//m_pDeviceContext->Map(m_pDepthTexture, 0, D3D11_MAP_READ, 0, &SubResources);

	///* 내 마우스 좌표가 존재하는 위치에 있는 텍스쳐 픽셀의 인덱스 */
	//_uint		iIndex = ptWindowPos.y * 1280.0f + ptWindowPos.x;

	//_float2		vResult = ((_float2*)SubResources.pData)[iIndex];

	//m_pDeviceContext->Unmap(m_pDepthTexture, 0);


	//if (0.0f == vResult.y)
	//	return -1.f;

	//return vResult.x;

	return 0;
}


HRESULT CGraphic_Device::Ready_SwapChain(HWND hWnd, _bool isWindowed, _uint iWinCX, _uint iWinCY)
{
	IDXGIDevice* pDevice = nullptr;
	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDevice);

	IDXGIAdapter* pAdapter = nullptr;
	pDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pAdapter);

	IDXGIFactory* pFactory = nullptr;
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);

	/* 스왑체인을 생성한다. = 텍스쳐를 생성하는 행위 + 스왑하는 형태  */
	DXGI_SWAP_CHAIN_DESC		SwapChain;
	ZeroMemory(&SwapChain, sizeof(DXGI_SWAP_CHAIN_DESC));

	/*텍스처(백버퍼)를 생성하는 행위*/
	SwapChain.BufferDesc.Width = iWinCX;
	SwapChain.BufferDesc.Height = iWinCY;


	SwapChain.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	SwapChain.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChain.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChain.BufferCount = 1;

	/*스왑하는 형태*/
	SwapChain.BufferDesc.RefreshRate.Numerator = 60;
	SwapChain.BufferDesc.RefreshRate.Denominator = 1;
	SwapChain.SampleDesc.Quality = 0;
	SwapChain.SampleDesc.Count = 1;

	SwapChain.OutputWindow = hWnd;
	SwapChain.Windowed = isWindowed;
	SwapChain.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	/* 백버퍼라는 텍스처를 생성했다. */
	if (FAILED(pFactory->CreateSwapChain(m_pDevice, &SwapChain, &m_pSwapChain)))
		return E_FAIL;

	Safe_Release(pFactory);
	Safe_Release(pAdapter);
	Safe_Release(pDevice);

	return S_OK;
}


HRESULT CGraphic_Device::Ready_BackBufferRenderTargetView()
{
	if (nullptr == m_pDevice)
		return E_FAIL;



	/* 내가 앞으로 사용하기위한 용도의 텍스쳐를 생성하기위한 베이스 데이터를 가지고 있는 객체이다. */
	/* 내가 앞으로 사용하기위한 용도의 텍스쳐 : ID3D11RenderTargetView, ID3D11ShaderResoureView, ID3D11DepthStencilView */
	ID3D11Texture2D*  pBackBufferTexture = nullptr;

	/* 스왑체인이 들고있던 텍스처를 가져와봐. */
	if (FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateRenderTargetView(pBackBufferTexture, nullptr, &m_pBackBufferRTV)))
		return E_FAIL;

	Safe_Release(pBackBufferTexture);

	return S_OK;
}

//HRESULT CGraphic_Device::Ready_HitScreenRenderTargetView(_uint iWinCX, _uint iWinCY)
//{
//	if (nullptr == m_pDevice)
//		return E_FAIL;
//
//	D3D11_TEXTURE2D_DESC	TextureDesc;
//	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));
//
//	TextureDesc.Width = iWinCX;
//	TextureDesc.Height = iWinCY;
//	TextureDesc.MipLevels = 1;
//	TextureDesc.ArraySize = 1;
//	TextureDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
//
//	TextureDesc.SampleDesc.Quality = 0;
//	TextureDesc.SampleDesc.Count = 1;
//
//	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
//	TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET
//		/*| D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE*/;
//	TextureDesc.CPUAccessFlags = 0;
//	TextureDesc.MiscFlags = 0;
//
//	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pHitScreenTexture)))
//		return E_FAIL;
//
//	if (FAILED(m_pDevice->CreateRenderTargetView(m_pHitScreenTexture, nullptr, &m_pHitScreenRTV)))
//		return E_FAIL;
//
//
//	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));
//
//	TextureDesc.Width = iWinCX;
//	TextureDesc.Height = iWinCY;
//	TextureDesc.MipLevels = 1;
//	TextureDesc.ArraySize = 1;
//	TextureDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
//
//	TextureDesc.SampleDesc.Quality = 0;
//	TextureDesc.SampleDesc.Count = 1;
//
//	TextureDesc.Usage = D3D11_USAGE_STAGING;
//	TextureDesc.BindFlags = 0;
//	TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
//	TextureDesc.MiscFlags = 0;
//
//	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pDepthTexture)))
//		return E_FAIL;
//
//	return S_OK;
//}

HRESULT CGraphic_Device::Ready_DepthStencilRenderTargetView(_uint iWinCX, _uint iWinCY)
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	ID3D11Texture2D* pDepthStencilTexture = nullptr;



	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = iWinCX;
	TextureDesc.Height = iWinCY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL
		/*| D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE*/;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
		return E_FAIL;

	/* RenderTarget */
	/* ShaderResource */
	/* DepthStencil */

	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pDepthStencilView)))
		return E_FAIL;

	Safe_Release(pDepthStencilTexture);

	return S_OK;
}

CGraphic_Device* CGraphic_Device::Create(const ENGINE_DESC& EngineDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppDeviceContextOut)
{
	CGraphic_Device* pInstance = new CGraphic_Device();

	if (FAILED(pInstance->Initialize(EngineDesc, ppDevice, ppDeviceContextOut)))
	{
		MSG_BOX("Failed to Created : CGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGraphic_Device::Free()
{
	Safe_Release(m_pSwapChain);
	Safe_Release(m_pDepthStencilView);
	Safe_Release(m_pBackBufferRTV);
	Safe_Release(m_pDeviceContext);

	/*#if defined(DEBUG) || defined(_DEBUG)
		ID3D11Debug* d3dDebug;
		HRESULT hr = m_pDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&d3dDebug));
		if (SUCCEEDED(hr))
		{
			OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
			OutputDebugStringW(L"                                                                    D3D11 Live Object ref Count Checker \r ");
			OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
	
			hr = d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	
			OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
			OutputDebugStringW(L"                                                                    D3D11 Live Object ref Count Checker END \r ");
			OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
		}
		if (d3dDebug != nullptr)            d3dDebug->Release();
	#endif*/


	Safe_Release(m_pDevice);
}
