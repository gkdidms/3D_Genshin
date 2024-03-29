#include "Tool_Object.h"

#include "GameInstance.h"
#include "Tool_Manager.h"

CTool_Object::CTool_Object(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CTool_Object::CTool_Object(const CTool_Object& rhs)
	: CGameObject{ rhs }
{
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

	if (FAILED(Add_Components()))
		return E_FAIL;

	CTool_Manager::Objects.push_back(this);
	this->AddRef();

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
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CTool_Object::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	for (int i = 0; i < m_pVIBufferCom->Get_NumMeshes(); ++i)
	{
		//if (FAILED(m_pVIBufferCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
		//	return E_FAIL;

		m_pVIBufferCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE);

		m_pShaderCom->Begin(0);
		m_pVIBufferCom->Render(i);
	}
	
	return S_OK;
}

HRESULT CTool_Object::Add_Components()
{
	if (FAILED(Add_Component(LEVEL_MAIN, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_MAIN, m_strPrototypeVIBufferName, m_strComVIBufferName, reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CTool_Object::Bind_ResourceData()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

CTool_Object* CTool_Object::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTool_Object* pInstance = new CTool_Object(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CTool_Object::Clone(void* pArg)
{
	CTool_Object* pInstance = new CTool_Object(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CTool_Object::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
