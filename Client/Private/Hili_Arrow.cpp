#include "Hili_Arrow.h"

#include "GameInstance.h"

CHili_Arrow::CHili_Arrow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CMonster_Bullet{ pDevice, pContext }
{
}

CHili_Arrow::CHili_Arrow(const CHili_Arrow& rhs)
    : CMonster_Bullet{ rhs }
{
}

HRESULT CHili_Arrow::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CHili_Arrow::Initialize(void* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;

    BULLET_DESC* pDesc = static_cast<BULLET_DESC*>(pArg);
    m_fSpeed = pDesc->fSpeedPecSec;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    _matrix WorldMatrix;
    _matrix HandMatrix = XMMatrixIdentity();
    HandMatrix.r[3] = XMLoadFloat4x4(&pDesc->HandCombinedTransformationMatrix).r[3];
    m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45.f));
    WorldMatrix = m_pTransformCom->Get_WorldMatrix() * HandMatrix * XMLoadFloat4x4(&pDesc->ParentMatrix);

    m_pTransformCom->Set_WorldMatrix(WorldMatrix);
    m_vTargetPos.y = m_vTargetPos.y + 1.f;

    m_fHeight = pDesc->ParentMatrix.m[3][1]; // y 값 저장

    XMStoreFloat4(&m_vTargetLook, XMVector3Normalize(XMLoadFloat4(&m_vTargetPos) - m_pTransformCom->Get_State(CTransform::STATE_POSITION)));

    return S_OK;
}

void CHili_Arrow::Priority_Tick(const _float& fTimeDelta)
{
}

void CHili_Arrow::Tick(const _float& fTimeDelta)
{
    //이동
    m_isDead = Move_Arrow(fTimeDelta);

    //버퍼 업데이트
    _float4x4 TrailMatrix = *m_pTransformCom->Get_WorldFloat4x4();
   // m_pTransformCom->Get_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f), &TrailMatrix);
    m_pTrailVIBufferCom->Add_Trail(fTimeDelta, XMLoadFloat4x4(&TrailMatrix));

    m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void CHili_Arrow::Late_Tick(const _float& fTimeDelta)
{
    m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONLIGHT, this);
}

HRESULT CHili_Arrow::Render()
{
    if (FAILED(Bind_ResourceData()))
        return E_FAIL;

    _uint iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (int i = 0; i < iNumMeshes; ++i)
    {
        m_pShaderCom->Begin(1);
        m_pModelCom->Render(i);
    }

    //트레일 버퍼
    if (FAILED(Bind_TrailResourceData()))
        return E_FAIL;

    m_pTrailShaderCom->Begin(5);
    m_pTrailVIBufferCom->Render();

#ifdef _DEBUG
    m_pColliderCom->Render();
#endif // _DEBUG
}

HRESULT CHili_Arrow::Add_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxNorTex_Skill", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxPosTex_Skill", L"Com_TrailShader", reinterpret_cast<CComponent**>(&m_pTrailShaderCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Monster_Arrow", L"Com_Texture", reinterpret_cast<CComponent**>(&m_pTextureCom[0]))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Skill_Hili_Arrow", L"Com_TrailTexture", reinterpret_cast<CComponent**>(&m_pTextureCom[1]))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Monster_Arrow", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;



    CBounding_AABB::BOUNDING_AABB_DESC BoundingBoxDesc{};
    BoundingBoxDesc.eType = CCollider::COLLIDER_AABB;
    BoundingBoxDesc.vExtents = _float3(0.1f, 0.1f, 0.1f);
    BoundingBoxDesc.vCenter = _float3(0.f, BoundingBoxDesc.vExtents.y, 0.f);

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider", L"Com_Collider", reinterpret_cast<CComponent**>(&m_pColliderCom), &BoundingBoxDesc)))
        return E_FAIL;

    CVIBuffer_Trail::VIBUFFER_TRAIL_DESC TrailDesc{};
    TrailDesc.iMaxTrail = 40;
    TrailDesc.vInitPosA = _float3(-0.3f, 0.f, 0.3f);
    TrailDesc.vInitPosB = _float3(-0.3f, 0.f, -0.3f);
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_VIBuffer_Trail", L"Com_TrailBuffer", reinterpret_cast<CComponent**>(&m_pTrailVIBufferCom), &TrailDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CHili_Arrow::Bind_ResourceData()
{
    if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    if (FAILED(m_pTextureCom[0]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
        return E_FAIL;



    return S_OK;
}

HRESULT CHili_Arrow::Bind_TrailResourceData()
{
    if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pTrailShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pTrailShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;

    if (FAILED(m_pTrailShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    if (FAILED(m_pTextureCom[1]->Bind_ShaderResource(m_pTrailShaderCom, "g_TrailDiffuseTexture", 0)))
        return E_FAIL;

    return S_OK;
}

_bool CHili_Arrow::Move_Arrow(const _float& fTimeDelta)
{
    m_fAccelTime += fTimeDelta * 0.01f;

    _float fTemp = (m_fPower * m_fAccelTime) - (9.8f * m_fAccelTime * m_fAccelTime * 0.5f);

    _vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

    _float fY = XMVectorGetY(vPosition);
    vPosition += XMLoadFloat4(&m_vTargetLook) * m_fSpeed * fTimeDelta;
    vPosition = XMVectorSetY(vPosition, fY - fTemp);

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

    if (m_fHeight >= XMVectorGetY(vPosition))
    {
        // 충돌했을때 이펙트 출력
        return true;
    }


    return false;
}

CHili_Arrow* CHili_Arrow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CHili_Arrow* pInstance = new CHili_Arrow(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
        Safe_Release(pInstance);

    return pInstance;
}

CGameObject* CHili_Arrow::Clone(void* pArg)
{
    CHili_Arrow* pInstance = new CHili_Arrow(*this);

    if (FAILED(pInstance->Initialize(pArg)))
        Safe_Release(pInstance);

    return pInstance;
}

void CHili_Arrow::Free()
{
    __super::Free();

    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTrailShaderCom);
    for (auto& pTexture : m_pTextureCom)
        Safe_Release(pTexture);
    Safe_Release(m_pTrailVIBufferCom);
}
