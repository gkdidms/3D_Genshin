#include "MainTool.h"

#include "GameInstance.h"
#include "Main_Level.h"
#include "Tool_Terrain.h"
#include "Tool_Camera.h"
#include "Tool_Non_Object.h"
#include "Tool_Dungeon.h"
#include "Tool_Anim_Object.h"

CMainTool::CMainTool()
    :m_pGameInstance { CGameInstance::GetInstance() },
    m_pTool_Manager { CTool_Manager::GetInstance() }
{
    Safe_AddRef(m_pGameInstance);
    Safe_AddRef(m_pTool_Manager);
}

HRESULT CMainTool::Initialize()
{
    ENGINE_DESC tEngineDesc{};
    tEngineDesc.iWinSizeX = g_iWinSizeX;
    tEngineDesc.iWinSizeY = g_iWinSizeY;
    tEngineDesc.isWindowed = true;
    tEngineDesc.hWnd = g_hWnd;
    tEngineDesc.hInst = g_hInst;

    if (FAILED(m_pGameInstance->Initialize_Engine(LEVEL_END, tEngineDesc, &m_pDevice, &m_pContext)))
        return E_FAIL;

    if (FAILED(m_pTool_Manager->Initialize(m_pDevice, m_pContext)))
        return E_FAIL;
    
    if (FAILED(Ready_Level_For_Main()))
        return E_FAIL;

    if (FAILED(Open_Level(LEVEL_MAIN)))
        return E_FAIL;

	return S_OK;
}

void CMainTool::Tick(const _float& fTimeDelta)
{
    m_pGameInstance->Tick(fTimeDelta);

    m_pTool_Manager->Tick(fTimeDelta);
}

void CMainTool::Render()
{
    m_pGameInstance->Clear_BackBuffer_View(_float4{ 0.f, 0.f, 1.f, 1.f });
    m_pGameInstance->Clear_DepthStencil_View();

    m_pGameInstance->Draw_From_Tool();
    m_pTool_Manager->Render();

    m_pGameInstance->Present();
}

HRESULT CMainTool::Open_Level(LEVEL_STATE eLevelState)
{
    if (FAILED(m_pGameInstance->Open_Level(eLevelState, CMain_Level::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    return S_OK;
}

HRESULT CMainTool::Ready_Level_For_Main()
{
    // 
    _matrix PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) *  XMMatrixRotationY(XMConvertToRadians(180.0f));
    
    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Model_Slime_Fire", CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/Monster/Slime_Fire/Slime_Fire.fbx", PreTransformMatrix, "../../Data/Slime_Fire.dat"))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Model_Slime_Water", CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/Monster/Slime_Water/Slime_Water.fbx", PreTransformMatrix, "../../Data/Slime_Water.dat"))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Model_TreasureBox", CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/SceneObj/TreasureBox/TreasureBox.fbx", PreTransformMatrix, "../../Data/TreasureBox.dat"))))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Model_TreasureBox_Big", CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/SceneObj/TreasureBox_Big/TreasureBox_Big.fbx", PreTransformMatrix, "../../Data/TreasureBox_Big.dat"))))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Model_ChecKPoint", CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/SceneObj/CheckPoint/CheckPoint.fbx", PreTransformMatrix, "../../Data/SceneObj_CheckPoint.dat"))))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Model_WindField", CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/SceneObj/WindFleid/WindFleid.fbx", PreTransformMatrix, "../../Data/SceneObj_WindField.dat"))))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Model_FireCore", CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/SceneObj/FireCore/FireCore.fbx", PreTransformMatrix, "../../Data/SceneObj_FireCore.dat"))))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Model_DungeonGate", CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/SceneObj/Gate/DungeonGate.fbx", PreTransformMatrix, "../../Data/SceneObj_Gate.dat"))))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Model_Operator", CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/SceneObj/Operator/Operator.fbx", PreTransformMatrix, "../../Data/SceneObj_Operator.dat"))))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Model_Plane", CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/SceneObj/Plane/Plane.fbx", PreTransformMatrix, "../../Data/SceneObj_Plane.dat"))))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Model_ThornWall", CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/SceneObj/ThornWall/ThornWall.fbx", PreTransformMatrix, "../../Data/SceneObj_ThornWall.dat"))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Model_Hili_Fire", CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/Monster/Hili_Fire/Hili_Fire.fbx", PreTransformMatrix, "../../Data/Hili_Fire.dat"))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Model_Hili_Electric", CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/Monster/Hili_Electric/Hili_Electric.fbx", PreTransformMatrix, "../../Data/Hili_Electric.dat"))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Model_Boss_Tarta", CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/Boss/Tarta/Tartaglia.fbx", PreTransformMatrix, "../../Data/Boss_Tartaglia.dat"))))
        return E_FAIL;

    PreTransformMatrix = XMMatrixIdentity();
    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Model_Dungeon_1", CModel::Create(m_pDevice, m_pContext,  "../../Client/Bin/Resources/Models/Map/Dungeon_1/Dungeon_1.fbx", PreTransformMatrix, "../../Data/Dungeon_1.dat"))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Model_Dungeon_2", CModel::Create(m_pDevice, m_pContext,  "../../Client/Bin/Resources/Models/Map/Dungeon_2/Dungeon_2.fbx", PreTransformMatrix, "../../Data/Dungeon_2.dat"))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Model_GoldenHouse", CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/Resources/Models/Map/GoldenHouse/GoldenHouse.fbx", PreTransformMatrix, "../../Data/GoldenHouse.dat"))))
        return E_FAIL;

    //buffer
    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_VIBuffer_Terrain", CVIBuffer_Terrain::Create(m_pDevice, m_pContext, L"../../Client/Bin/Resources/Textures/Terrain/Height1.bmp"))))
        return E_FAIL;
    
    // Shader
    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Shader", CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxNorTex.hlsl", VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Shader_VtxMesh", CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxMesh.hlsl", VTXMESH::Elements, VTXMESH::iNumElements))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Shader_VtxAnimMesh", CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl", VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Navigation", CNavigation::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    // Texture
    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Texture", CTexture::Create(m_pDevice, m_pContext, L"../../Client/Bin/Resources/Textures/Terrain/Tile0.jpg", 1))))
        return E_FAIL;

    // Calculator
    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Calculator", CCalculator::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    // °´Ã¼ ¿øÇü
    if(FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_Terrain", CTool_Terrain::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_Camera", CTool_Camera::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_Object", CTool_Non_Object::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_AnimObject", CTool_Anim_Object::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_Dungeon", CTool_Dungeon::Create(m_pDevice, m_pContext))))
        return E_FAIL;


    return S_OK;
}

CMainTool* CMainTool::Create()
{
	CMainTool* pInstance = new CMainTool();

	if (FAILED(pInstance->Initialize()))
		Safe_Release(pInstance);

	return pInstance;
}

void CMainTool::Free()
{
    m_pTool_Manager->Release_Tool();
    Safe_Release(m_pTool_Manager);

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);

    m_pGameInstance->Release_Engine();
    Safe_Release(m_pGameInstance);
}
