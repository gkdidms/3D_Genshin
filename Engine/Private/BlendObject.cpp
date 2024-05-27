#include "BlendObject.h"

#include "GameInstance.h"

CBlendObject::CBlendObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CBlendObject::CBlendObject(const CBlendObject& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CBlendObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBlendObject::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CBlendObject::Priority_Tick(const _float& fTimeDelta)
{
}

void CBlendObject::Tick(const _float& fTimeDelta)
{
}

void CBlendObject::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CBlendObject::Render()
{
	return S_OK;
}

_float CBlendObject::Compute_ViewZ(_fvector vWorldPos)
{
	return XMVectorGetX(XMVector3Length(m_pGameInstance->Get_CamPosition() - vWorldPos));
}

void CBlendObject::Free()
{
	__super::Free();
}
