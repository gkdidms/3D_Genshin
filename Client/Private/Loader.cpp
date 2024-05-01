#include "Loader.h"

#include "GameInstance.h"

#include "DefaultCamera.h"
#include "Terrain.h"

#include "Player.h"
#include "Tighnari_Body.h"
#include "Nilou_Body.h"
#include "Wanderer_Body.h"
#include "Yae_Body.h"

#include "Weapon_Ayus.h"
#include "Weapon_Alaya.h"
#include "Weapon_Regalis.h"
#include "Weapon_Narukami.h"

#include "SkillObj_Gohei.h"

#include "Dungeon_1.h"
#include "Dungeon_2.h"

#include "Slime_Large_Fire.h"
#include "Slime_Large_Water.h"
#include "Slime_Mid_Fire.h"
#include "Slime_Mid_Water.h"

#include "TreasureBox.h"

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

	lstrcpy(m_szLoadingText, TEXT("플레이어 모델를(을) 로딩 중 입니다."));

	_matrix PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) *  XMMatrixRotationY(XMConvertToRadians(180.0f));
	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Tighnari", CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Player/Tighnari/Tighnari.fbx", PreTransformMatrix, "../../Data/Tighnari.dat"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Nilou", CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Player/Nilou/Nilou.fbx", PreTransformMatrix, "../../Data/Nilou.dat"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Wanderer", CModel::Create(m_pDevice, m_pContext,  "../Bin/Resources/Models/Player/Wanderer/Wanderer.fbx", PreTransformMatrix, "../../Data/Wanderer.dat"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Yae", CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Player/Yae/Yae.fbx", PreTransformMatrix, "../../Data/Yae.dat"))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("몬스터를(을) 로딩 중 입니다."));
	PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Slime_Fire", CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Slime_Fire/Slime_Fire.fbx", PreTransformMatrix, "../../Data/Slime_Fire.dat"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Slime_Water", CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Monster/Slime_Water/Slime_Water.fbx", PreTransformMatrix, "../../Data/Slime_Water.dat"))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("씬 오브젝트를(을) 로딩 중 입니다."));
	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_TreasureBox", CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/SceneObj/TreasureBox/TreasureBox.fbx", PreTransformMatrix, "../../Data/TreasureBox.dat"))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("무기 모델를(을) 로딩 중 입니다."));
	PreTransformMatrix = XMMatrixIdentity();
	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Ayus", CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Weapon/Ayus/Ayus.fbx", PreTransformMatrix, "../../Data/Weapon_Ayus.dat"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Alaya", CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Weapon/Alaya/Alaya.fbx", PreTransformMatrix, "../../Data/Weapon_Alaya.dat"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Narukami", CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Weapon/Narukami/Narukami.fbx", PreTransformMatrix, "../../Data/Weapon_Narukami.dat"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Regalis", CModel::Create(m_pDevice, m_pContext,  "../Bin/Resources/Models/Weapon/Regalis/Regalis.fbx", PreTransformMatrix, "../../Data/Weapon_Regalis.dat"))))
		return E_FAIL;

	lstrcpy(m_szLoadingText, TEXT("스킬 오브젝트 모델를(을) 로딩 중 입니다."));
	
	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Gohei", CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/SkillObj/Yae_Gohei/Gohei.fbx", PreTransformMatrix, "../../Data/SkillObj_Gohei.dat"))))
		return E_FAIL;


	lstrcpy(m_szLoadingText, TEXT("맵 모델를(을) 로딩 중 입니다."));

	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Dungeon_1", CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Dungeon_1/Dungeon_1.fbx", PreTransformMatrix, "../../Data/Dungeon_1.dat"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Dungeon_2", CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Map/Dungeon_2/Dungeon_2.fbx", PreTransformMatrix, "../../Data/Dungeon_2.dat"))))
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

	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Navigation_Stage_1", CNavigation::Create(m_pDevice, m_pContext, TEXT("../../Data/Navigation/Navigation_Stage_1.dat")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_GAMEPLAY, L"Prototype_Component_Collider", CCollider::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/*객체 원형 로딩*/
	lstrcpy(m_szLoadingText, TEXT("객체원형을 로딩 중 입니다."));
	if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_Camera", CDefaultCamera::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_Terrain", CTerrain::Create(m_pDevice, m_pContext))))
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

	if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_Weapon_Ayus", CWeapon_Ayus::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_Weapon_Alaya", CWeapon_Alaya::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_Weapon_Narukami", CWeapon_Narukami::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_Weapon_Regalis", CWeapon_Regalis::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_Map_Dungeon_1", CDungeon_1::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_Map_Dungeon_2", CDungeon_2::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_SkillObj_Gohei", CSkillObj_Gohei::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_Monster_Slime_Fire_Large", CSlime_Large_Fire::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_Monster_Slime_Fire_Mid", CSlime_Mid_FIre::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_Monster_Slime_Water_Large", CSlime_Large_Water::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_Monster_Slime_Water_Mid", CSlime_Mid_Water::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_SceneObj_TreasureBox", CTreasureBox::Create(m_pDevice, m_pContext))))
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