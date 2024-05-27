#include "Boss_Body.h"

CBoss_Body::CBoss_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{ pDevice, pContext }
{
}

CBoss_Body::CBoss_Body(const CBoss_Body& rhs)
	: CPartObject{ rhs }
{
}

HRESULT CBoss_Body::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBoss_Body::Initialize(void* pArg)
{
	if (nullptr == pArg) return E_FAIL;

	TARTAGLIA_DESC* pDesc = static_cast<TARTAGLIA_DESC*>(pArg);
	m_pTargetMatrix = pDesc->pTargetMatrix;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CBoss_Body::Priority_Tick(const _float& fTimeDelta)
{
}

void CBoss_Body::Tick(const _float& fTimeDelta)
{
}

void CBoss_Body::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CBoss_Body::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (int i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			continue;
		m_pShaderCom->Begin(0);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CBoss_Body::Add_Components()
{
	return S_OK;
}

HRESULT CBoss_Body::Bind_ResourceData()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fFar", m_pGameInstance->Get_CamFar(), sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

void CBoss_Body::Change_Animation()
{
}

void CBoss_Body::Free()
{
	__super::Free();
}
