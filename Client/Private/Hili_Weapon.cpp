#include "Hili_Weapon.h"

#include "GameInstance.h"

CHili_Weapon::CHili_Weapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CGameObject{pDevice, pContext}
{
}

CHili_Weapon::CHili_Weapon(const CHili_Weapon& rhs)
    : CGameObject{rhs}
{
}

HRESULT CHili_Weapon::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CHili_Weapon::Initialize(void* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;
    
    HILI_WEAPON_DESC* pDesc = static_cast<HILI_WEAPON_DESC*>(pArg);
    m_TargetMatrix = pDesc->pTargetCombinedTransformationMatrix;
    m_SocketMatrix = pDesc->pHandCombinedTransformationMatrix;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CHili_Weapon::Priority_Tick(const _float& fTimeDelta)
{
}

void CHili_Weapon::Tick(const _float& fTimeDelta)
{
}

void CHili_Weapon::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CHili_Weapon::Render()
{
    if (FAILED(Bind_ResourceData()))
        return E_FAIL;

    _uint iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (int i = 0; i < iNumMeshes; ++i)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
            continue;

        m_pShaderCom->Begin(0);
        m_pModelCom->Render(i);
    }

    return S_OK;
}

HRESULT CHili_Weapon::Bind_ResourceData()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

HRESULT CHili_Weapon::Add_Components()
{
    return S_OK;
}

void CHili_Weapon::Free()
{
    __super::Free();

    Safe_Release(m_pModelCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pColliderCom);
}
