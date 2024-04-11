#include "Dungeon_2.h"

#include "GameInstance.h"

CDungeon_2::CDungeon_2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMap{ pDevice, pContext }
{
}

CDungeon_2::CDungeon_2(const CDungeon_2& rhs)
	: CMap{ rhs }
{
}

HRESULT CDungeon_2::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDungeon_2::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CDungeon_2::Priority_Tick(const _float& fTimeDelta)
{
}

void CDungeon_2::Tick(const _float& fTimeDelta)
{
}

void CDungeon_2::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CDungeon_2::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (int i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
			continue;

		m_pShaderCom->Begin(0);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CDungeon_2::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxMesh", L"Component_Shader_VtxMesh", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Dungeon_2", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CDungeon_2::Bind_ResourceData()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

CDungeon_2* CDungeon_2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDungeon_2* pInstance = new CDungeon_2(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CDungeon_2::Clone(void* pArg)
{
	CGameObject* pInstance = new CDungeon_2(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CDungeon_2::Free()
{
	__super::Free();
}
