#include "Effect_Mesh.h"

#include "GameInstance.h"

CEffect_Mesh::CEffect_Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTool_Effect{ pDevice, pContext }
{
}

CEffect_Mesh::CEffect_Mesh(const CEffect_Mesh& rhs)
	: CTool_Effect { rhs }
{
}

HRESULT CEffect_Mesh::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Mesh::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	EFFECT_MESH_DESC* pDesc = static_cast<EFFECT_MESH_DESC*>(pArg);
	strcpy_s(m_szModelFilePath, pDesc->szModelFilePath);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CEffect_Mesh::Priority_Tick(const _float& fTimeDelta)
{
}

void CEffect_Mesh::Tick(const _float& fTimeDelta)
{
}

void CEffect_Mesh::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDERER_STATE(m_iRendererType), this);
}

HRESULT CEffect_Mesh::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (int i = 0; i < iNumMeshes; ++i)
	{
		m_pShaderCom->Begin(m_iShaderPass);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CEffect_Mesh::Add_Components()
{
	if (FAILED(Add_Component(LEVEL_MAIN, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	_matrix PreMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	m_pModelCom = CModel::Create(m_pDevice, m_pContext, m_szModelFilePath, PreMatrix, m_szModelFilePath);

	if (nullptr == m_pModelCom)
		return E_FAIL;

	if (FAILED(__super::Add_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Mesh::Bind_ResourceData()
{
	if (FAILED(__super::Bind_ResourceData()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_UV", &m_UV, sizeof(_float2))))
		return E_FAIL;

	return S_OK;
}

CEffect_Mesh* CEffect_Mesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Mesh* pInstance = new CEffect_Mesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CEffect_Mesh::Clone(void* pArg)
{
	CEffect_Mesh* pInstance = new CEffect_Mesh(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CEffect_Mesh::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
}
