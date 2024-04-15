#include "Loader.h"

#include "GameInstance.h"

#include "DefaultCamera.h"
#include "Terrain.h"
#include "Monster.h"

#include "Player.h"
#include "Tighnari_Body.h"
#include "Yae_Body.h"
#include "Wanderer_Body.h"
#include "Nilou_Body.h"

#include "Dungeon_1.h"
#include "Dungeon_2.h"

CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pContext{pContext}, m_pDevice{pDevice} , m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pGameInstance);
}

_uint APIENTRY Loading_Main(void* pArg)
{
	CLoader* pLoader = (CLoader*)pArg;

	if (FAILED(pLoader->Loading()))
		return 1;

	return 0;
}

HRESULT CLoader::Initialize(LEVEL_STATE eNextLevel)
{
	m_eNextLevel = eNextLevel;

	InitializeCriticalSection(&m_Critical_Section);

	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Loading_Main, this, 0, nullptr);
	if (m_hThread == nullptr)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Loading()
{
	
	EnterCriticalSection(&m_Critical_Section);

	if (LEVEL_LOGO == m_eNextLevel)
		Loading_For_Logo();
	else if (LEVEL_GAMEPLAY == m_eNextLevel)
		Loading_For_GamePlay();

	LeaveCriticalSection(&m_Critical_Section);

	return S_OK;
}

HRESULT CLoader::Loading_For_Logo()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));


	lstrcpy(m_szLoadingText, TEXT("모델를(을) 로딩 중 입니다."));


	lstrcpy(m_szLoadingText, TEXT("셰이더를(을) 로딩 중 입니다."));


	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));

	CLoader::m_isFinished = true;
	return S_OK;
}

HRESULT CLoader::Loading_For_GamePlay()
{
	lstrcpy(m_szLoadingText, TEXT("텍스쳐를 로딩 중 입니다."));
	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Terrain", CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile0.dds"), 1))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("모델를(을) 로딩 중 입니다."));
	//_matrix PreTransformMatrix;
	//PreTransformMatrix = XMMatrixScaling(1.f, 1.f, 1.f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
 //	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Fiona", CModel::Create(m_pDevice, m_pContext, CMesh::TYPE_ANIM, "../Bin/Resources/Models/Fiona/Fiona.fbx", PreTransformMatrix, "../../Data/Fiona.dat", CModel::CREATE_READ))))
	//	return E_FAIL;

	_matrix PreTransformMatrix = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Tighnari", CModel::Create(m_pDevice, m_pContext, CMesh::TYPE_ANIM, "../Bin/Resources/Models/Player/Tighnari/Tighnari.fbx", PreTransformMatrix, "../../Data/Tighnari.dat", CModel::CREATE_READ))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Nilou", CModel::Create(m_pDevice, m_pContext, CMesh::TYPE_ANIM, "../Bin/Resources/Models/Player/Nilou/Nilou.fbx", PreTransformMatrix, "../../Data/Nilou.dat", CModel::CREATE_READ))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Wanderer", CModel::Create(m_pDevice, m_pContext, CMesh::TYPE_ANIM, "../Bin/Resources/Models/Player/Wanderer/Wanderer.fbx", PreTransformMatrix, "../../Data/Wanderer.dat", CModel::CREATE_READ))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Yae", CModel::Create(m_pDevice, m_pContext, CMesh::TYPE_ANIM, "../Bin/Resources/Models/Player/Yae/Yae.fbx", PreTransformMatrix, "../../Data/Yae.dat", CModel::CREATE_READ))))
		return E_FAIL;

	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Dungeon_1", CModel::Create(m_pDevice, m_pContext, CMesh::TYPE_NONANIM, "../Bin/Resources/Models/Map/Dungeon_1/Dungeon_1.fbx", PreTransformMatrix, "../../Data/Dungeon_1.dat", CModel::CREATE_READ))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Dungeon_2", CModel::Create(m_pDevice, m_pContext, CMesh::TYPE_NONANIM, "../Bin/Resources/Models/Map/Dungeon_2/Dungeon_2.fbx", PreTransformMatrix, "../../Data/Dungeon_2.dat", CModel::CREATE_READ))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("셰이더를(을) 로딩 중 입니다."));
	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxNorTex", CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFile/Shader_VtxNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxMesh", CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFile/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimMesh", CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFile/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("컴포넌트 로딩 중 입니다."));
	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_VIBuffer_Terrain", CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height1.bmp")))))
		return E_FAIL;

	/*객체 원형 로딩*/
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));
	if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_Camera", CDefaultCamera::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_Terrain", CTerrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_Fiona", CMonster::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_Player", CPlayer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_Player_Tighnari", CTighnari_Body::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_Player_Nilou", CNilou_Body::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_Player_Wanderer", CWanderer_Body::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_Player_Yae", CYae_Body::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_Map_Dungeon_1", CDungeon_1::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_Map_Dungeon_2", CDungeon_2::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//게임 오브젝트 준비
	lstrcpy(m_szLoadingText, TEXT("오브젝트 클론을 로딩 중 입니다."));
	if (FAILED(Open_File(LEVEL_GAMEPLAY)))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("로딩이 완료되었습니다."));
	m_isFinished = true;

	return S_OK;
}

CLoader* CLoader::Create(LEVEL_STATE eNextLevel, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLoader* pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel)))
		Safe_Release(pInstance);

	return pInstance;
}

void CLoader::Free()
{
	DeleteCriticalSection(&m_Critical_Section);

	WaitForSingleObject(m_hThread, INFINITE);
	DeleteObject(m_hThread);
	CloseHandle(m_hThread);

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
	Safe_Release(m_pGameInstance);
}

HRESULT CLoader::Open_File(LEVEL_STATE eNextLevel)
{
	char pFilePath[MAX_PATH] = "";
	strcpy_s(pFilePath, eNextLevel == LEVEL_GAMEPLAY ? "../../Data/Stage/Stage_1.dat" : "../../Data/Stage/Stage_1.dat");

	ifstream ifs(pFilePath, ios::binary | ios::in);

	if (ifs.fail())
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

		Ready_Object(pDungeonName, L"Layer_Map");
	}

	_uint iNumObjects = { 0 };
	ifs.read((_char*)&iNumObjects, sizeof(_uint));

	for (int i = 0; i < iNumObjects; ++i)
	{
		char strObjectName[MAX_PATH] = { "" };
		_uint iNumObjectName = { 0 };

		ifs.read((_char*)&iNumObjectName, sizeof(_uint));
		ifs.read((_char*)strObjectName, iNumObjectName);

		_uint iNumObjectIndex = { 0 };

		//find_if(m_CloneDesc[OBJECT_MONSTER].begin(), m_CloneDesc[OBJECT_MONSTER].end(), [&](CLONE_DESC Desc)->_bool {
		//	if (!strcmp(strObjectName, Desc.strName.c_str()))
		//	{
		//		iNumObjectIndex = Desc.iIndex;
		//		return true;
		//	}

		//	return false;
		//	});

		//Add_CloneObject(OBJECT_MONSTER, L"Layer_Object", XMVectorSet(0.f, 0.f, 0.f, 1.f), iNumObjectIndex);

		//_float4x4 WorldMatrix = {};
		//ifs.read((_char*)&WorldMatrix, sizeof(_float4x4));

		//m_Objects[i]->m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&WorldMatrix));
	}

	ifs.close();
	return S_OK;
}

HRESULT CLoader::Ready_Object(const char* pObjectName, const wstring& strLayerTag)
{
	if (!strcmp(pObjectName, "Dungeon_1"))
	{
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_Map_Dungeon_1", strLayerTag)))
			return E_FAIL;
	}
	else if (!strcmp(pObjectName, "Dungeon_2"))
	{
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_Map_Dungeon_2", strLayerTag)))
			return E_FAIL;
	}

	return S_OK;
}
