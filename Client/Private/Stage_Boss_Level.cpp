#include "Stage_Boss_Level.h"

#include "Loading_Level.h"

#include "GameInstance.h"
#include "DefaultCamera.h"
#include "Player.h"
#include "Map.h"

#include "SceneObj.h"

#include "Boss.h"

CStage_Boss_Level::CStage_Boss_Level(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext}
{
}

CStage_Boss_Level::CStage_Boss_Level(const CStage_Boss_Level& rhs)
	: CLevel{rhs}
{
}

HRESULT CStage_Boss_Level::Initialize()
{
	if (FAILED(Load_File()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(L"Layer_Camera")))
		return E_FAIL;

	return S_OK;
}

void CStage_Boss_Level::Tick(const _float& fTimeDelta)
{
	SetWindowText(g_hWnd, TEXT("Boss Stage 입니다."));

	if (GetAsyncKeyState(VK_RETURN))
	{
		m_pGameInstance->Open_Level(LEVEL_LOADING, CLoading_Level::Create(LEVEL_GAMEPLAY, m_pDevice, m_pContext));
	}
}

void CStage_Boss_Level::Render()
{
}

HRESULT CStage_Boss_Level::Ready_Layer_Camera(const wstring& strLayerTag)
{
	CDefaultCamera::DEFAULT_CAMERA_DESC tDefaultCamera = {};

	tDefaultCamera.fSensor = 0.2f;
	tDefaultCamera.pPlayerMatrix = dynamic_cast<CTransform*>(m_pGameInstance->Get_GameObject_Component(LEVEL_GAMEPLAY, L"Layer_Player", L"Com_Transform"))->Get_WorldFloat4x4();

	tDefaultCamera.pCameraLookMatrix = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, L"Layer_Player", 0))->Get_PlayerCameraLook();
	tDefaultCamera.vEye = _float4(0.f, 2.f, -2.f, 1.f);
	tDefaultCamera.vFocus = _float4(0.f, 10.f, 0.f, 1.f);

	tDefaultCamera.fFovY = XMConvertToRadians(90.f);
	tDefaultCamera.fAspect = g_iWinSizeX / (_float)g_iWinSizeY;
	tDefaultCamera.fNear = 0.1f;
	tDefaultCamera.fFar = 3000.f;

	tDefaultCamera.fSpeedPecSec = 20.f;
	tDefaultCamera.fRotatePecSec = XMConvertToRadians(45.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_Camera", strLayerTag, &tDefaultCamera)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage_Boss_Level::Ready_Layer_BackGround(const wstring& strLayerTag, const char* pObjectName, void* pArg)
{
	if (!strcmp(pObjectName, "GoldenHouse"))
	{
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_Map_GoldenHouse", strLayerTag, pArg)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CStage_Boss_Level::Ready_Layer_Player(const wstring& strLayerTag, void* pArg)
{
 	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_Player", strLayerTag, pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CStage_Boss_Level::Ready_Object(const char* strName, _matrix WorldMatrix, _uint iNavigationIndex)
{
	if (string(strName).find("Boss_Tarta") != string::npos)
	{
		CBoss::BOSS_DESC Desc{};

		XMStoreFloat4x4(&Desc.WorldMatrix, WorldMatrix);
		Desc.iBossNavigationIndex = iNavigationIndex;
		Desc.TargetMatrix= dynamic_cast<CTransform*>(m_pGameInstance->Get_GameObject_Component(LEVEL_GAMEPLAY, L"Layer_Player", L"Com_Transform"))->Get_WorldFloat4x4();

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_Boss", L"Layer_Boss", &Desc)))
			return E_FAIL;
	}
	else if (string(strName).find("TreasureBox") != string::npos)
	{
		CSceneObj::SCENEOBJ_DESC Desc{};

		XMStoreFloat4x4(&Desc.WorldMatrix, WorldMatrix);

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_SceneObj_TreasureBox", L"Layer_TreasureBox", &Desc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CStage_Boss_Level::Load_File()
{
	char pFilePath[MAX_PATH] = "";
	strcpy_s(pFilePath, "../../Data/Stage/Stage_Boss.dat");

	ifstream ifs(pFilePath, ios::binary | ios::in);

	if (ifs.fail())
		return E_FAIL;

	//플레이어
	_float3 vPlayerPos = {};
	_int iPlayerNavigationIndex = { 0 };
	ifs.read((_char*)&vPlayerPos, sizeof(_float3));
	ifs.read((_char*)&iPlayerNavigationIndex, sizeof(_int));

	CPlayer::PLAYER_DESC Desc = {};
	Desc.vPlayerPos = vPlayerPos;
	Desc.iPlayerNavigationIndex = iPlayerNavigationIndex;
	Desc.fSpeedPecSec = 20.f;
	Desc.fRotatePecSec = XMConvertToRadians(60.f);

	//플레이어 생성
	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"), &Desc)))
		return E_FAIL;

	//지형 (하나)
	_uint iNumDungeon = { 0 };
	ifs.read((_char*)&iNumDungeon, sizeof(_uint));

	if (iNumDungeon != 0)
	{
		_uint iNumDungeonObjectName = { 0 };
		char pDungeonName[MAX_PATH] = { "" };

		ifs.read((_char*)&iNumDungeonObjectName, sizeof(_uint));
		ifs.read((_char*)pDungeonName, iNumDungeonObjectName);

		_float4x4 WorldMatrix = {};
		ifs.read((_char*)&WorldMatrix, sizeof(_float4x4));

		CMap::MAP_DESC MapDesc = {};
		MapDesc.WorldMatrix = WorldMatrix;

		if (FAILED(Ready_Layer_BackGround(TEXT("Layer_Map"), pDungeonName, &MapDesc)))
			return E_FAIL;
	}

	_uint iNumObjects = { 0 };
	ifs.read((_char*)&iNumObjects, sizeof(_uint));

	for (int i = 0; i < iNumObjects; ++i)
	{
		char strObjectName[MAX_PATH] = { "" };
		_uint iNumObjectName = { 0 };

		ifs.read((_char*)&iNumObjectName, sizeof(_uint));
		ifs.read((_char*)strObjectName, iNumObjectName);

		_uint iNavigationIndex = 0;
		ifs.read((_char*)&iNavigationIndex, sizeof(_uint));

		_float4x4 WorldMatrix = {};
		ifs.read((_char*)&WorldMatrix, sizeof(_float4x4));

		Ready_Object(strObjectName, XMLoadFloat4x4(&WorldMatrix), iNavigationIndex);
	}

	ifs.close();
	return S_OK;
}

CStage_Boss_Level* CStage_Boss_Level::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CStage_Boss_Level* pInstance = new CStage_Boss_Level(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
		Safe_Release(pInstance);

	return pInstance;
}

void CStage_Boss_Level::Free()
{
	__super::Free();
}
