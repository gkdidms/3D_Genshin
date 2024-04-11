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
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

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
}
