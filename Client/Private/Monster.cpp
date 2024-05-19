#include "Monster.h"

#include "GameInstance.h"

#include "Player.h"

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
    if (nullptr == pArg)
        return E_FAIL;

    MONSTER_DESC* pDesc = static_cast<MONSTER_DESC*>(pArg);

    m_pTargetMatrix = pDesc->TargetMatrix;
    m_iMonsterNavigationIndex = pDesc->iMonsterNavigationIndex;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&pDesc->WorldMatrix));

    return S_OK;
}

void CMonster::Priority_Tick(const _float& fTimeDelta)
{
    m_isColl = false; // 초기화

    CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"), 0));
    _bool isCheckColl = m_pColliderCom->Intersect(dynamic_cast<CCollider*>(pPlayer->Get_Component(TEXT("Com_Collider"))));

    if (isCheckColl)
    {
        m_isColl = true;
        pPlayer->Set_Coll(true);
        _char strPlayerRank = pPlayer->Get_StateRank();
        m_pColliderCom->Compute_Rank(m_strStateRank, strPlayerRank, m_pTransformCom, dynamic_cast<CTransform*>(pPlayer->Get_Component(TEXT("Com_Transform"))), m_pNavigation, dynamic_cast<CNavigation*>(pPlayer->Get_Component(TEXT("Com_Navigation"))), fTimeDelta);
    }

    vector<CGameObject*> vecMonsters = m_pGameInstance->Get_GameObjects(LEVEL_GAMEPLAY, TEXT("Layer_Monster"));

    //자기 자신을 비교하지 않도록 처리하기
    for (auto& pMonster : vecMonsters)
    {
        if (pMonster->Get_Index() == m_iIndex)
            continue;

        isCheckColl = m_pColliderCom->Intersect(dynamic_cast<CCollider*>(pMonster->Get_Component(TEXT("Com_Collider"))));

        if (isCheckColl)
        {
            m_isColl = true;
            CMonster* pCollMonster = dynamic_cast<CMonster*>(pMonster);
            _char strCollMonsterRank = pMonster->Get_StateRank();

            m_pColliderCom->Compute_Rank(m_strStateRank, strCollMonsterRank, m_pTransformCom, dynamic_cast<CTransform*>(pCollMonster->Get_Component(TEXT("Com_Transform"))), m_pNavigation, dynamic_cast<CNavigation*>(pCollMonster->Get_Component(TEXT("Com_Navigation"))), fTimeDelta);
        }
    }
}

void CMonster::Tick(const _float& fTimeDelta)
{

}

void CMonster::Late_Tick(const _float& fTimeDelta)
{

}

HRESULT CMonster::Render()
{
    if (FAILED(Bind_ResourceData()))
        return E_FAIL;

    _uint iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (int i = 0; i < iNumMeshes; ++i)
    {
        if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
            continue;

        m_pShaderCom->Begin(0);
        m_pModelCom->Render(i);
    }

    return S_OK;
}

void CMonster::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pColliderCom);
    Safe_Release(m_pNavigation);
}
