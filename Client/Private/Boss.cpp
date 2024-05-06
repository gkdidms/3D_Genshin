#include "Boss.h"

#include "GameInstance.h"
#include "PartObject.h"

CBoss::CBoss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CGameObject{ pDevice, pContext }
{
}

CBoss::CBoss(const CBoss& rhs)
    : CGameObject{ rhs }
{
}

HRESULT CBoss::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBoss::Initialize(void* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    BOSS_DESC* pDesc = static_cast<BOSS_DESC*>(pArg);
    m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&pDesc->WorldMatrix));
    m_iBossNavigationIndex = pDesc->iBossNavigationIndex;

    if (FAILED(Add_Components()))
        return E_FAIL;

    if (FAILED(Ready_Body()))
        return E_FAIL;

    if (FAILED(Ready_Weapon()))
        return E_FAIL;

    return S_OK;
}

void CBoss::Priority_Tick(const _float& fTimeDelta)
{
}

void CBoss::Tick(const _float& fTimeDelta)
{
    for (auto PartObject : m_PartObject[m_CurrentPage])
    {
        PartObject->Tick(fTimeDelta);
    }

    _float4x4 RootMatrix;
    dynamic_cast<CPartObject*>(m_PartObject[m_CurrentPage][PART_BODY])->Set_PlayerPos(&RootMatrix);
    XMStoreFloat4x4(&RootMatrix, XMLoadFloat4x4(&RootMatrix) * -1.f);
    _bool isMove = m_pTransformCom->Go_Run(XMLoadFloat4x4(&RootMatrix), m_pNavigationCom);

}

void CBoss::Late_Tick(const _float& fTimeDelta)
{
    for (auto PartObject : m_PartObject[m_CurrentPage])
    {
        PartObject->Late_Tick(fTimeDelta);
    }
}

HRESULT CBoss::Render()
{
    for (auto PartObject : m_PartObject[m_CurrentPage])
    {
        if (FAILED(PartObject->Render()))
            return E_FAIL;
    }

    return S_OK;
}

HRESULT CBoss::Add_Components()
{
    CNavigation::NAVIGATION_DESC NavigationDesc = {};
    NavigationDesc.iIndex = m_iBossNavigationIndex;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Navigation", L"Com_Navigation", reinterpret_cast<CComponent**>(&m_pNavigationCom), &NavigationDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CBoss::Ready_Body()
{
    CPartObject::PART_DESC Desc{};

    Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
    Desc.pState = &m_CurrentState;
    Desc.fSpeedPecSec = 20.f;
    Desc.fRotatePecSec = XMConvertToRadians(45.f);

    CGameObject* pGameObject = m_pGameInstance->Clone_Object(L"Prototype_GameObject_Boss_Tartaglia", &Desc);
    if (nullptr == pGameObject)
        return E_FAIL;
    m_PartObject[ONE_PAGE].emplace_back(pGameObject);

    return S_OK;
}

HRESULT CBoss::Ready_Weapon()
{
    return S_OK;
}

CBoss* CBoss::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBoss* pInstance = new CBoss(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
        Safe_Release(pInstance);

    return pInstance;
}

CGameObject* CBoss::Clone(void* pArg)
{
    CBoss* pInstance = new  CBoss(*this);

    if (FAILED(pInstance->Initialize(pArg)))
        Safe_Release(pInstance);

    return pInstance;
}

void CBoss::Free()
{
    __super::Free();
}
