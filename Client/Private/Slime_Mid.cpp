#include "Slime_Mid.h"

#include "GameInstance.h"

CSlime_Mid::CSlime_Mid(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSlime{ pDevice, pContext}
{
}

CSlime_Mid::CSlime_Mid(const CSlime_Mid& rhs)
	: CSlime{ rhs }
{
}

HRESULT CSlime_Mid::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CSlime_Mid::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(1.5f, 1.5f, 1.5f);

	return S_OK;
}

void CSlime_Mid::Priority_Tick(const _float& fTimeDelta)
{
}

void CSlime_Mid::Tick(const _float& fTimeDelta)
{
}

void CSlime_Mid::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CSlime_Mid::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSlime_Mid::Bind_ResourceData()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CSlime_Mid::Change_Animation(const _float& fTimeDelta)
{
}

void CSlime_Mid::Free()
{
	__super::Free();
}
