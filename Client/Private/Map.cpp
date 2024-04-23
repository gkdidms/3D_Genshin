#include "Map.h"

#include "GameInstance.h"

CMap::CMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CMap::CMap(const CMap& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CMap::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMap::Initialize(void* pArg)
{
	_float4x4 WorldMatrix = {};

	if (nullptr != pArg)
	{
		MAP_DESC* pDesc = static_cast<MAP_DESC*>(pArg);
		WorldMatrix = pDesc->WorldMatrix;
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&WorldMatrix));

	return S_OK;
}

void CMap::Priority_Tick(const _float& fTimeDelta)
{
}

void CMap::Tick(const _float& fTimeDelta)
{
}

void CMap::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CMap::Render()
{
	return S_OK;
}

void CMap::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pNavigationCom);
}
