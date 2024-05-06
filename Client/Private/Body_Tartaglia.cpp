#include "Body_Tartaglia.h"

#include "Boss.h"

CBody_Tartaglia::CBody_Tartaglia(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CPartObject_Body{ pDevice, pContext }
{
}

CBody_Tartaglia::CBody_Tartaglia(const CBody_Tartaglia& rhs)
    : CPartObject_Body { rhs }
{
}

HRESULT CBody_Tartaglia::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBody_Tartaglia::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

   

    return S_OK;
}

void CBody_Tartaglia::Priority_Tick(const _float& fTimeDelta)
{
}

void CBody_Tartaglia::Tick(const _float& fTimeDelta)
{
    _float4x4 MoveMatrix;
    m_pModelCom->Play_Animation(fTimeDelta, &MoveMatrix);
}

void CBody_Tartaglia::Late_Tick(const _float& fTimeDelta)
{
    XMStoreFloat4x4(&m_pWorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));

    m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CBody_Tartaglia::Render()
{
    if (FAILED(__super::Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CBody_Tartaglia::Add_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Boss_Tartaglia", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CBody_Tartaglia::Bind_ResourceData()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pWorldMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

void CBody_Tartaglia::Change_Animation()
{
    _uint iAnim = { 0 };
    _bool isLoop = { false };

    switch (*m_pState)
    {
    case CBoss::BOSS_BOW_IDLE:
    {
        iAnim = 26;
        isLoop = true;
        break;
    }
    case CBoss::BOSS_BLADE_IDEL:
    {
        iAnim = 11;
        isLoop = true;
        break;
    }
    case CBoss::BOSS_BOW_COMBO_ATTACK:
    {
        iAnim = 12;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BOW_COVER_ATTACK_BS:
    {
        iAnim = 14;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BOW_COVER_ATTACK_AS:
    {
        iAnim = 13;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BOW_COVER_ATTACK_LOOP:
    {
        iAnim = 15;
        isLoop = true;
        break;
    }
    case CBoss::BOSS_BOW_NORMAL_ATTACK_BS:
    {
        iAnim = 17;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BOW_NORMAL_ATTACK_AS:
    {
        iAnim = 16;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BOW_NORMAL_ATTACK_LOOP:
    {
        iAnim = 18;
        isLoop = true;
        break;
    }
    case CBoss::BOSS_BOW_POWER_ATTACK_BS:
    {
        iAnim = 20;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BOW_POWER_ATTACK_AS:
    {
        iAnim = 19;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BOW_POWER_ATTACK_LOOP:
    {
        iAnim = 21;
        isLoop = true;
        break;
    }
    case CBoss::BOSS_BOW_RANGE_ATTACK_BS:
    {
        iAnim = 23;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BOW_RANGE_ATTACK_AS:
    {
        iAnim = 22;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BOW_RANGE_ATTACK_LOOP_1:
    {
        iAnim = 24;
        isLoop = true;
        break;
    }
    case CBoss::BOSS_BOW_RANGE_ATTACK_LOOP_2:
    {
        iAnim = 25;
        isLoop = true;
        break;
    }
    case CBoss::BOSS_DEFEND_1:
    {
        iAnim = 0;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_DEFEND_2:
    {
        iAnim = 1;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_DEFEND_3:
    {
        iAnim = 2;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_DEFEND_ATTACK_1:
    {
        iAnim = 3;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_DEFEND_ATTACK_2:
    {
        iAnim = 4;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BLADE_EXTRA_ATTACK:
    {
        iAnim = 5;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BLADE_NORMAL_ATTACK_1:
    {
        iAnim = 6;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BLADE_NORMAL_ATTACK_2:
    {
        iAnim = 7;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BLADE_NORMAL_ATTACK_3:
    {
        iAnim = 8;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_BLADE_NORMAL_ATTACK_4:
    {
        iAnim = 9;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_HIT_H:
    {
        iAnim = 27;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_HIT_L:
    {
        iAnim = 29;
        isLoop = false;
        break;
    }
    case CBoss::BOSS_DIE:
    {
        break;
    }
    case CBoss::BOSS_END:
    {
        break;
    }
    default:
        break;
    }
}

CBody_Tartaglia* CBody_Tartaglia::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBody_Tartaglia* pInstance = new CBody_Tartaglia(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
        Safe_Release(pInstance);

    return pInstance;
}

CGameObject* CBody_Tartaglia::Clone(void* pArg)
{
    CBody_Tartaglia* pInstance = new CBody_Tartaglia(*this);

    if (FAILED(pInstance->Initialize(pArg)))
        Safe_Release(pInstance);

    return pInstance;
}

void CBody_Tartaglia::Free()
{
    __super::Free();
}
