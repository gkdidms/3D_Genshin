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

void CTool_Dungeon::Set_Cells(vector<TOOL_CELL_DESC> Cells)
{
	m_Cells = Cells;

	for (auto& Point : m_Cells)
		m_pNavigationCom->Set_Points(Point.Points, Point.iOption);
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
		m_strComVIBufferName = pDesc->strComVIBufferCom;
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
	if (m_pTool_Manager->IsPickingWithDungeon() && m_pGameInstance->GetMouseState(DIM_RB) == CInput_Device::TAP)
		Get_MousePos_On_Dungeon();
	if (m_pTool_Manager->IsPickingCell() && m_pGameInstance->GetMouseState(DIM_RB) == CInput_Device::TAP)
		Picking_Cell();
	if (m_pTool_Manager->IsPickingPlayer() && m_pGameInstance->GetMouseState(DIM_RB) == CInput_Device::TAP)
		Picking_PlayerPos();
	if (m_pTool_Manager->isCellRemove() && m_pGameInstance->GetMouseState(DIM_RB) == CInput_Device::TAP)
	{
		isRemove();
	}
		

	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(m_pTool_Manager->Get_DungeonDegree()));

	_float3 vPos = m_pTool_Manager->Get_DungeonPos();
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(vPos.x, vPos.y, vPos.z, 1.f));
}

void CTool_Dungeon::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
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

	m_pNavigationCom->Render();
	
	return S_OK;
}

HRESULT CTool_Dungeon::Add_Components()
{
	if (FAILED(Add_Component(LEVEL_MAIN, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_MAIN, m_strPrototypeVIBufferName, m_strComVIBufferName, reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(Add_Component(LEVEL_MAIN, L"Prototype_Component_Navigation", L"Com_Navigation", reinterpret_cast<CComponent**>(&m_pNavigationCom))))
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
		POINT ptMouse;

		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		_int iIndex = m_pNavigationCom->Find_Index(ptMouse, vMousePos, m_pGameInstance->Get_PickDir(), m_pTransformCom->Get_WorldMatrix());
		if (FAILED(m_pObject_Manager->Add_CloneObject(
			CTool_Object_Manager::OBJECT_SCENE,
			L"GameObject_Object",
			vMousePos,
			iIndex,
			m_pTool_Manager->Get_CreateMonsterIndex())))
			return;
	}
}

void CTool_Dungeon::Picking_PlayerPos()
{
	_bool isSuccess = { false };

	_vector vMousePos = m_pGameInstance->Picking(&isSuccess);

	if (isSuccess)
	{
		POINT ptMouse;

		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);

		_float vPlayerPos[3] = { XMVectorGetX(vMousePos), XMVectorGetY(vMousePos), XMVectorGetZ(vMousePos) };
		m_pObject_Manager->Set_PlayerPos(vPlayerPos);

		_int iIndex = m_pNavigationCom->Find_Index(ptMouse, vMousePos, m_pGameInstance->Get_PickDir(), m_pTransformCom->Get_WorldMatrix());
		m_pObject_Manager->Set_PlayerNavigationIndex(iIndex);
	}
}

void CTool_Dungeon::Picking_Cell()
{
	_bool isSuccess = { false };

	_vector vMousePos = m_pGameInstance->Picking(&isSuccess);

	if (isSuccess)
	{
		//picking 성공하면 삼각형 정점 그려주기
		_float3 vMouseFloat3 = {};
		XMStoreFloat3(&vMouseFloat3, vMousePos);

		//근처에 있는 정점의 포인트로 좌표 변경
		Check_Point(&vMouseFloat3);

		XMStoreFloat3(&m_Points[m_iPointCount], XMLoadFloat3(&vMouseFloat3));

		++m_iPointCount;
		if (m_iPointCount == 3)
		{
			m_pNavigationCom->Set_Points(m_Points, m_pTool_Manager->Get_CellOption());

			TOOL_CELL_DESC tDesc{};
			memcpy(tDesc.Points, m_Points, sizeof(_float3) * 3);
			tDesc.iOption = m_pTool_Manager->Get_CellOption();
			tDesc.iIndex = m_Cells.size();
			m_Cells.emplace_back(tDesc);

			m_iPointCount = 0;
			ZeroMemory(m_Points, sizeof(_float3) * 3);
		}
	}
}



void CTool_Dungeon::Check_Point(_float3* vPoint)
{
	_float fDistance = 0.3f;

	if (m_Cells.size() <= 0)
		return;

	for (auto& CellPoints : m_Cells)
	{
		
		if (CellPoints.Points[0].x + fDistance >= (*vPoint).x && CellPoints.Points[0].x - fDistance <= (*vPoint).x)
		{
			if (CellPoints.Points[0].z + fDistance >= (*vPoint).z && CellPoints.Points[0].z - fDistance <= (*vPoint).z)
			{
				if (CellPoints.Points[0].y + fDistance >= (*vPoint).y && CellPoints.Points[0].y - fDistance <= (*vPoint).y)
				{
					*vPoint = CellPoints.Points[0];
					return;
				}
			}
		}
		if (CellPoints.Points[1].x + fDistance >= (*vPoint).x && CellPoints.Points[1].x - fDistance <= (*vPoint).x)
		{
			if (CellPoints.Points[1].z + fDistance >= (*vPoint).z && CellPoints.Points[1].z - fDistance <= (*vPoint).z)
			{
				if (CellPoints.Points[1].y + fDistance >= (*vPoint).y && CellPoints.Points[1].y - fDistance <= (*vPoint).y)
				{
					*vPoint = CellPoints.Points[1];
					return;
				}
			}
				
		}
		if (CellPoints.Points[2].x + fDistance >= (*vPoint).x && CellPoints.Points[2].x - fDistance <= (*vPoint).x)
		{
			if (CellPoints.Points[2].z + fDistance >= (*vPoint).z && CellPoints.Points[2].z - fDistance <= (*vPoint).z)
			{
				if (CellPoints.Points[2].y + fDistance >= (*vPoint).y && CellPoints.Points[2].y - fDistance <= (*vPoint).y)
				{
					*vPoint = CellPoints.Points[2];
					return;
				}

			}
		}
	}
}

_bool CTool_Dungeon::isRemove()
{
	_int iIndex = { -1 };
	_int iVecterIndex = { 0 };
	_bool isSuccess = { false };

	_vector vMousePos = m_pGameInstance->Picking(&isSuccess);

	if (isSuccess)
	{
		POINT ptMouse;
		GetCursorPos(&ptMouse);
		ScreenToClient(g_hWnd, &ptMouse);
		iIndex = m_pNavigationCom->Find_Index(ptMouse, vMousePos, m_pGameInstance->Get_PickDir(), m_pTransformCom->Get_WorldMatrix(), &iVecterIndex);
	}
	else
		return false;
	//셀 지우기
	if (iIndex != -1 && m_pNavigationCom->isRemove(iIndex))
	{
		if (m_Cells[iVecterIndex].iIndex == iIndex)
		{
			m_Cells.erase(m_Cells.begin() + iVecterIndex);

			return true;
		}
	}
	return false;
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

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);

	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pTool_Manager);
}
