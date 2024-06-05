#include "Tartaglia_Blade.h"

#include "Boss.h"

CTartaglia_Blade::CTartaglia_Blade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CWeapon { pDevice, pContext }
{
}

CTartaglia_Blade::CTartaglia_Blade(const CTartaglia_Blade& rhs)
    : CWeapon { rhs }
{
}

HRESULT CTartaglia_Blade::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTartaglia_Blade::Initialize(void* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;

    TARTAGLIA_BLADE_DESC* pDesc = static_cast<TARTAGLIA_BLADE_DESC*>(pArg);
    m_ElementalType = pDesc->eElementalType;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    return S_OK;
}

void CTartaglia_Blade::Priority_Tick(const _float& fTimeDelta)
{
}

void CTartaglia_Blade::Tick(const _float& fTimeDelta)
{
}

void CTartaglia_Blade::Late_Tick(const _float& fTimeDelta)
{
    _matrix		SocketMatrix = XMMatrixIdentity();

    m_isHide = !(*m_pState == CBoss::BOSS_BLADE_EXTRA_ATTACK || *m_pState == CBoss::BOSS_BLADE_RANGE_ATTACK 
        || *m_pState == CBoss::BOSS_BLADE_NORMAL_ATTACK_1 || *m_pState == CBoss::BOSS_BLADE_NORMAL_ATTACK_2 || *m_pState == CBoss::BOSS_BLADE_NORMAL_ATTACK_3
        || *m_pState == CBoss::BOSS_BLADE_NORMAL_ATTACK_4);
    if (!m_isHide)
    {
        SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

        SocketMatrix.r[0] = XMVector3Normalize(SocketMatrix.r[0]);
        SocketMatrix.r[1] = XMVector3Normalize(SocketMatrix.r[1]);
        SocketMatrix.r[2] = XMVector3Normalize(SocketMatrix.r[2]);

        XMStoreFloat4x4(&m_pWorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));

        m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
    }
}

HRESULT CTartaglia_Blade::Render()
{
    if (m_isHide)
        return S_OK;

    if (FAILED(__super::Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CTartaglia_Blade::Add_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_STAGE_BOSS, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    if (m_ElementalType == ELECTRIC)
    {
        if (FAILED(__super::Add_Component(LEVEL_STAGE_BOSS, L"Prototype_Component_Model_Tartaglia_Blade_Electric", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
            return E_FAIL;
    }
    else if (m_ElementalType == WATER)
    {
        if (FAILED(__super::Add_Component(LEVEL_STAGE_BOSS, L"Prototype_Component_Model_Tartaglia_Blade_Water", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
            return E_FAIL;
    }

    return S_OK;
}

HRESULT CTartaglia_Blade::Bind_ResourceData()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pWorldMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fFar", m_pGameInstance->Get_CamFar(), sizeof(_float))))
        return E_FAIL;

    return S_OK;
}

void CTartaglia_Blade::Change_Animation(const _float& fTimeDelta)
{
}

CTartaglia_Blade* CTartaglia_Blade::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTartaglia_Blade* pInstance = new CTartaglia_Blade(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
        Safe_Release(pInstance);

    return pInstance;
}

CGameObject* CTartaglia_Blade::Clone(void* pArg)
{
    CTartaglia_Blade* pInstane = new CTartaglia_Blade(*this);

    if (FAILED(pInstane->Initialize(pArg)))
        Safe_Release(pInstane);

    return pInstane;
}

void CTartaglia_Blade::Free()
{
    __super::Free();
}
