#include "Tool_Object_Manager.h"

#include "GameInstance.h"

#include "Tool_Object.h"
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
		CLONE_DESC { "tartaglia", L"Prototype_Component_Model_Tarta", L"Com_Tarta_Model"},
		CLONE_DESC { "Nillou", L"Prototype_Component_Model_Nillou", L"Com_Nillou_Model"}
	};

	m_CloneDesc[OBJECT_DUNGEON] = {
		CLONE_DESC { "Dungeon_1", L"Prototype_Component_Model_Dungeon_1", L"Com_Dungeon"},
		CLONE_DESC { "Dungeon_2", L"Prototype_Component_Model_Dungeon_2", L"Com_Dungeon"},
	};

	return S_OK;
}

HRESULT CTool_Object_Manager::Add_CloneObject(const char* pName, OBJECTTYPE eType, wstring strGameObjectTag, wstring strLayerTag, _vector vObjPos, _uint iObjectIndex)
{
	_float3 vObjPosFloat3;
	XMStoreFloat3(&vObjPosFloat3, vObjPos);

	if (eType == OBJECT_MONSTER)
	{
		CTool_Object::OBJECT_DESC tDesc{};
		strcpy_s(tDesc.pObjectName, pName);
		tDesc.vPosition = _float4(vObjPosFloat3.x, vObjPosFloat3.y, vObjPosFloat3.z, 1.f);
		tDesc.strPrototypeVIBufferCom = m_CloneDesc[OBJECT_MONSTER][iObjectIndex].strPrototypeVIBufferCom;
		tDesc.strComVIBufferCOm = m_CloneDesc[OBJECT_MONSTER][iObjectIndex].strComVIBufferCom;

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_MAIN, strGameObjectTag, strLayerTag, &tDesc)))
			return E_FAIL;
	}
	else if (eType == OBJECT_DUNGEON)
	{
		CTool_Dungeon::TOOL_DUNGEON_DESC tDesc{};
		strcpy_s(tDesc.pName, pName);
		tDesc.strPrototypeVIBufferCom = m_CloneDesc[OBJECT_DUNGEON][iObjectIndex].strPrototypeVIBufferCom;
		tDesc.strComVIBufferCom = m_CloneDesc[OBJECT_DUNGEON][iObjectIndex].strComVIBufferCom;

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_MAIN, strGameObjectTag, strLayerTag, &tDesc)))
			return E_FAIL;
	}

    return S_OK;
}

void CTool_Object_Manager::Free()
{
	for (auto& iter : m_Objects)
		Safe_Release(iter);

    Safe_Release(m_pGameInstance);
	
}
