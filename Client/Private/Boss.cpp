#include "Boss.h"

#include "GameInstance.h"

#include "PartObject.h"

#include "BT_Tartaglia.h"
#include "BT_EvilEye.h"
#include "BT_Harbinger.h"
#include "BT_Boss.h"

#include "Boss_Body.h"
#include "Body_EvilEye.h"
#include "Body_Harbinger.h"

#include "Harbinger_Blade.h"
#include "Tartaglia_Blade.h"


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
    m_pTargetMatrix = pDesc->TargetMatrix;

    if (FAILED(Add_Components()))
        return E_FAIL;

    if (FAILED(Ready_Body()))
        return E_FAIL;

    if (FAILED(Ready_Weapon()))
        return E_FAIL;

    if (FAILED(Ready_Object()))
        return E_FAIL;

    return S_OK;
}

void CBoss::Priority_Tick(const _float& fTimeDelta)
{
}

void CBoss::Tick(const _float& fTimeDelta)
{
    if (m_pGameInstance->GetKeyState(DIK_0) == CInput_Device::TAP)
    {
        m_CurrentPage = ONE_PAGE;
        m_CurrentState = PLAYER_IDLE;
    }
    else if (m_pGameInstance->GetKeyState(DIK_9) == CInput_Device::TAP)
    {
        m_CurrentPage = TWO_PAGE;
        m_CurrentState = PLAYER_IDLE;
    }
    else if (m_pGameInstance->GetKeyState(DIK_8) == CInput_Device::TAP)
    {
        m_CurrentPage = FINAL_PAGE;
        m_CurrentState = PLAYER_IDLE;
    }
        

    if (m_CurrentState == BOSS_DIE && m_CurrentPage == ONE_PAGE)
        m_CurrentPage = TWO_PAGE;
    else if (m_CurrentState == BOSS_DIE && m_CurrentPage == TWO_PAGE)
        m_CurrentPage = FINAL_PAGE;

    m_pBT[m_CurrentPage]->Tick(fTimeDelta);

    for (auto PartObject : m_PartObject[m_CurrentPage])
    {
        PartObject->Tick(fTimeDelta);
    }

    CBoss_Body* pCurrentBody = dynamic_cast<CBoss_Body*>(m_PartObject[m_CurrentPage][PART_BODY]);
    _float4x4 RootMatrix;
    pCurrentBody->Set_PlayerPos(&RootMatrix);

    if (pCurrentBody->isMovePos())
    {
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4x4(&RootMatrix).r[3]);
    }
    else
    {
        XMStoreFloat4x4(&RootMatrix, XMLoadFloat4x4(&RootMatrix) * -1.f);
        m_pTransformCom->Go_Run(XMLoadFloat4x4(&RootMatrix), m_pNavigationCom);
    }

    m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void CBoss::Late_Tick(const _float& fTimeDelta)
{
    _vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    vPos = XMVectorSetY(vPos, m_pNavigationCom->Compute_Height(vPos));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

    for (auto PartObject : m_PartObject[m_CurrentPage])
    {
        PartObject->Late_Tick(fTimeDelta);
    }

    m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CBoss::Render()
{
    for (auto PartObject : m_PartObject[m_CurrentPage])
    {
        if (FAILED(PartObject->Render()))
            return E_FAIL;
    }

#ifdef _DEBUG
    m_pColliderCom->Render();
#endif // _DEBUG

    return S_OK;
}

HRESULT CBoss::Add_Components()
{
    CNavigation::NAVIGATION_DESC NavigationDesc = {};
    NavigationDesc.iIndex = m_iBossNavigationIndex;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Navigation", L"Com_Navigation", reinterpret_cast<CComponent**>(&m_pNavigationCom), &NavigationDesc)))
        return E_FAIL;

    CBounding_AABB::BOUNDING_AABB_DESC ColliderDesc = {};
    ColliderDesc.eType = CCollider::COLLIDER_AABB;
    ColliderDesc.vExtents = _float3{ 0.5f, 0.8f, 0.5f };
    ColliderDesc.vCenter = _float3{ 0.f, ColliderDesc.vExtents.y, 0.f };

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider", L"Com_Collider", reinterpret_cast<CComponent**>(&m_pColliderCom), &ColliderDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CBoss::Ready_Body()
{
    CBoss_Body::TARTAGLIA_DESC Desc{};

    //OnePage
    Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
    Desc.pState = &m_CurrentState;
    Desc.fSpeedPecSec = 20.f;
    Desc.fRotatePecSec = XMConvertToRadians(45.f);
    Desc.pTargetMatrix = m_pTargetMatrix;

    CGameObject* pGameObject = m_pGameInstance->Clone_Object(L"Prototype_GameObject_Boss_Tartaglia", &Desc);
    if (nullptr == pGameObject)
        return E_FAIL;
    m_PartObject[ONE_PAGE].emplace_back(pGameObject);

    //TwoPage
    Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
    Desc.pState = &m_CurrentState;
    Desc.fSpeedPecSec = 20.f;
    Desc.fRotatePecSec = XMConvertToRadians(45.f);
    Desc.pTargetMatrix = m_pTargetMatrix;

    pGameObject = m_pGameInstance->Clone_Object(L"Prototype_GameObject_Boss_EvilEye", &Desc);
    if (nullptr == pGameObject)
        return E_FAIL;
    m_PartObject[TWO_PAGE].emplace_back(pGameObject);

    //FinalPage
    Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
    Desc.pState = &m_CurrentState;
    Desc.fSpeedPecSec = 20.f;
    Desc.fRotatePecSec = XMConvertToRadians(45.f);
    Desc.pTargetMatrix = m_pTargetMatrix;

    pGameObject = m_pGameInstance->Clone_Object(L"Prototype_GameObject_Boss_Harbinger", &Desc);
    if (nullptr == pGameObject)
        return E_FAIL;

    m_PartObject[FINAL_PAGE].emplace_back(pGameObject);

    return S_OK;
}

HRESULT CBoss::Ready_Weapon()
{
    CComponent* pComponent = { nullptr };
    CWeapon::WEAPON_DESC Desc{};

    //One_Page
    CTartaglia_Blade::TARTAGLIA_BLADE_DESC TartaBladeDesc{};
    pComponent = m_PartObject[ONE_PAGE][PART_BODY]->Get_Component(TEXT("Com_Model"));
    TartaBladeDesc.eWeaponType = CWeapon::WEAPON_SWORD;
    TartaBladeDesc.pHandCombinedTransformationMatrix = dynamic_cast<CModel*>(pComponent)->Get_BoneCombinedTransformationMatrix("WeaponR");
    TartaBladeDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
    TartaBladeDesc.pState = &m_CurrentState;
    TartaBladeDesc.eElementalType = WATER;

    CGameObject* pWaterBladeR = m_pGameInstance->Clone_Object(L"Prototype_GameObject_Tartaglia_Blade", &TartaBladeDesc);
    if (nullptr == pWaterBladeR)
        return E_FAIL;

    m_PartObject[ONE_PAGE].emplace_back(pWaterBladeR);

    pComponent = m_PartObject[ONE_PAGE][PART_BODY]->Get_Component(TEXT("Com_Model"));
    TartaBladeDesc.eWeaponType = CWeapon::WEAPON_SWORD;
    TartaBladeDesc.pHandCombinedTransformationMatrix = dynamic_cast<CModel*>(pComponent)->Get_BoneCombinedTransformationMatrix("WeaponL");
    TartaBladeDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
    TartaBladeDesc.pState = &m_CurrentState;
    TartaBladeDesc.eElementalType = WATER;

    CGameObject* pWaterBladeL = m_pGameInstance->Clone_Object(L"Prototype_GameObject_Tartaglia_Blade", &TartaBladeDesc);
    if (nullptr == pWaterBladeL)
        return E_FAIL;

    m_PartObject[ONE_PAGE].emplace_back(pWaterBladeL);

    pComponent = m_PartObject[ONE_PAGE][PART_BODY]->Get_Component(TEXT("Com_Model"));
    Desc.eWeaponType = CWeapon::WEAPON_BOW;
    Desc.pHandCombinedTransformationMatrix = dynamic_cast<CModel*>(pComponent)->Get_BoneCombinedTransformationMatrix("WeaponL");
    Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
    Desc.pState = &m_CurrentState;

    CGameObject* pBow = m_pGameInstance->Clone_Object(L"Prototype_GameObject_Tartaglia_Bow", &Desc);
    if (nullptr == pBow)
        return E_FAIL;

    m_PartObject[ONE_PAGE].emplace_back(pBow);

    //TWO_PAGE
    pComponent = m_PartObject[TWO_PAGE][PART_BODY]->Get_Component(TEXT("Com_Model"));
    TartaBladeDesc.eWeaponType = CWeapon::WEAPON_SWORD;
    TartaBladeDesc.pHandCombinedTransformationMatrix = dynamic_cast<CModel*>(pComponent)->Get_BoneCombinedTransformationMatrix("WeaponR");
    TartaBladeDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
    TartaBladeDesc.pState = &m_CurrentState;
    TartaBladeDesc.eElementalType = ELECTRIC;

    CGameObject* pElectricBladeR = m_pGameInstance->Clone_Object(L"Prototype_GameObject_Tartaglia_Blade", &TartaBladeDesc);
    if (nullptr == pElectricBladeR)
        return E_FAIL;

    m_PartObject[TWO_PAGE].emplace_back(pElectricBladeR);

    pComponent = m_PartObject[TWO_PAGE][PART_BODY]->Get_Component(TEXT("Com_Model"));
    TartaBladeDesc.eWeaponType = CWeapon::WEAPON_SWORD;
    TartaBladeDesc.pHandCombinedTransformationMatrix = dynamic_cast<CModel*>(pComponent)->Get_BoneCombinedTransformationMatrix("WeaponL");
    TartaBladeDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
    TartaBladeDesc.pState = &m_CurrentState;
    TartaBladeDesc.eElementalType = ELECTRIC;

    CGameObject* pElectricBladeL = m_pGameInstance->Clone_Object(L"Prototype_GameObject_Tartaglia_Blade", &TartaBladeDesc);
    if (nullptr == pElectricBladeL)
        return E_FAIL;

    m_PartObject[TWO_PAGE].emplace_back(pElectricBladeL);

    pComponent = m_PartObject[TWO_PAGE][PART_BODY]->Get_Component(TEXT("Com_Model"));
    Desc.eWeaponType = CWeapon::WEAPON_SWORD;
    Desc.pHandCombinedTransformationMatrix = dynamic_cast<CModel*>(pComponent)->Get_BoneCombinedTransformationMatrix("WeaponR");
    Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
    Desc.pState = &m_CurrentState;

    CGameObject* pDualBlade = m_pGameInstance->Clone_Object(L"Prototype_GameObject_Tartaglia_DualBlade", &Desc);
    if (nullptr == pDualBlade)
        return E_FAIL;

    m_PartObject[TWO_PAGE].emplace_back(pDualBlade);

    //Final_Page
    pComponent = m_PartObject[FINAL_PAGE][PART_BODY]->Get_Component(TEXT("Com_Model"));
    Desc.eWeaponType = CWeapon::WEAPON_SWORD;
    Desc.pHandCombinedTransformationMatrix = dynamic_cast<CModel*>(pComponent)->Get_BoneCombinedTransformationMatrix("WeaponR");
    Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
    Desc.pState = &m_CurrentState;

    CGameObject* pHarbingerBladeR= m_pGameInstance->Clone_Object(L"Prototype_GameObject_Harbinger_Blade", &Desc);
    if (nullptr == pHarbingerBladeR)
        return E_FAIL;

    m_PartObject[FINAL_PAGE].emplace_back(pHarbingerBladeR);

    pComponent = m_PartObject[FINAL_PAGE][PART_BODY]->Get_Component(TEXT("Com_Model"));
    Desc.eWeaponType = CWeapon::WEAPON_SWORD;
    Desc.pHandCombinedTransformationMatrix = dynamic_cast<CModel*>(pComponent)->Get_BoneCombinedTransformationMatrix("WeaponL");
    Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
    Desc.pState = &m_CurrentState;

    CGameObject* pHarbingerBladeL = m_pGameInstance->Clone_Object(L"Prototype_GameObject_Harbinger_Blade", &Desc);
    if (nullptr == pHarbingerBladeL)
        return E_FAIL;

    m_PartObject[FINAL_PAGE].emplace_back(pHarbingerBladeL);

    pComponent = m_PartObject[FINAL_PAGE][PART_BODY]->Get_Component(TEXT("Com_Model"));
    Desc.eWeaponType = CWeapon::WEAPON_BOW;
    Desc.pHandCombinedTransformationMatrix = dynamic_cast<CModel*>(pComponent)->Get_BoneCombinedTransformationMatrix("WeaponL");
    Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
    Desc.pState = &m_CurrentState;

    CGameObject* pHarbingerBow = m_pGameInstance->Clone_Object(L"Prototype_GameObject_Harbinger_Bow", &Desc);
    if (nullptr == pHarbingerBow)
        return E_FAIL;

    m_PartObject[FINAL_PAGE].emplace_back(pHarbingerBow);

    pComponent = m_PartObject[FINAL_PAGE][PART_BODY]->Get_Component(TEXT("Com_Model"));
    Desc.eWeaponType = CWeapon::WEAPON_SWORD;
    Desc.pHandCombinedTransformationMatrix = dynamic_cast<CModel*>(pComponent)->Get_BoneCombinedTransformationMatrix("WeaponR");
    Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
    Desc.pState = &m_CurrentState;

    CGameObject* pHarbingerDualBlade = m_pGameInstance->Clone_Object(L"Prototype_GameObject_Harbinger_DualBlade", &Desc);
    if (nullptr == pHarbingerDualBlade)
        return E_FAIL;

    m_PartObject[FINAL_PAGE].emplace_back(pHarbingerDualBlade);

    return S_OK;
}

HRESULT CBoss::Ready_Object()
{
    //One_Page
    CBoss_Body* pTartaglia = dynamic_cast<CBoss_Body*>(m_PartObject[ONE_PAGE][PART_BODY]);
    CBT_Boss::BT_BOSS_DESC TartagliaDesc{};
    TartagliaDesc.pModel = pTartaglia->Get_Model();
    TartagliaDesc.pCollider = m_pColliderCom;
    TartagliaDesc.pInfo = pTartaglia->Get_Info();
    TartagliaDesc.pState = &m_CurrentState;
    TartagliaDesc.pTargetMatrix = m_pTargetMatrix;
    TartagliaDesc.pTransform = m_pTransformCom;

    m_pBT[ONE_PAGE] = CBT_Tartaglia::Create(&TartagliaDesc);

    if (nullptr == m_pBT[ONE_PAGE])
        return E_FAIL;

    //Two_Page
    CBoss_Body* pEvilEye = dynamic_cast<CBoss_Body*>(m_PartObject[TWO_PAGE][PART_BODY]);
    CBT_Boss::BT_BOSS_DESC EvilEye{};
    EvilEye.pModel = pEvilEye->Get_Model();
    EvilEye.pCollider = m_pColliderCom;
    EvilEye.pInfo = pEvilEye->Get_Info();
    EvilEye.pState = &m_CurrentState;
    EvilEye.pTargetMatrix = m_pTargetMatrix;
    EvilEye.pTransform = m_pTransformCom;

    m_pBT[TWO_PAGE] = CBT_EvilEye::Create(&EvilEye);

    if (nullptr == m_pBT[TWO_PAGE])
        return E_FAIL;

    CBoss_Body* pHarbinger = dynamic_cast<CBoss_Body*>(m_PartObject[FINAL_PAGE][PART_BODY]);
    CBT_Boss::BT_BOSS_DESC Harbinger{};
    Harbinger.pModel = pHarbinger->Get_Model();
    Harbinger.pCollider = m_pColliderCom;
    Harbinger.pInfo = pHarbinger->Get_Info();
    Harbinger.pState = &m_CurrentState;
    Harbinger.pTargetMatrix = m_pTargetMatrix;
    Harbinger.pTransform = m_pTransformCom;

    m_pBT[FINAL_PAGE] = CBT_Harbinger::Create(&Harbinger);

    if (nullptr == m_pBT[FINAL_PAGE])

        return E_FAIL;
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

    for (auto& PartObjects : m_PartObject)
    {
        for (auto* pPartObject : PartObjects)
            Safe_Release(pPartObject);
    }

    for (auto& pBT : m_pBT)
        Safe_Release(pBT);

    Safe_Release(m_pNavigationCom);
    Safe_Release(m_pColliderCom);
}
