#include "Level_Tool.h"

#include "EffectTool_Camera.h"
#include "GameInstance.h"

#include "Effect_Manager.h"
CLevel_Tool::CLevel_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel {pDevice, pContext},
	m_pEffectManager{ CEffect_Manager::GetInstance() }
{
	Safe_AddRef(m_pEffectManager);
}

HRESULT CLevel_Tool::Initialize()
{
	if (FAILED(Ready_Lights()))
		return E_FAIL;
	
	if (FAILED(Ready_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Terrain(TEXT("Layer_Terrain"))))
		return E_FAIL;

	//if (FAILED(Ready_Effect(TEXT("Layer_Effect"))))
	//	return E_FAIL;

	return S_OK;
}

void CLevel_Tool::Tick(const _float& fTimeDelta)
{
	m_pEffectManager->Tick(fTimeDelta);
}

void CLevel_Tool::Render()
{
}

HRESULT CLevel_Tool::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	m_pGameInstance->Add_Light(LightDesc);

	return S_OK;
}

HRESULT CLevel_Tool::Ready_Camera(const wstring strLayerTag)
{
	CEffectTool_Camera::EFFECT_TOOL_CAMERA_DESC Desc{};
	Desc.fSensor = 0.1f;
	Desc.vEye = _float4{ 0.f, 10.f, -10.f, 1.f };
	Desc.vFocus = _float4{ 0.f, 0.f, 0.f, 1.f };
	Desc.fFovY = XMConvertToRadians(60.f);
	Desc.fAspect = g_iWinSizeX / (_float)g_iWinSizeY;
	Desc.fNear = 0.1f;
	Desc.fFar = 3000.f;

	Desc.fSpeedPecSec = 30.f;
	Desc.fRotatePecSec = XMConvertToRadians(45.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_MAIN, L"Prototype_GameObject_Camera", strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Tool::Ready_Terrain(const wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_MAIN, L"Prototype_GameObject_Effect_Terrain", strLayerTag)))
		return E_FAIL;
	return S_OK;
}

HRESULT CLevel_Tool::Ready_Effect(const wstring strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_MAIN, L"Prototype_GameObject_Effect_Point", strLayerTag)))
		return E_FAIL;

	return S_OK;
}

CLevel_Tool* CLevel_Tool::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Tool* pInstance = new CLevel_Tool(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
		Safe_Release(pInstance);

	return pInstance;
}

void CLevel_Tool::Free()
{
	__super::Free();

	Safe_Release(m_pEffectManager);
}
