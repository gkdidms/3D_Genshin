#include "PartObject_Body.h"

#include "GameInstance.h"

CPartObject_Body::CPartObject_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{ pDevice, pContext }
{
}

CPartObject_Body::CPartObject_Body(const CPartObject_Body& rhs)
	: CPartObject{ rhs }
{
}

HRESULT CPartObject_Body::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPartObject_Body::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CPartObject_Body::Priority_Tick(const _float& fTimeDelta)
{
}

void CPartObject_Body::Tick(const _float& fTimeDelta)
{

}

void CPartObject_Body::Late_Tick(const _float& fTimeDelta)
{

}

HRESULT CPartObject_Body::Render()
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
	/*for (int i = 0; i < 15; ++i)
	{
		_uint iTemp = { 1 };
		if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;

		m_pModelCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE);

		m_pShaderCom->Begin(0);
		m_pModelCom->Render(i);
	}*/

	return S_OK;
}

void CPartObject_Body::Free()
{
	__super::Free();
}
