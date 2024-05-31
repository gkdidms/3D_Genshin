#include "MainEffect.h"

#include "Level_Tool.h"

#include "Effect_Manager.h"

#include "EffectTool_Camera.h"
#include "Tool_Effect_Point.h"
#include "EffectTool_Terrain.h"

#include "Effect_Default.h"
#include "Effect_Mesh.h"
#include "Effect_Trail.h"

CMainEffect::CMainEffect()
    : m_pGameInstance { CGameInstance::GetInstance() },
    m_pEffectTool_Manager { CEffectTool_Manager::GetInstance() },
    m_pEffectManager { CEffect_Manager::GetInstance()}
{
    Safe_AddRef(m_pGameInstance);
    Safe_AddRef(m_pEffectTool_Manager);
    Safe_AddRef(m_pEffectManager);
}

HRESULT CMainEffect::Initialize()
{
    ENGINE_DESC tEngineDesc{};
    tEngineDesc.iWinSizeX = g_iWinSizeX;
    tEngineDesc.iWinSizeY = g_iWinSizeY;
    tEngineDesc.isWindowed = true;
    tEngineDesc.hWnd = g_hWnd;
    tEngineDesc.hInst = g_hInst;

    if (FAILED(m_pGameInstance->Initialize_Engine(LEVEL_END, tEngineDesc, &m_pDevice, &m_pContext)))
        return E_FAIL;

    m_pEffectTool_Manager->Initialize(m_pDevice, m_pContext);

    if (FAILED(Loading_Components()))
        return E_FAIL;
    if (FAILED(Loading_GameObjects()))
        return E_FAIL;
    if (FAILED(Open_Level(LEVEL_MAIN)))
        return E_FAIL;

	return S_OK;
}

void CMainEffect::Tick(const _float& fTimeDelta)
{
    m_pGameInstance->Tick(fTimeDelta);
    m_pEffectTool_Manager->Tick(fTimeDelta);
}

HRESULT CMainEffect::Open_Level(LEVEL_STATE eLevelState)
{
    if (FAILED(m_pGameInstance->Open_Level(eLevelState, CLevel_Tool::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    return S_OK;
}

HRESULT CMainEffect::Loading_Components()
{
    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Shader_Instance_Point", CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxInstance_Point.hlsl"), VTXINSTANCE_POINT::Elements, VTXINSTANCE_POINT::iNumElements))))
        return E_FAIL;

    CVIBuffer_Instance::INSTANCE_DESC		InstanceDesc{};
    InstanceDesc.iNumInstance = 300;
    InstanceDesc.vOffsetPos = _float3(0.0f, -3.f, 0.0f);
    InstanceDesc.vPivotPos = _float3(0.0f, 0.f, 0.0f);
    InstanceDesc.vRange = _float3(0.5f, 0.5f, 0.5f);
    InstanceDesc.vSize = _float2(0.2f, 0.4f);
    InstanceDesc.vSpeed = _float2(1.f, 7.f);
    InstanceDesc.vLifeTime = _float2(1.f, 3.f);
    InstanceDesc.isLoop = false;

    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_VIBuffer_Instance_Point", CVIBuffer_Instance_Point::Create(m_pDevice, m_pContext, InstanceDesc))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Test", CTexture::Create(m_pDevice, m_pContext, TEXT("../../Client/Bin/Resources/Textures/Snow.png"), 1))))
        return E_FAIL;

#pragma region  Terrain
    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Shader", CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxNorTex.hlsl", VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Texture", CTexture::Create(m_pDevice, m_pContext, L"../../Client/Bin/Resources/Textures/Terrain/Tile0.dds", 1))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_VIBuffer_Terrain", CVIBuffer_Terrain::Create(m_pDevice, m_pContext, L"../../Client/Bin/Resources/Textures/Terrain/Height1.bmp"))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Collider", CCollider::Create(m_pDevice, m_pContext))))
        return E_FAIL;
#pragma endregion

#pragma region Effect_Default
    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Shader_VtxPosTex", CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxPosTex.hlsl", VXTPOSTEX::Elements, VXTPOSTEX::iNumElements))))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_VIBuffer_Rect", CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_VIBuffer_RectZ", CVIBuffer_RectZ::Create(m_pDevice, m_pContext))))
        return E_FAIL;
    /*if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Texture", CTexture::Create(m_pDevice, m_pContext, L"../../Client/Bin/Resources/Textures/Terrain/Tile0.dds", 1))))
        return E_FAIL;*/
#pragma endregion

#pragma region Effect_Mesh
    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Shader_VtxMesh", CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxMesh.hlsl", VTXMESH::Elements, VTXMESH::iNumElements))))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_VIBuffer_Trail", CVIBuffer_Trail::Create(m_pDevice, m_pContext))))
        return E_FAIL;
#pragma endregion

    if (FAILED(m_pGameInstance->Add_Component_Prototype(LEVEL_MAIN, L"Prototype_Component_Shader_VtxMesh_Trail", CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_VtxMesh_Trail.hlsl", VTXMESH::Elements, VTXMESH::iNumElements))))
        return E_FAIL;

    return S_OK;
}

HRESULT CMainEffect::Loading_GameObjects()
{
    if (FAILED(m_pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Camera"), CEffectTool_Camera::Create(m_pDevice, m_pContext))))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Effect_Point"), CTool_Effect_Point::Create(m_pDevice, m_pContext))))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Effect_Terrain"), CEffectTool_Terrain::Create(m_pDevice, m_pContext))))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Effect_Default"), CEffect_Default::Create(m_pDevice, m_pContext))))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Effect_Mesh"), CEffect_Mesh::Create(m_pDevice, m_pContext))))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Add_GameObject_Prototype(TEXT("Prototype_GameObject_Effect_Trail"), CEffect_Trail::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    return S_OK;
}

void CMainEffect::Render()
{
    m_pGameInstance->Clear_BackBuffer_View(_float4{ 0.f, 0.f, 0.f, 1.f });
    m_pGameInstance->Clear_DepthStencil_View();

    m_pGameInstance->Draw_From_Tool();

    m_pEffectTool_Manager->Render();

    m_pGameInstance->Present();
}

CMainEffect* CMainEffect::Create()
{
    CMainEffect* pInstance = new CMainEffect();

    if (FAILED(pInstance->Initialize()))
        Safe_Release(pInstance);

	return pInstance;
}

void CMainEffect::Free()
{
    m_pEffectTool_Manager->Release_Tool();
    Safe_Release(m_pEffectTool_Manager);

    m_pEffectManager->Release_EffectManager();
    Safe_Release(m_pEffectManager);

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);

    m_pGameInstance->Release_Engine();
    Safe_Release(m_pGameInstance);
}
