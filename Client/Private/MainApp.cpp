#include "../Default/framework.h"
#include "MainApp.h"

#include "GameInstance.h"
#include "Loading_Level.h"
#include "Background.h"

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
	m_pGameInstance->Tick(fTimeDelta);
}

void CMainApp::Render()
{
	m_pGameInstance->Draw(_float4(0.f, 0.f, 1.f, 1.f));
}

HRESULT CMainApp::Open_Level()
{	
	if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLoading_Level::Create(LEVEL_LOGO, m_pDevice, m_pContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Components()
{
	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, L"Prototype_Component_Shader", CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFile/Shader_VtxPosTex.hlsl"), VXTPOSTEX::Elements, VXTPOSTEX::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_STATIC, L"Prototype_Component_Texture_Loading", CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Default0.jpg"), 1))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_GameObjects()
{
	if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_Background", CBackground::Create(m_pDevice, m_pContext))))
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
