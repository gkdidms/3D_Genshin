#include "Tool_Object_Manager.h"

#include "GameInstance.h"

#include "Tool_Non_Object.h"
#include "Tool_Dungeon.h"
#include "Tool_Manager.h"

IMPLEMENT_SINGLETON(CTool_Object_Manager)
CTool_Object_Manager::CTool_Object_Manager()
    : m_pGameInstance{ CGameInstance::GetInstance() }
{
    Safe_AddRef(m_pGameInstance);
}

void CTool_Object_Manager::Remove_Object(int iIndex)
{
	Safe_Release(m_Objects[iIndex]);
	m_Objects.erase(m_Objects.begin() + iIndex);
}

HRESULT CTool_Object_Manager::Initialize()
{
	m_CloneDesc[OBJECT_SCENE] = {
		
		CLONE_DESC { 0, "Slime_Fire_Large", L"Prototype_Component_Model_Slime_Fire", L"Com_Monster", true},
		CLONE_DESC { 1, "Slime_Water_Large", L"Prototype_Component_Model_Slime_Water", L"Com_Monster", true},
		CLONE_DESC { 2, "Slime_Fire_Mid", L"Prototype_Component_Model_Slime_Fire", L"Com_Monster", true},
		CLONE_DESC { 3, "Slime_Water_Mid", L"Prototype_Component_Model_Slime_Water", L"Com_Monster", true},
		CLONE_DESC { 4, "TreasureBox", L"Prototype_Component_Model_TreasureBox", L"Com_SceneObject", true},
		CLONE_DESC { 5, "TreasureBox_Big", L"Prototype_Component_Model_TreasureBox_Big", L"Com_SceneObject", true},
		CLONE_DESC { 6, "WindField", L"Prototype_Component_Model_WindField", L"Com_SceneObject", false},
		CLONE_DESC { 7, "Hili_Fire_Bow", L"Prototype_Component_Model_Hili_Fire", L"Com_Monster", true},
		CLONE_DESC { 8, "Hili_Fire_Club", L"Prototype_Component_Model_Hili_Fire", L"Com_Monster", true},
		CLONE_DESC { 9, "Hili_Electric_Bow", L"Prototype_Component_Model_Hili_Electric", L"Com_Monster", true},
		CLONE_DESC { 10, "Hili_Electric_Club", L"Prototype_Component_Model_Hili_Electric", L"Com_Monster", true},
		CLONE_DESC { 11, "Boss_Tartaglia", L"Prototype_Component_Model_Boss_Tarta", L"Com_Boss", true},
		CLONE_DESC { 12, "CheckPoint", L"Prototype_Component_Model_ChecKPoint", L"Com_SceneObject", false},
		CLONE_DESC { 13, "FireCore", L"Prototype_Component_Model_FireCore", L"Com_SceneObject", true},
		CLONE_DESC { 14, "DungeonGate", L"Prototype_Component_Model_DungeonGate", L"Com_SceneObject", true},
		CLONE_DESC { 15, "Operator", L"Prototype_Component_Model_Operator", L"Com_SceneObject", true},
		CLONE_DESC { 16, "MovePlane", L"Prototype_Component_Model_Plane", L"Com_SceneObject", false},
		CLONE_DESC { 17, "Plane", L"Prototype_Component_Model_Plane", L"Com_SceneObject", false},
		CLONE_DESC { 18, "ThornWall", L"Prototype_Component_Model_ThornWall", L"Com_SceneObject", false},
	};

	m_CloneDesc[OBJECT_DUNGEON] = {
		CLONE_DESC { 0, "Dungeon_1", L"Prototype_Component_Model_Dungeon_1", L"Com_Dungeon"},
		CLONE_DESC { 1, "Dungeon_2", L"Prototype_Component_Model_Dungeon_2", L"Com_Dungeon"},
		CLONE_DESC { 2, "GoldenHouse", L"Prototype_Component_Model_GoldenHouse", L"Com_Dungeon"},
	};

	return S_OK;
}

void CTool_Object_Manager::Priority_Tick(const _float& fTimeDelta)
{
	for (auto& pTerrain : m_Terrains)
		pTerrain->Priority_Tick(fTimeDelta);

	for (auto& pCloneObject : m_Objects)
		pCloneObject->Priority_Tick(fTimeDelta);
}

void CTool_Object_Manager::Tick(const _float& fTimeDelta)
{
	for (auto& pTerrain : m_Terrains)
		pTerrain->Tick(fTimeDelta);

	for (auto& pCloneObject : m_Objects)
		pCloneObject->Tick(fTimeDelta);
}

void CTool_Object_Manager::Late_Tick(const float& fTimeDelta)
{
	for (auto& pTerrain : m_Terrains)
		pTerrain->Late_Tick(fTimeDelta);

	for (auto& pCloneObject : m_Objects)
		pCloneObject->Late_Tick(fTimeDelta);
}

HRESULT CTool_Object_Manager::Render()
{
	for (auto& pTerrain : m_Terrains)
	{
		if (FAILED(pTerrain->Render()))
			return E_FAIL;
	}

	for (auto& pCloneObject : m_Objects)
	{
		if (FAILED(pCloneObject->Render()))
			return E_FAIL;
	}
		
	return S_OK;
}

HRESULT CTool_Object_Manager::Add_CloneObject(OBJECTTYPE eType, wstring strLayerTag, _vector vObjPos, _uint iNavigationIndex, _uint iObjectIndex)
{
	_float3 vObjPosFloat3;
	XMStoreFloat3(&vObjPosFloat3, vObjPos);

	if (eType == OBJECT_SCENE)
	{
		CTool_Non_Object::OBJECT_DESC tDesc{};
		
		strcpy_s(tDesc.pObjectName, m_CloneDesc[OBJECT_SCENE][iObjectIndex].strName.c_str());
		strcat_s(tDesc.pObjectName, "_%d");
		sprintf_s(tDesc.pObjectName, tDesc.pObjectName, m_Objects.size());

		tDesc.vPosition = _float4(vObjPosFloat3.x, vObjPosFloat3.y, vObjPosFloat3.z, 1.f);
		tDesc.strPrototypeVIBufferCom = m_CloneDesc[OBJECT_SCENE][iObjectIndex].strPrototypeVIBufferCom;
		tDesc.strComVIBufferCom = m_CloneDesc[OBJECT_SCENE][iObjectIndex].strComVIBufferCom;
		tDesc.iObjectNavigationIndex = iNavigationIndex;

		CTool_Object* pGameObject = { nullptr };

		if (m_CloneDesc[OBJECT_SCENE][iObjectIndex].IsAnimation)
			pGameObject = dynamic_cast<CTool_Object*>(m_pGameInstance->Clone_Object(L"Prototype_GameObject_AnimObject", &tDesc));
		else
			pGameObject = dynamic_cast<CTool_Object*>(m_pGameInstance->Clone_Object(L"Prototype_GameObject_Object", &tDesc));

		if (nullptr == pGameObject)
			return E_FAIL;

		m_Objects.emplace_back(pGameObject);
	}
	else if (eType == OBJECT_DUNGEON)
	{
		CTool_Dungeon::DUNGEON_DESC tDesc{};
		strcpy_s(tDesc.pObjectName, m_CloneDesc[OBJECT_DUNGEON][iObjectIndex].strName.c_str());
		tDesc.strPrototypeVIBufferCom = m_CloneDesc[OBJECT_DUNGEON][iObjectIndex].strPrototypeVIBufferCom;
		tDesc.strComVIBufferCom = m_CloneDesc[OBJECT_DUNGEON][iObjectIndex].strComVIBufferCom;

		CGameObject* pGameObject = m_pGameInstance->Clone_Object(L"Prototype_GameObject_Dungeon", &tDesc);
		if (nullptr == pGameObject)
			return E_FAIL;

		m_Terrains.emplace_back(pGameObject);
	}

    return S_OK;
}

HRESULT CTool_Object_Manager::Cell_Save(const _char* pFileName)
{
	char pFilePath[MAX_PATH] = "../../Data/Navigation/";
	strcat_s(pFilePath, pFileName);

	ofstream ofs(pFilePath, ios::binary | ios::out);

	if (ofs.fail())
		return E_FAIL;

	vector<CTool_Dungeon::TOOL_CELL_DESC> Cells = dynamic_cast<CTool_Dungeon*>(m_Terrains[0])->Get_Cells();

	for (size_t i = 0; i < Cells.size(); ++i)
	{
		_bool isResult = Check_SameCells(i, Cells[i].Points, Cells);

		if (isResult || !SamePoint(Cells[i].Points))
		{
			Cells.erase(Cells.begin() + i);
		}
	}

	_uint NumCells = Cells.size();
	ofs.write((_char*)&NumCells, sizeof(_uint));

	for (auto& Point : Cells)
	{
		SortCell(Point.Points);
			
		ofs.write((_char*)Point.Points, sizeof(_float3) * 3);
		ofs.write((_char*)&Point.iOption, sizeof(_int));
	}

	ofs.close();

	return S_OK;
}

HRESULT CTool_Object_Manager::Cell_Load(const _char* pFileName)
{
	if (m_Terrains.size() == 0)
		return S_OK;
		
	char pFilePath[MAX_PATH] = "../../Data/Navigation/";
	strcat_s(pFilePath, pFileName);

	ifstream ifs(pFilePath, ios::binary | ios::in);

	if (ifs.fail())
		return E_FAIL;

	vector<CTool_Dungeon::TOOL_CELL_DESC> Cells;

	_uint NumCells = { 0 };
	ifs.read((_char*)&NumCells, sizeof(_uint));

	for (size_t i = 0; i < NumCells; ++i)
	{
		CTool_Dungeon::TOOL_CELL_DESC tDesc{};

		ifs.read((_char*)tDesc.Points, sizeof(_float3) * 3);
		ifs.read((_char*)&tDesc.iOption, sizeof(_int));
		tDesc.iIndex = i;

		SortCell(tDesc.Points);
		Cells.emplace_back(tDesc);
	}

	dynamic_cast<CTool_Dungeon*>(m_Terrains[0])->Set_Cells(Cells);

	ifs.close();

	return S_OK;
}

void CTool_Object_Manager::SortCell(_float3* Points)
{
	_vector vUP = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	XMVECTOR ab = XMLoadFloat3(&Points[1]) - XMLoadFloat3(&Points[0]);
	XMVECTOR bc = XMLoadFloat3(&Points[2]) - XMLoadFloat3(&Points[1]);

	// ab와 bc의 크로스 제품 계산
	XMVECTOR cross = XMVector4Normalize(XMVector3Cross(ab, bc));

	float dotY = XMVectorGetX(XMVector4Dot(cross, vUP));

	if (dotY < 0)
	{
		swap(Points[1], Points[2]);
	}
}

_bool CTool_Object_Manager::SamePoint(_float3* Points)
{
	if (XMVector3Equal(XMLoadFloat3(&Points[0]), XMLoadFloat3(&Points[1])))
		return false;
	else if (XMVector3Equal(XMLoadFloat3(&Points[1]), XMLoadFloat3(&Points[2])))
		return false;
	else if (XMVector3Equal(XMLoadFloat3(&Points[2]), XMLoadFloat3(&Points[0])))
		return false;

	return true;
}
_bool CTool_Object_Manager::Check_SameCells(_int iIndex, _float3* vPoint, vector<CTool_Dungeon::TOOL_CELL_DESC> Cells)
{
	for (size_t i = 0; i < Cells.size(); ++i)
	{
		if (i == iIndex)
			continue;

		_uint iCount = 0;
		_bool isResult[3] = { false, false, false };
		for (auto& CellPoint : Cells[i].Points)
		{
			if (XMVector3Equal(XMLoadFloat3(&vPoint[0]), XMLoadFloat3(&CellPoint))
				|| XMVector3Equal(XMLoadFloat3(&vPoint[1]), XMLoadFloat3(&CellPoint))
				|| XMVector3Equal(XMLoadFloat3(&vPoint[2]), XMLoadFloat3(&CellPoint)))
			{
				isResult[iCount] = true;
			}

			iCount++;
		}

		if (isResult[0] && isResult[1] && isResult[2])
			return true;
	}

	return false;
}
HRESULT CTool_Object_Manager::Save(const _char* pFileName)
{
	char pFilePath[MAX_PATH] = "../../Data/Stage/";
	strcat_s(pFilePath, pFileName);

	ofstream ofs(pFilePath, ios::binary | ios::out);

	if (ofs.fail())
		return E_FAIL;

	//플레이어 위치 저장
	ofs.write((_char*)&m_vPlayerPos, sizeof(_float) * 3);
	ofs.write((_char*)&m_iPlayerNavigationIndex, sizeof(_int));

	//지형 (하나)
	_uint iNumDungeon = m_Terrains.size();
	ofs.write((_char*)&iNumDungeon, sizeof(_uint));

	if (iNumDungeon != 0)
	{
		CTool_Dungeon* pDungeon = dynamic_cast<CTool_Dungeon*>(m_Terrains[0]);

		_uint iNumDungeonObjectName = pDungeon->Get_ObjectName().size() + 1;
		string strDungeonName = pDungeon->Get_ObjectName();

		ofs.write((_char*)&iNumDungeonObjectName, sizeof(_uint));
		ofs.write(strDungeonName.c_str(), iNumDungeonObjectName);

		const _float4x4* WorldMatrix = pDungeon->m_pTransformCom->Get_WorldFloat4x4();
		ofs.write((_char*)WorldMatrix, sizeof(_float4x4));
	}

	_uint iNumObjects = m_Objects.size();
	ofs.write((_char*)&iNumObjects, sizeof(_uint));
	//오브젝트
	for (auto& pObject : m_Objects)
	{
		string strObjectName = pObject->Get_ObjectName();
		_uint iNumObjectName = pObject->Get_ObjectName().size() + 1;
		
		ofs.write((_char*)&iNumObjectName, sizeof(_uint));
		ofs.write(strObjectName.c_str(), sizeof(_char)*iNumObjectName);

		_uint iNavigationIndex = pObject->Get_NavigationIndex();
		ofs.write((_char*)&iNavigationIndex, sizeof(_uint));

		const _float4x4* WorldMatrix = pObject->m_pTransformCom->Get_WorldFloat4x4();
		ofs.write((_char*)WorldMatrix, sizeof(_float4x4));
	}

	ofs.close();

	return S_OK;
}

HRESULT CTool_Object_Manager::Load(const _char* pFileName)
{
	Release_Object();

	char pFilePath[MAX_PATH] = "../../Data/Stage/";
	strcat_s(pFilePath, pFileName);

	ifstream ifs(pFilePath, ios::binary | ios::in);

	if (ifs.fail())
		return E_FAIL;
	
	//플레이어
	ifs.read((_char*)&m_vPlayerPos, sizeof(_float) * 3);
	ifs.read((_char*)&m_iPlayerNavigationIndex, sizeof(_int));

	//지형 (하나)
	_uint iNumDungeon = { 0 };
	ifs.read((_char*)&iNumDungeon, sizeof(_uint));

	if (iNumDungeon != 0)
	{
		_uint iNumDungeonObjectName = { 0 };
		char pDungeonName[MAX_PATH] = {""};

		ifs.read((_char*)&iNumDungeonObjectName, sizeof(_uint));
		ifs.read((_char*)pDungeonName, iNumDungeonObjectName);

		int iNumObject = { 0 };
		if (!strcmp(pDungeonName, "Dungeon_1"))
			iNumObject = 0;
		else if (!strcmp(pDungeonName,"Dungeon_2"))
			iNumObject = 1;
		else if (!strcmp(pDungeonName, "GoldenHouse"))
			iNumObject = 2;

		Add_CloneObject(OBJECT_DUNGEON, L"Layer_Dungeon", XMVectorSet(0.f, 0.f, 0.f, 1.f), 0, iNumObject);

		_float4x4 WorldMatrix = {};
		ifs.read((_char*)&WorldMatrix, sizeof(_float4x4));

		m_Terrains[0]->m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&WorldMatrix));

		CTool_Manager::GetInstance()->Set_DungeonPos(XMVectorSet(WorldMatrix.m[3][0], WorldMatrix.m[3][1], WorldMatrix.m[3][2], 1.f));
	}

	_uint iNumObjects = { 0 };
	ifs.read((_char*)&iNumObjects, sizeof(_uint));

	for (int i = 0; i < iNumObjects; ++i)
	{
		char strObjectName[MAX_PATH] = {""};
		_uint iNumObjectName = { 0 };

		ifs.read((_char*)&iNumObjectName, sizeof(_uint));
		ifs.read((_char*)strObjectName, iNumObjectName);

		_uint iNavigationIndex = 0;
		ifs.read((_char*)&iNavigationIndex, sizeof(_uint));
		
		_uint iNumObjectIndex = { 0 };

		find_if(m_CloneDesc[OBJECT_SCENE].begin(), m_CloneDesc[OBJECT_SCENE].end(), [&](CLONE_DESC Desc)->_bool {
			
			if (string(strObjectName).find(Desc.strName.c_str()) != string::npos)
			{
				iNumObjectIndex = Desc.iIndex;
				return true;
			}
			
			return false;
		});

		Add_CloneObject(OBJECT_SCENE, L"Layer_Object", XMVectorSet(0.f, 0.f, 0.f, 1.f), iNavigationIndex, iNumObjectIndex);

		_float4x4 WorldMatrix = {};
		ifs.read((_char*)&WorldMatrix, sizeof(_float4x4));

		m_Objects[i]->m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&WorldMatrix));
	}

	ifs.close();
	return S_OK;
}

void CTool_Object_Manager::Release_Object()
{
	for (auto& iter : m_Terrains)
		Safe_Release(iter);
	m_Terrains.clear();

	for (auto& iter : m_Objects)
		Safe_Release(iter);
	m_Objects.clear();
}

void CTool_Object_Manager::Free()
{
	for (auto& iter : m_Terrains)
		Safe_Release(iter);

	for (auto& iter : m_Objects)
		Safe_Release(iter);

    Safe_Release(m_pGameInstance);
}
