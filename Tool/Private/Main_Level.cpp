#include "Main_Level.h"

#include "GameInstance.h"
#include "Tool_Camera.h"
#include "Tool_Non_Object.h"
#include "Tool_Object_Manager.h"

CMain_Level::CMain_Level(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext },
	m_pObjectManager { CTool_Object_Manager::GetInstance() }
{
	Safe_AddRef(m_pObjectManager);
}

HRESULT CMain_Level::Initialize()
{
	m_pObjectManager->Initialize();

	if (FAILED(Ready_Layer_Camera(L"GameObject_Camera")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Terrain(L"GameObject_Terrain")))
		return E_FAIL;

	return S_OK;
}

void CMain_Level::Tick(const _float& fTimeDelta)
{           
	m_pObjectManager->Priority_Tick(fTimeDelta);
	m_pObjectManager->Tick(fTimeDelta);
	m_pObjectManager->Late_Tick(fTimeDelta);
}

void CMain_Level::Render()
{
	m_pObjectManager->Render();
}

HRESULT CMain_Level::Ready_Layer_Camera(const wstring strName)
{
	CTool_Camera::TOOLCAMERA_DESC tDesc{};
	tDesc.fSensor = 0.1f;
	tDesc.vEye = _float4{ 0.f, 30.f, -30.f, 1.f };
	tDesc.vFocus = _float4{ 0.f, 0.f, 0.f, 1.f };
	tDesc.fFovY = XMConvertToRadians(60.f);
	tDesc.fAspect = g_iWinSizeX / (_float)g_iWinSizeY;
	tDesc.fNear = 0.1f;
	tDesc.fFar = 3000.f;

	tDesc.fSpeedPecSec = 30.f;
	tDesc.fRotatePecSec = XMConvertToRadians(45.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_MAIN, L"Prototype_GameObject_Camera", strName, &tDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMain_Level::Ready_Layer_Terrain(const wstring strName)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_MAIN, L"Prototype_GameObject_Terrain", strName)))
		return E_FAIL;

	return S_OK;
}

CMain_Level* CMain_Level::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMain_Level* pInstance = new CMain_Level(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMain_Level::Free()
{
	__super::Free();
	m_pObjectManager->Release_Object();
	Safe_Release(m_pObjectManager);
	
}
