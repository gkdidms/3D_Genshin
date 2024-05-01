#include "Slime_Large.h"

#include "GameInstance.h"

CSlime_Large::CSlime_Large(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSlime{ pDevice, pContext}
{
}

CSlime_Large::CSlime_Large(const CSlime_Large& rhs)
	: CSlime{ rhs }
{
}

HRESULT CSlime_Large::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CSlime_Large::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	

	return S_OK;
}

void CSlime_Large::Priority_Tick(const _float& fTimeDelta)
{
}

void CSlime_Large::Tick(const _float& fTimeDelta)
{
}

void CSlime_Large::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CSlime_Large::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSlime_Large::Bind_ResourceData()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CSlime_Large::Change_Animation(const _float& fTimeDelta)
{
}

void CSlime_Large::Free()
{
	__super::Free();
}
