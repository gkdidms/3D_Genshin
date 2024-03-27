#include "GamePlay_Level.h"

#include "GameInstance.h"
#include "DefaultCamera.h"
#include "Monster.h"

CGamePlay_Level::CGamePlay_Level(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
{
}

HRESULT CGamePlay_Level::Initialize()
{
	if (FAILED(Ready_Layer_Camera(L"Layer_Camera")))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(L"Layer_Terrain")))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(L"Layer_Fiona")))
		return E_FAIL;

	return S_OK;
}

void CGamePlay_Level::Tick(const _float& fTimeDelta)
{
	SetWindowText(g_hWnd, TEXT("GamePlay ÀÔ´Ï´Ù."));
}

void CGamePlay_Level::Render()
{
}

HRESULT CGamePlay_Level::Ready_Layer_Camera(const wstring& strLayerTag)
{
	CDefaultCamera::DEFAULT_CAMERA_DESC tDefaultCamera = {};

	tDefaultCamera.vEye = _float4(0.f, 40.f, -30.f, 1.f);
	tDefaultCamera.vFocus = _float4(0.f, 0.f, 0.f, 1.f);
	
	tDefaultCamera.fFovY = XMConvertToRadians(60.f);
	tDefaultCamera.fAspect = g_iWinSizeX / (_float)g_iWinSizeY;
	tDefaultCamera.fNear = 0.1f;
	tDefaultCamera.fFar = 3000.f;

	tDefaultCamera.fSpeedPecSec = 20.f;
	tDefaultCamera.fRotatePecSec = XMConvertToRadians(45.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_Camera", strLayerTag, &tDefaultCamera)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGamePlay_Level::Ready_Layer_BackGround(const wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_Terrain", strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGamePlay_Level::Ready_Layer_Monster(const wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_Fiona", strLayerTag)))
		return E_FAIL;

	return S_OK;
}

CGamePlay_Level* CGamePlay_Level::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CGamePlay_Level* pInstance = new CGamePlay_Level(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
		Safe_Release(pInstance);

	return pInstance;
}

void CGamePlay_Level::Free()
{
	__super::Free();
}
