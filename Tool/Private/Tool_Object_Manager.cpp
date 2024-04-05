#include "Tool_Object_Manager.h"

#include "GameInstance.h"

#include "Tool_Non_Object.h"
#include "Tool_Dungeon.h"

IMPLEMENT_SINGLETON(CTool_Object_Manager)
CTool_Object_Manager::CTool_Object_Manager()
    : m_pGameInstance{ CGameInstance::GetInstance() }
{
    Safe_AddRef(m_pGameInstance);
}

HRESULT CTool_Object_Manager::Initialize()
{
	m_CloneDesc[OBJECT_MONSTER] = {
		
		CLONE_DESC { "Fiona", L"Prototype_Component_Model_Fiona", L"Com_Fiona_Model", false},
		CLONE_DESC { "tartaglia", L"Prototype_Component_Model_Tarta", L"Com_Tarta_Model", false},
		CLONE_DESC { "Nillou", L"Prototype_Component_Model_Nillou", L"Com_Nillou_Model", true},
		CLONE_DESC { "Tighnari", L"Prototype_Component_Model_Tighnari", L"Com_Tighnari_Model", false}
	};

	m_CloneDesc[OBJECT_DUNGEON] = {
		CLONE_DESC { "Dungeon_1", L"Prototype_Component_Model_Dungeon_1", L"Com_Dungeon"},
		CLONE_DESC { "Dungeon_2", L"Prototype_Component_Model_Dungeon_2", L"Com_Dungeon"},
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
		tDesc.strComVIBufferCOm = m_CloneDesc[OBJECT_MONSTER][iObjectIndex].strComVIBufferCom;

		CGameObject* pGameObject = { nullptr };

		if (m_CloneDesc[OBJECT_MONSTER][iObjectIndex].IsAnimation)
			pGameObject = m_pGameInstance->Clone_Object(L"Prototype_GameObject_AnimObject", &tDesc);
		else
			pGameObject = m_pGameInstance->Clone_Object(L"Prototype_GameObject_Object", &tDesc);

		if (nullptr == pGameObject)
			return E_FAIL;

		m_Objects.emplace_back(pGameObject);
	}
	else if (eType == OBJECT_DUNGEON)
	{
		CTool_Dungeon::DUNGEON_DESC tDesc{};
		strcpy_s(tDesc.pObjectName, m_CloneDesc[OBJECT_DUNGEON][iObjectIndex].strName.c_str());
		tDesc.strPrototypeVIBufferCom = m_CloneDesc[OBJECT_DUNGEON][iObjectIndex].strPrototypeVIBufferCom;
		tDesc.strComVIBufferCOm = m_CloneDesc[OBJECT_DUNGEON][iObjectIndex].strComVIBufferCom;

		CGameObject* pGameObject = m_pGameInstance->Clone_Object(L"Prototype_GameObject_Dungeon", &tDesc);
		if (nullptr == pGameObject)
			return E_FAIL;

		m_Terrains.emplace_back(pGameObject);
	}

    return S_OK;
}

void CTool_Object_Manager::Release_Object()
{
	for (auto& iter : m_Terrains)
		Safe_Release(iter);

	for (auto& iter : m_Objects)
		Safe_Release(iter);
}

void CTool_Object_Manager::Free()
{
	for (auto& iter : m_Terrains)
		Safe_Release(iter);

	for (auto& iter : m_Objects)
		Safe_Release(iter);

    Safe_Release(m_pGameInstance);
}
