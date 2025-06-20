#include "../Default/framework.h"
#include "MainApp.h"

#include "GameInstance.h"
#include "Loading_Level.h"
#include "Background.h"
#include "LoadingBar.h"

_uint CMainApp::g_iCurrentLevel = 0;

CMainApp::CMainApp()
	: m_pGameInstance{ CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	ENGINE_DESC EngineDesc = {};
	EngineDesc.iWinSizeX = g_iWinSizeX;
	EngineDesc.iWinSizeY = g_iWinSizeY;
	EngineDesc.isWindowed = true;
	EngineDesc.hWnd = g_hWnd;
	EngineDesc.hInst = g_hInst;

	m_pGameInstance->Initialize_Engine(LEVEL_END, EngineDesc, &m_pDevice, &m_pContext);

	if (FAILED(Ready_Prototype_Components()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_GameObjects()))
		return E_FAIL;

	if (FAILED(Open_Level()))
		return E_FAIL;

	if (FAILED(Ready_Font()))
		return E_FAIL;

#ifdef _DEBUG

	if (::AllocConsole() == TRUE)
	{
		FILE* nfp[3];
		freopen_s(nfp + 0, "CONOUT$", "rb", stdin);
		freopen_s(nfp + 1, "CONOUT$", "wb", stdout);
		freopen_s(nfp + 2, "CONOUT$", "wb", stderr);
		std::ios::sync_with_stdio();
	}

#endif // _DEBUG

	return S_OK;
}

void CMainApp::Tick(const _float& fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;

	m_pGameInstance->Tick(fTimeDelta);
}

void CMainApp::Render()
{
	++m_iNumRender;

	if (m_fTimeAcc >= 1.f)
	{
		wsprintf(m_szFPS, TEXT("FPS : %d"), m_iNumRender);
		m_fTimeAcc = 0.f;
		m_iNumRender = 0;
	}

	m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 1.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();

	m_pGameInstance->Draw();

	m_pGameInstance->Render_Font(TEXT("Font_Nanum_28"), m_szFPS, _float2(0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));

	m_pGameInstance->Present();
}

HRESULT CMainApp::Open_Level()
{	
	if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLoading_Level::Create(LEVEL_GAMEPLAY, m_pDevice, m_pContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Components()
{
	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, L"Prototype_Component_Shader", CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VXTPOSTEX::Elements, VXTPOSTEX::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, L"Prototype_Component_Texture_Loading", CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Loading/Loading.jpg"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, L"Prototype_Component_Texture_LoadingBar", CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Loading/Loading_Icon.dds"), 1))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObjects()
{
	if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_Background", CBackground::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_LoadingBar", CLoadingBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Font()
{
	if (FAILED(m_pGameInstance->Add_Font(L"Font_Nanum_12", L"../Bin/Resources/Fonts/nanum12.spritefont")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(L"Font_Nanum_10", L"../Bin/Resources/Fonts/nanum10.spritefont")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(L"Font_Nanum_14", L"../Bin/Resources/Fonts/nanum14.spritefont")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(L"Font_Nanum_20", L"../Bin/Resources/Fonts/nanum20.spritefont")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(L"Font_Nanum_22", L"../Bin/Resources/Fonts/nanum22.spritefont")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(L"Font_Nanum_24", L"../Bin/Resources/Fonts/nanum24.spritefont")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(L"Font_Nanum_28", L"../Bin/Resources/Fonts/nanum28.spritefont")))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Font(L"Font_SDK_SC_10", L"../Bin/Resources/Fonts/SDK_SC_10.spritefont")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(L"Font_SDK_SC_12", L"../Bin/Resources/Fonts/SDK_SC_12.spritefont")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(L"Font_SDK_SC_20", L"../Bin/Resources/Fonts/SDK_SC_20.spritefont")))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Font(L"Font_SDK_SC_24", L"../Bin/Resources/Fonts/SDK_SC_24.spritefont")))
		return E_FAIL;

	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	m_pGameInstance->Release_Engine();
	Safe_Release(m_pGameInstance);
}
