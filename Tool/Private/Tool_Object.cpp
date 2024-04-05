#include "Tool_Object.h"

#include "GameObject_Manager.h"
#include "GameInstance.h"
#include "Tool_Manager.h"
#include "Tool_Object_Manager.h"

CTool_Object::CTool_Object(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext },
	m_pObjectManager{ CTool_Object_Manager::GetInstance() }
{
	Safe_AddRef(m_pObjectManager);
}

CTool_Object::CTool_Object(const CTool_Object& rhs)
	: CGameObject{ rhs },
	m_pObjectManager{ rhs.m_pObjectManager }
{
	Safe_AddRef(m_pObjectManager);
}

HRESULT CTool_Object::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTool_Object::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		OBJECT_DESC* pDesc = (OBJECT_DESC*)pArg;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&pDesc->vPosition));
		string ObjectName(pDesc->pObjectName);
		m_strObjectName = ObjectName;

		m_strPrototypeVIBufferName = pDesc->strPrototypeVIBufferCom;
		m_strComVIBufferName = pDesc->strComVIBufferCOm;
	}

	return S_OK;
}

void CTool_Object::Priority_Tick(const _float& fTimeDelta)
{
}

void CTool_Object::Tick(const _float& fTimeDelta)
{
}

void CTool_Object::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CTool_Object::Render()
{
	return S_OK;
}

void CTool_Object::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pObjectManager);
}
