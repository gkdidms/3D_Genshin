#include "PartObject_Weapon.h"

#include "GameInstance.h"

CPartObject_Weapon::CPartObject_Weapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{ pDevice, pContext }
{
}

CPartObject_Weapon::CPartObject_Weapon(const CPartObject_Weapon& rhs)
	: CPartObject{ rhs }
{
}

HRESULT CPartObject_Weapon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPartObject_Weapon::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CPartObject_Weapon::Priority_Tick(const _float& fTimeDelta)
{
}

void CPartObject_Weapon::Tick(const _float& fTimeDelta)
{
}

void CPartObject_Weapon::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CPartObject_Weapon::Render()
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

	return S_OK;
}

void CPartObject_Weapon::Free()
{
	__super::Free();
}
