#include "Tool_Anim_Object.h"

#include "GameInstance.h"
#include "Tool_Manager.h"
#include "Tool_Object_Manager.h"

CEffect_Anim_Model::CEffect_Anim_Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTool_Object{ pDevice, pContext }
{
}

CEffect_Anim_Model::CEffect_Anim_Model(const CEffect_Anim_Model& rhs)
	: CTool_Object{ rhs }
{
}

HRESULT CEffect_Anim_Model::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Anim_Model::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	CModel::ANIM_DESC AnimDesc{0, true, true, false };
	m_pVIBufferCom->Set_Animation(AnimDesc);

	return S_OK;
}

void CEffect_Anim_Model::Priority_Tick(const _float& fTimeDelta)
{
}

void CEffect_Anim_Model::Tick(const _float& fTimeDelta)
{
	_float4x4 MoveMatrix;
	m_pVIBufferCom->Play_Animation(fTimeDelta, &MoveMatrix);
}

void CEffect_Anim_Model::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CEffect_Anim_Model::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	for (int i = 0; i < m_pVIBufferCom->Get_NumMeshes(); ++i)
	{
		if (FAILED(m_pVIBufferCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;

		if (FAILED(m_pVIBufferCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
			continue;

		//m_pVIBufferCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE);

		m_pShaderCom->Begin(0);
		m_pVIBufferCom->Render(i);
	}

	return S_OK;
}

HRESULT CEffect_Anim_Model::Add_Components()
{
	if (FAILED(Add_Component(LEVEL_MAIN, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_MAIN, m_strPrototypeVIBufferName, m_strComVIBufferName, reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Anim_Model::Bind_ResourceData()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

CEffect_Anim_Model* CEffect_Anim_Model::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Anim_Model* pInstance = new CEffect_Anim_Model(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CEffect_Anim_Model::Clone(void* pArg)
{
	CEffect_Anim_Model* pInstance = new CEffect_Anim_Model(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CEffect_Anim_Model::Free()
{
	__super::Free();
}
