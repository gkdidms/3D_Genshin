#include "MainTool.h"
#include "GameInstance.h"

#include "Main_Level.h"
#include "Tool_Terrain.h"
#include "Tool_Camera.h"
#include "Tool_Object.h"

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
    // ¸ðµ¨
    _matrix PreTransformMatrix = XMMatrixIdentity();
    //if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Model_Fiona", CModel::Create(m_pDevice, m_pContext, CMesh::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Fiona/Fiona.fbx", PreTransformMatrix))))
    //    return E_FAIL;
    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Model_Tarta", CModel::Create(m_pDevice, m_pContext, CMesh::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Tarta/Tartaglia.fbx", PreTransformMatrix))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Model_Nillou", CModel::Create(m_pDevice, m_pContext, CMesh::TYPE_NONANIM, "../../Client/Bin/Resources/Models/Nillou/Nilou.fbx", PreTransformMatrix))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Model_Dungeon_1", CModel::Create(m_pDevice, m_pContext, CMesh::TYPE_NONANIM, "../../Client/Bin/Resources/Models/map/Dungeon_1/Dungeon_1.fbx", PreTransformMatrix))))
        return E_FAIL;

    //buffer
    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_VIBuffer_Terrain", CVIBuffer_Terrain::Create(m_pDevice, m_pContext, L"../../Client/Bin/Resources/Textures/Terrain/Height1.bmp"))))
        return E_FAIL;
    
    // Shader
    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Shader", CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFile/Shader_VtxNorTex.hlsl", VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Shader_VtxMesh", CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFile/Shader_VtxMesh.hlsl", VTXMESH::Elements, VTXMESH::iNumElements))))
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

    if (FAILED(m_pGameInstance->Add_GameObject_Prototype(L"Prototype_GameObject_Object", CTool_Object::Create(m_pDevice, m_pContext))))
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
