#include "Tool_Object_Manager.h"

#include "GameInstance.h"

#include "Tool_Object.h"

IMPLEMENT_SINGLETON(CTool_Object_Manager)
CTool_Object_Manager::CTool_Object_Manager()
    : m_pGameInstance{ CGameInstance::GetInstance() }
{
    Safe_AddRef(m_pGameInstance);
}

HRESULT CTool_Object_Manager::Add_CloneObject(const char* pName, wstring strGameObjectTag, wstring strLayerTag, _vector vObjPos)
{
	_float3 vObjPosFloat3;
	XMStoreFloat3(&vObjPosFloat3, vObjPos);

	CTool_Object::OBJECT_DESC tDesc{};
	strcpy_s(tDesc.pObjectName, pName);
	tDesc.vPosition = _float4(vObjPosFloat3.x, vObjPosFloat3.y, vObjPosFloat3.z, 1.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_MAIN, strGameObjectTag, strLayerTag, &tDesc)))
		return E_FAIL;

    return S_OK;
}

void CTool_Object_Manager::Free()
{
    Safe_Release(m_pGameInstance);
}
