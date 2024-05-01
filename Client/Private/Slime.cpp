#include "Slime.h"

#include "GameInstance.h"

CSlime::CSlime(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster{ pDevice, pContext }
{
}

CSlime::CSlime(const CSlime& rhs)
	: CMonster{ rhs }
{
}

HRESULT CSlime::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CSlime::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CSlime::Priority_Tick(const _float& fTimeDelta)
{
}

void CSlime::Tick(const _float& fTimeDelta)
{
}

void CSlime::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CSlime::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CSlime::Free()
{
	__super::Free();
}
