#include "Tool_Dungeon.h"

#include "Tool_Manager.h"
#include "Tool_Object_Manager.h"

CTool_Dungeon::CTool_Dungeon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject { pDevice, pContext},
	m_pTool_Manager { CTool_Manager::GetInstance() },
	m_pObject_Manager { CTool_Object_Manager::GetInstance() }
{
	Safe_AddRef(m_pTool_Manager);
	Safe_AddRef(m_pObject_Manager);
}

CTool_Dungeon::CTool_Dungeon(const CTool_Dungeon& rhs)
	: CGameObject{ rhs },
	m_pTool_Manager { rhs.m_pTool_Manager },
	m_pObject_Manager { rhs.m_pObject_Manager }
{
	Safe_AddRef(m_pTool_Manager);
	Safe_AddRef(m_pObject_Manager);
}

HRESULT CTool_Dungeon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTool_Dungeon::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		DUNGEON_DESC* pDesc = (DUNGEON_DESC*)pArg;

		m_strObjectName = string(pDesc->pObjectName);
		m_strPrototypeVIBufferName = pDesc->strPrototypeVIBufferCom;
		m_strComVIBufferName = pDesc->strComVIBufferCOm;
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(-1.f, -1.f, -1.f);

	return S_OK;
}

void CTool_Dungeon::Priority_Tick(const _float& fTimeDelta)
{
}

void CTool_Dungeon::Tick(const _float& fTimeDelta)
{
	if (m_pTool_Manager->Is_PickingWithDungeon() && m_pGameInstance->GetMouseState(DIM_RB) == CInput_Device::TAP)
		Get_MousePos_On_Dungeon();

	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(m_pTool_Manager->Get_DungeonDegree()));
}

void CTool_Dungeon::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_PRIORITY, this);
}

HRESULT CTool_Dungeon::Render()
{
	if (FAILED(Bind_Resource()))
		return E_FAIL;

	_uint iNumMeshes = m_pVIBufferCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pVIBufferCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
			continue;

		m_pShaderCom->Begin(0);
		m_pVIBufferCom->Render(i);
	}
	
	return S_OK;
}

HRESULT CTool_Dungeon::Add_Components()
{
	if (FAILED(Add_Component(LEVEL_MAIN, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_MAIN, m_strPrototypeVIBufferName, m_strComVIBufferName, reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CTool_Dungeon::Bind_Resource()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CTool_Dungeon::Get_MousePos_On_Dungeon()
{
	_bool isSuccess = { false };

	_vector vMousePos = m_pGameInstance->Picking(&isSuccess);

	if (isSuccess)
	{
		if (FAILED(m_pObject_Manager->Add_CloneObject(
			CTool_Object_Manager::OBJECT_MONSTER,
			L"GameObject_Object",
			vMousePos,
			m_pTool_Manager->Get_CreateObjectIndex())))
			return;
	}
}

CTool_Dungeon* CTool_Dungeon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTool_Dungeon* pInstance = new CTool_Dungeon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CTool_Dungeon::Clone(void* pArg)
{
	CGameObject* pInstance = new CTool_Dungeon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CTool_Dungeon::Free()
{
	__super::Free();
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pTool_Manager);
}
