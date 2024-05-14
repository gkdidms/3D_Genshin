#include "PartObject.h"

#include "GameInstance.h"
#include "StateManager.h"

CPartObject::CPartObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext },
	m_pStateManager { CStateManager::GetInstance() }
{
	Safe_AddRef(m_pStateManager);
}

CPartObject::CPartObject(const CPartObject& rhs)
	: CGameObject { rhs },
	m_pStateManager { rhs.m_pStateManager }
{
	Safe_AddRef(m_pStateManager);
}

HRESULT CPartObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPartObject::Initialize(void* pArg)
{
	PART_DESC* pDesc = (PART_DESC*)pArg;

	if (nullptr != pDesc)
	{
		m_pParentMatrix = pDesc->pParentMatrix;
		m_pState = pDesc->pState;
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CPartObject::Priority_Tick(const _float& fTimeDelta)
{
}

void CPartObject::Tick(const _float& fTimeDelta)
{
}

void CPartObject::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CPartObject::Render()
{
	return S_OK;
}

void CPartObject::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pStateManager);
}
