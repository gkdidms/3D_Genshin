#include "GoldenHouse.h"

#include "GameInstance.h"

CGoldenHouse::CGoldenHouse(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CMap{ pDevice, pContext }
{
}

CGoldenHouse::CGoldenHouse(const CGoldenHouse& rhs)
    : CMap{ rhs }
{
}

HRESULT CGoldenHouse::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CGoldenHouse::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    return S_OK;
}

void CGoldenHouse::Priority_Tick(const _float& fTimeDelta)
{
}

void CGoldenHouse::Tick(const _float& fTimeDelta)
{
}

void CGoldenHouse::Late_Tick(const _float& fTimeDelta)
{
    m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CGoldenHouse::Render()
{
    if (FAILED(Bind_ResourceData()))
        return E_FAIL;

    _uint iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (int i = 0; i < iNumMeshes; ++i)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
            continue;

        m_pModelCom->Bind_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS);

        m_pShaderCom->Begin(1);
        m_pModelCom->Render(i);
    }

#ifdef _DEBUG
    m_pNavigationCom->Render();
#endif // _DEBUG

    return S_OK;
}

HRESULT CGoldenHouse::Add_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_STAGE_BOSS, L"Prototype_Component_Shader_VtxMesh", L"Component_Shader_VtxMesh", reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STAGE_BOSS, L"Prototype_Component_Model_GoldenHouse", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STAGE_BOSS, L"Prototype_Component_Navigation", L"Com_Navigation", reinterpret_cast<CComponent**>(&m_pNavigationCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CGoldenHouse::Bind_ResourceData()
{
    if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fFar", m_pGameInstance->Get_CamFar(), sizeof(_float))))
        return E_FAIL;

    return S_OK;
}

CGoldenHouse* CGoldenHouse::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CGoldenHouse* pInstance = new CGoldenHouse(pDevice, pContext);
    
    if (FAILED(pInstance->Initialize_Prototype()))
        Safe_Release(pInstance);

    return pInstance;
}

CGameObject* CGoldenHouse::Clone(void* pArg)
{
    CGoldenHouse* pInstance = new CGoldenHouse(*this);

    if (FAILED(pInstance->Initialize(pArg)))
        Safe_Release(pInstance);

    return pInstance;
}

void CGoldenHouse::Free()
{
    __super::Free();
}
