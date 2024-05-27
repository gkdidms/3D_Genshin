#include "Item.h"

#include "GameInstance.h"

CItem::CItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CGameObject{ pDevice, pContext}
{
}

CItem::CItem(const CItem& rhs)
    : CGameObject { rhs }
{
}

HRESULT CItem::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CItem::Initialize(void* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;

    ITEM_DESC* pDesc = static_cast<ITEM_DESC*>(pArg);
    m_fHeight = pDesc->fHeight;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    //생성되는 위치로 네비게이션 인덱스 찾기
    m_RandomNumber = mt19937_64(m_RandomDevice());
    uniform_real_distribution<float>	Angle(0.f, 360.f);

    _vector vPos = XMLoadFloat4x4(&pDesc->CreateMatrix).r[3];
    _float fAngle = Angle(m_RandomNumber);

    m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&pDesc->CreateMatrix));
    m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(fAngle));

    return S_OK;
}

void CItem::Priority_Tick(const _float& fTimeDelta)
{
}

void CItem::Tick(const _float& fTimeDelta)
{
    if (m_isDrop)
    {
        m_isDrop = m_pTransformCom->Jump(fTimeDelta, m_fHeight, 0.2f);
    }

    m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void CItem::Late_Tick(const _float& fTimeDelta)
{
    m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CItem::Render()
{
    if (FAILED(Bind_ResourceData()))
        return E_FAIL;

    _uint iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (int i = 0; i < iNumMeshes; ++i)
    {
        /*if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
            continue;*/
        m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE);

        m_pShaderCom->Begin(1);
        m_pModelCom->Render(i);
    }

#ifdef _DEBUG
    m_pColliderCom->Render();
#endif // _DEBUG

    return S_OK;
}

HRESULT CItem::Add_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Item", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    CBounding_AABB::BOUNDING_AABB_DESC Desc{};

    Desc.eType = CCollider::COLLIDER_AABB;
    Desc.vExtents = _float3(0.5f, 0.5f, 0.5f);
    Desc.vCenter = _float3(0.f, Desc.vExtents.y, 0.f);

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider", L"Com_Collider", reinterpret_cast<CComponent**>(&m_pColliderCom), &Desc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CItem::Bind_ResourceData()
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

CItem* CItem::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CItem* pInstance = new CItem(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
        Safe_Release(pInstance);

    return pInstance;
}

CGameObject* CItem::Clone(void* pArg)
{
    CItem* pInstance = new CItem(*this);

    if (FAILED(pInstance->Initialize(pArg)))
        Safe_Release(pInstance);

    return pInstance;
}

void CItem::Free()
{
    __super::Free();

    Safe_Release(m_pColliderCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
}
