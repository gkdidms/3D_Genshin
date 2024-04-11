#include "Monster.h"

#include "GameInstance.h"

CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CGameObject{ pDevice, pContext }
{
}

CMonster::CMonster(const CMonster& rhs)
    : CGameObject{ rhs }
{
}

HRESULT CMonster::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    //m_pVIBufferModelCom->Set_Animation(CModel::ANIM_DESC{(_uint)rand() % 20, true});
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(rand() % 20, 3.f, rand() % 20, 1.f));
    return S_OK;
}

void CMonster::Priority_Tick(const _float& fTimeDelta)
{
}

void CMonster::Tick(const _float& fTimeDelta)
{
    m_pVIBufferModelCom->Play_Animation(fTimeDelta);
}

void CMonster::Late_Tick(const _float& fTimeDelta)
{
    m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CMonster::Render()
{
    if (FAILED(Bind_Render()))
        return E_FAIL;

    _uint iNumMeshes = m_pVIBufferModelCom->Get_NumMeshes();

    for (int i = 0; i < iNumMeshes; ++i)
    {
        if (FAILED(m_pVIBufferModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
            return E_FAIL;

        if (FAILED(m_pVIBufferModelCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
            return E_FAIL;

        m_pShaderCom->Begin(0);
        m_pVIBufferModelCom->Render(i);
    }

    return S_OK;
}

HRESULT CMonster::Add_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimMesh", L"Component_Shader_VtxAnimMesh", reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Fiona", L"Component_Model_Fiona", reinterpret_cast<CComponent**>(&m_pVIBufferModelCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CMonster::Bind_Render()
{
    if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

CMonster* CMonster::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMonster* pInstance = new CMonster(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
        Safe_Release(pInstance);

    return pInstance;
}

CGameObject* CMonster::Clone(void* pArg)
{
    CMonster* pInstance = new CMonster(*this);

    if (FAILED(pInstance->Initialize(pArg)))
        Safe_Release(pInstance);

    return pInstance;
}

void CMonster::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferModelCom);
}
