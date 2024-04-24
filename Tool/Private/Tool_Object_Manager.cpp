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

HRESULT CTool_Object_Manager::Initialize()
{
	m_CloneDesc[OBJECT_MONSTER] = {
		
		CLONE_DESC { 0, "Fiona", L"Prototype_Component_Model_Fiona", L"Com_Fiona_Model", false},
		CLONE_DESC { 1, "tartaglia", L"Prototype_Component_Model_Tarta", L"Com_Tarta_Model", false},
		CLONE_DESC { 2, "Nillou", L"Prototype_Component_Model_Nillou", L"Com_Nillou_Model", true},
		CLONE_DESC { 3, "Tighnari", L"Prototype_Component_Model_Tighnari", L"Com_Tighnari_Model", true}
	};

	m_CloneDesc[OBJECT_DUNGEON] = {
		CLONE_DESC { 0, "Dungeon_1", L"Prototype_Component_Model_Dungeon_1", L"Com_Dungeon"},
		CLONE_DESC { 1, "Dungeon_2", L"Prototype_Component_Model_Dungeon_2", L"Com_Dungeon"},
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

HRESULT CTool_Object_Manager::Add_CloneObject(OBJECTTYPE eType, wstring strLayerTag, _vector vObjPos, _uint iObjectIndex)
{
	_float3 vObjPosFloat3;
	XMStoreFloat3(&vObjPosFloat3, vObjPos);

	if (eType == OBJECT_MONSTER)
	{
		CTool_Non_Object::OBJECT_DESC tDesc{};
		strcpy_s(tDesc.pObjectName, m_CloneDesc[OBJECT_MONSTER][iObjectIndex].strName.c_str());
		tDesc.vPosition = _float4(vObjPosFloat3.x, vObjPosFloat3.y, vObjPosFloat3.z, 1.f);
		tDesc.strPrototypeVIBufferCom = m_CloneDesc[OBJECT_MONSTER][iObjectIndex].strPrototypeVIBufferCom;
		tDesc.strComVIBufferCom = m_CloneDesc[OBJECT_MONSTER][iObjectIndex].strComVIBufferCom;

		CTool_Object* pGameObject = { nullptr };

		if (m_CloneDesc[OBJECT_MONSTER][iObjectIndex].IsAnimation)
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

	_uint NumCells = Cells.size();
	ofs.write((_char*)&NumCells, sizeof(_uint));

	for (auto& Point : Cells)
	{
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

		Cells.emplace_back(tDesc);
	}

	dynamic_cast<CTool_Dungeon*>(m_Terrains[0])->Set_Cells(Cells);

	ifs.close();

	return S_OK;
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

		Add_CloneObject(OBJECT_DUNGEON, L"Layer_Dungeon", XMVectorSet(0.f, 0.f, 0.f, 1.f), iNumObject);

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
		
		_uint iNumObjectIndex = { 0 };
		
		find_if(m_CloneDesc[OBJECT_MONSTER].begin(), m_CloneDesc[OBJECT_MONSTER].end(), [&](CLONE_DESC Desc)->_bool {
			if (!strcmp(strObjectName, Desc.strName.c_str()))
			{
				iNumObjectIndex = Desc.iIndex;
				return true;
			}
			
			return false;
		});

		Add_CloneObject(OBJECT_MONSTER, L"Layer_Object", XMVectorSet(0.f, 0.f, 0.f, 1.f), iNumObjectIndex);

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
