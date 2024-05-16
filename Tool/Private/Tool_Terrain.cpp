#include "Tool_Terrain.h"

#include "Tool_Object_Manager.h"
#include "GameInstance.h"
#include "Tool_Manager.h"
#include "Tool_Non_Object.h"

CTool_Terrain::CTool_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext },
	m_pObject_Manager{ CTool_Object_Manager::GetInstance() },
	m_pTool_Manager { CTool_Manager::GetInstance() }
{
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pTool_Manager);
}

CTool_Terrain::CTool_Terrain(const CTool_Terrain& rhs)
	: CGameObject{ rhs },
	m_pObject_Manager{ rhs.m_pObject_Manager },
	m_pTool_Manager{rhs.m_pTool_Manager}
{
	Safe_AddRef(m_pObject_Manager);
	Safe_AddRef(m_pTool_Manager);
}

HRESULT CTool_Terrain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTool_Terrain::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;


	return S_OK;
}

void CTool_Terrain::Priority_Tick(const _float& fTimeDelta)
{
}

void CTool_Terrain::Tick(const _float& fTimeDelta)
{
	if (CTool_Manager::mTerrainPicking && m_pGameInstance->GetMouseState(DIM_RB) == CInput_Device::TAP)
		Get_MousePos_On_Terrain();

	m_pTransformCom->Set_Scale(CTool_Manager::fScaleTerrainX, 1.f, CTool_Manager::fScaleTerrainZ);
}

void CTool_Terrain::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_PRIORITY, this);
}

HRESULT CTool_Terrain::Render()
{
	if (m_pTool_Manager->IsShowTerrain())
		return S_OK;

	if (FAILED(Bind_ResouceData()))
		return E_FAIL;

	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CTool_Terrain::Add_Components()
{
	if (FAILED(Add_Component(LEVEL_MAIN, L"Prototype_Component_Shader", L"Component_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_MAIN, L"Prototype_Component_Texture", L"Component_Texture", reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_MAIN, L"Prototype_Component_VIBuffer_Terrain", L"Component_VIBuffer_Terrain", reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_MAIN, L"Prototype_Component_Calculator", L"Component_Calculator", reinterpret_cast<CComponent**>(&m_pCalculatorCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CTool_Terrain::Bind_ResouceData()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;
	//m_pTextureCom->Bind_ShaderResource
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

void CTool_Terrain::Get_MousePos_On_Terrain()
{

	_bool IsPicking = { false };
	_vector vMousePos = m_pGameInstance->Picking(&IsPicking);
	
	if (IsPicking)
	{
		_float3 vMousePosFloat3;
		XMStoreFloat3(&vMousePosFloat3, vMousePos);

		CTool_Manager::vWorldMousePos = vMousePosFloat3;

		if (FAILED(m_pObject_Manager->Add_CloneObject(
			CTool_Object_Manager::OBJECT_SCENE,
			L"GameObject_Object",
			vMousePos,
			0,
			m_pTool_Manager->Get_CreateMonsterIndex())))
			return;
	}
}

CTool_Terrain* CTool_Terrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTool_Terrain* pInstance = new CTool_Terrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CTool_Terrain::Clone(void* pArg)
{
	CTool_Terrain* pInstance = new CTool_Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CTool_Terrain::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pCalculatorCom);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pTool_Manager);
}
