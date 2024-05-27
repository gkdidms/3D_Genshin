#include "GamePlay_Level.h"

#include "GameInstance.h"
#include "DefaultCamera.h"
#include "CutCamera.h"

#include "SceneObj.h"
#include "Monster.h"
#include "Player.h"
#include "Map.h"

#include "Hili.h"
#include "FireCore.h"
#include "CheckPoint.h"
#include "Plane.h"

CGamePlay_Level::CGamePlay_Level(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
{
}

HRESULT CGamePlay_Level::Initialize()
{
	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Load_File(LEVEL_GAMEPLAY)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(L"Layer_Camera")))
		return E_FAIL;

	//if (FAILED(Ready_Layer_Monster(L"Layer_Fiona")))
	//	return E_FAIL;
	if (FAILED(Ready_Layer_UI(L"Layer_UI")))
		return E_FAIL;
	return S_OK;
}

void CGamePlay_Level::Tick(const _float& fTimeDelta)
{
	SetWindowText(g_hWnd, TEXT("GamePlay 입니다."));
}

void CGamePlay_Level::Render()
{
}

HRESULT CGamePlay_Level::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(0.7f, 0.7f, 0.7f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(0.1f, 0.1f, 0.1f, 1.f);

	m_pGameInstance->Add_Light(LightDesc);


	//ZeroMemory(&LightDesc, sizeof(LIGHT_DESC));
	//LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	//LightDesc.vPosition = _float4(20.f, 5.f, 20.f, 1.f);
	//LightDesc.fRange = 20.f;
	//LightDesc.vDiffuse = _float4(1.f, 0.0f, 0.f, 1.f);
	//LightDesc.vAmbient = _float4(0.4f, 0.1f, 0.1f, 1.f);
	//LightDesc.vSpecular = LightDesc.vDiffuse;

	//m_pGameInstance->Add_Light(LightDesc);

	//ZeroMemory(&LightDesc, sizeof(LIGHT_DESC));
	//LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	//LightDesc.vPosition = _float4(40.f, 5.f, 20.f, 1.f);
	//LightDesc.fRange = 20.f;
	//LightDesc.vDiffuse = _float4(0.0f, 1.f, 0.f, 1.f);
	//LightDesc.vAmbient = _float4(0.1f, 0.4f, 0.1f, 1.f);
	//LightDesc.vSpecular = LightDesc.vDiffuse;

	//m_pGameInstance->Add_Light(LightDesc);
	return S_OK;
}

HRESULT CGamePlay_Level::Ready_Layer_Camera(const wstring& strLayerTag)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, L"Layer_Player", 0));

	CDefaultCamera::DEFAULT_CAMERA_DESC tDefaultCamera = {};

	tDefaultCamera.fSensor = 0.2f;
	tDefaultCamera.pPlayerMatrix = dynamic_cast<CTransform*>(m_pGameInstance->Get_GameObject_Component(LEVEL_GAMEPLAY, L"Layer_Player", L"Com_Transform"))->Get_WorldFloat4x4();

	tDefaultCamera.pCameraLookMatrix = pPlayer->Get_PlayerCameraLook();
	tDefaultCamera.vEye = _float4(0.f, 2.f, -3.f, 1.f);
	tDefaultCamera.vFocus = _float4(0.f, 10.f, 0.f, 1.f);
	
	tDefaultCamera.fFovY = XMConvertToRadians(60.f);
	tDefaultCamera.fAspect = g_iWinSizeX / (_float)g_iWinSizeY;
	tDefaultCamera.fNear = 0.1f;
	tDefaultCamera.fFar = 3000.f;

	tDefaultCamera.fSpeedPecSec = 20.f;
	tDefaultCamera.fRotatePecSec = XMConvertToRadians(45.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_Camera", strLayerTag, &tDefaultCamera)))
		return E_FAIL;

	CCutCamera::Cut_CAMERA_DESC tCutCamera = {};

	tCutCamera.fSensor = 0.2f;
	tCutCamera.pPlayerMatrix = dynamic_cast<CTransform*>(m_pGameInstance->Get_GameObject_Component(LEVEL_GAMEPLAY, L"Layer_Player", L"Com_Transform"))->Get_WorldFloat4x4();

	tCutCamera.pCameraLookMatrix = pPlayer->Get_PlayerCameraLook();
	tCutCamera.vEye = _float4(0.f, 2.f, -2.f, 1.f);
	tCutCamera.vFocus = _float4(0.f, 10.f, 0.f, 1.f);

	tCutCamera.fFovY = XMConvertToRadians(90.f);
	tCutCamera.fAspect = g_iWinSizeX / (_float)g_iWinSizeY;
	tCutCamera.fNear = 0.1f;
	tCutCamera.fFar = 3000.f;

	tCutCamera.fSpeedPecSec = 20.f;
	tCutCamera.fRotatePecSec = XMConvertToRadians(45.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_CutCamera", strLayerTag, &tCutCamera)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGamePlay_Level::Ready_Layer_BackGround(const wstring& strLayerTag, const char* pObjectName, void* pArg)
{
	if (!strcmp(pObjectName, "Dungeon_1"))
	{
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_Map_Dungeon_1", strLayerTag, pArg)))
			return E_FAIL;
	}
	else if (!strcmp(pObjectName, "Dungeon_2"))
	{
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_Map_Dungeon_2", strLayerTag, pArg)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CGamePlay_Level::Ready_Layer_Player(const wstring& strLayerTag, void* pArg)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_Player", strLayerTag, pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGamePlay_Level::Ready_Layer_UI(const wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_UI_PlayerHP_Outline", strLayerTag)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_UI_SkillBtn_E", strLayerTag)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_UI_SkillBtn_Q", strLayerTag)))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_UI_Avatar", strLayerTag)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CGamePlay_Level::Ready_Object(const char* strName, _matrix WorldMatrix, _uint iNavigationIndex)
{
	if (string(strName).find("Slime_Fire_Large") != string::npos)
	{
		CMonster::MONSTER_DESC Desc{};

		XMStoreFloat4x4(&Desc.WorldMatrix, WorldMatrix);
		Desc.iMonsterNavigationIndex = iNavigationIndex;
		Desc.TargetMatrix = dynamic_cast<CTransform*>(m_pGameInstance->Get_GameObject_Component(LEVEL_GAMEPLAY, L"Layer_Player", L"Com_Transform"))->Get_WorldFloat4x4();

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_Monster_Slime_Fire_Large", L"Layer_Monster", &Desc)))
			return E_FAIL;
	}
	else if (string(strName).find("Slime_Fire_Mid") != string::npos)
	{
		CMonster::MONSTER_DESC Desc{};

		XMStoreFloat4x4(&Desc.WorldMatrix, WorldMatrix);
		Desc.iMonsterNavigationIndex = iNavigationIndex;
		Desc.TargetMatrix = dynamic_cast<CTransform*>(m_pGameInstance->Get_GameObject_Component(LEVEL_GAMEPLAY, L"Layer_Player", L"Com_Transform"))->Get_WorldFloat4x4();

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_Monster_Slime_Fire_Mid", L"Layer_Monster", &Desc)))
			return E_FAIL;
	}
	else if (string(strName).find("Slime_Water_Large") != string::npos)
	{
		CMonster::MONSTER_DESC Desc{};

		XMStoreFloat4x4(&Desc.WorldMatrix, WorldMatrix);
		Desc.iMonsterNavigationIndex = iNavigationIndex;
		Desc.TargetMatrix = dynamic_cast<CTransform*>(m_pGameInstance->Get_GameObject_Component(LEVEL_GAMEPLAY, L"Layer_Player", L"Com_Transform"))->Get_WorldFloat4x4();
		
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_Monster_Slime_Water_Large", L"Layer_Monster", &Desc)))
			return E_FAIL;
	}
	else if (string(strName).find("Slime_Water_Mid") != string::npos)
	{
		CMonster::MONSTER_DESC Desc{};

		XMStoreFloat4x4(&Desc.WorldMatrix, WorldMatrix);
		Desc.iMonsterNavigationIndex = iNavigationIndex;
		Desc.TargetMatrix = dynamic_cast<CTransform*>(m_pGameInstance->Get_GameObject_Component(LEVEL_GAMEPLAY, L"Layer_Player", L"Com_Transform"))->Get_WorldFloat4x4();

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_Monster_Slime_Water_Mid", L"Layer_Monster", &Desc)))
			return E_FAIL;
	}
	else if (string(strName).find("TreasureBox") != string::npos)
	{
		CSceneObj::SCENEOBJ_DESC Desc{};

		XMStoreFloat4x4(&Desc.WorldMatrix, WorldMatrix);

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_SceneObj_TreasureBox", L"Layer_SceneObj", &Desc)))
			return E_FAIL;

	}
	else if (string(strName).find("FireCore") != string::npos)
	{
		CFireCore::FIRECORE_DESC Desc{};
		Desc.pTargetMatrix = dynamic_cast<CTransform*>(m_pGameInstance->Get_GameObject_Component(LEVEL_GAMEPLAY, L"Layer_Player", L"Com_Transform"))->Get_WorldFloat4x4();
		XMStoreFloat4x4(&Desc.WorldMatrix, WorldMatrix);

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_SceneObj_FireCore", L"Layer_SceneObj", &Desc)))
			return E_FAIL;
	}
	else if (string(strName).find("DungeonGate") != string::npos)
	{
		CSceneObj::SCENEOBJ_DESC Desc{};

		XMStoreFloat4x4(&Desc.WorldMatrix, WorldMatrix);

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_SceneObj_DungeonGate", L"Layer_SceneObj", &Desc)))
			return E_FAIL;
	}
	else if (string(strName).find("CheckPoint") != string::npos)
	{
		CCheckPoint::CHECKPOINT_DESC Desc{};

		Desc.pTargetMatrix = dynamic_cast<CTransform*>(m_pGameInstance->Get_GameObject_Component(LEVEL_GAMEPLAY, L"Layer_Player", L"Com_Transform"))->Get_WorldFloat4x4();
		XMStoreFloat4x4(&Desc.WorldMatrix, WorldMatrix);

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_SceneObj_CheckPoint", L"Layer_SceneObj", &Desc)))
			return E_FAIL;
	}
	else if (string(strName).find("Operator") != string::npos)
	{
		CSceneObj::SCENEOBJ_DESC Desc{};

		XMStoreFloat4x4(&Desc.WorldMatrix, WorldMatrix);

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_SceneObj_Operator", L"Layer_SceneObj", &Desc)))
			return E_FAIL;
	}
	else if (string(strName).find("WindField") != string::npos)
	{
		CSceneObj::SCENEOBJ_DESC Desc{};

		XMStoreFloat4x4(&Desc.WorldMatrix, WorldMatrix);

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_SceneObj_WindField", L"Layer_SceneObj", &Desc)))
			return E_FAIL;
	}
	else if (string(strName).find("ThornWall") != string::npos)
	{
		CSceneObj::SCENEOBJ_DESC Desc{};

		XMStoreFloat4x4(&Desc.WorldMatrix, WorldMatrix);

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_SceneObj_ThornWall", L"Layer_SceneObj", &Desc)))
			return E_FAIL;
	}
	else if (string(strName).find("MovePlane") != string::npos)
	{
		CPlane::PLANE_DESC Desc{};

		XMStoreFloat4x4(&Desc.WorldMatrix, WorldMatrix);
		Desc.isMove = true;
		Desc.iNavigationIndex = iNavigationIndex;

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_SceneObj_Plane", L"Layer_SceneObj", &Desc)))
			return E_FAIL;
	}
	else if (string(strName).find("Plane") != string::npos)
	{
		CPlane::PLANE_DESC Desc{};

		XMStoreFloat4x4(&Desc.WorldMatrix, WorldMatrix);
		Desc.isMove = false;

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_SceneObj_Plane", L"Layer_SceneObj", &Desc)))
			return E_FAIL;
	}
	else if (string(strName).find("Hili_Fire") != string::npos)
	{
		CHili::HILI_DESC Desc{};

		XMStoreFloat4x4(&Desc.WorldMatrix, WorldMatrix);
		Desc.iMonsterNavigationIndex = iNavigationIndex;
		Desc.TargetMatrix = dynamic_cast<CTransform*>(m_pGameInstance->Get_GameObject_Component(LEVEL_GAMEPLAY, L"Layer_Player", L"Com_Transform"))->Get_WorldFloat4x4();

		if (string(strName).find("Bow") != string::npos)
			Desc.eWeapon = CHili::HILI_WEAPON_CROSSBOW;
		else if (string(strName).find("Club") != string::npos) return S_OK;
			//Desc.eWeapon = CHili::HILI_WEAPON_CLUB;

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_Monster_Hili_Fire", L"Layer_Monster", &Desc)))
			return E_FAIL;
	}
	else if (string(strName).find("Hili_Electric") != string::npos)
	{
		CHili::HILI_DESC Desc{};

		XMStoreFloat4x4(&Desc.WorldMatrix, WorldMatrix);
		Desc.iMonsterNavigationIndex = iNavigationIndex;
		Desc.TargetMatrix = dynamic_cast<CTransform*>(m_pGameInstance->Get_GameObject_Component(LEVEL_GAMEPLAY, L"Layer_Player", L"Com_Transform"))->Get_WorldFloat4x4();

		if (string(strName).find("Bow") != string::npos)
			Desc.eWeapon = CHili::HILI_WEAPON_CROSSBOW;
		else if (string(strName).find("Club") != string::npos)
			Desc.eWeapon = CHili::HILI_WEAPON_CLUB;

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_Monster_Hili_Electric", L"Layer_Monster_Hili_Electric", &Desc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CGamePlay_Level::Load_File(LEVEL_STATE eNextLevel)
{
	char pFilePath[MAX_PATH] = "";
	strcpy_s(pFilePath, "../../Data/Stage/Stage_1.dat");

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
