#include "Layer.h"

#include "GameObject.h"

CLayer::CLayer()
{
}

HRESULT CLayer::Initialize()
{
	return S_OK;
}

void CLayer::Priority_Tick(const _float& fTimeDelta)
{
	for (auto& iter : m_vecObjects)
		iter->Priority_Tick(fTimeDelta);
}

void CLayer::Tick(const _float& fTimeDelta)
{
	for (auto iter = m_vecObjects.begin(); iter < m_vecObjects.end();)
	{
		(*iter)->Tick(fTimeDelta);

		if ((*iter)->Get_Dead())
		{
			Safe_Release(*iter);
			iter = m_vecObjects.erase(iter);
		}
		else ++iter;
	}
}

void CLayer::Late_Tick(const _float& fTimeDelta)
{
	for (auto& iter : m_vecObjects)
		iter->Late_Tick(fTimeDelta);
}

void CLayer::Render()
{
	for (auto& iter : m_vecObjects)
		iter->Render();
}

HRESULT CLayer::Add_GameObject(CGameObject* pObject)
{
	if (nullptr == pObject)
		return E_FAIL;

	m_vecObjects.push_back(pObject);

	return S_OK;
}

CComponent* CLayer::Get_Component(wstring strComponentTag, _uint iIndex)
{
	if (m_vecObjects.size() <= iIndex)
		return nullptr;

	return m_vecObjects[iIndex]->Get_Component(strComponentTag);
}

CGameObject* CLayer::Get_Objecte(_uint iIndex)
{
	return m_vecObjects[iIndex];
}

CLayer* CLayer::Create()
{
	CLayer* pInstance = new CLayer();

	if (FAILED(pInstance->Initialize()))
		Safe_Release(pInstance);

	return pInstance;
}

void CLayer::Free()
{
	for (auto& iter : m_vecObjects)
		Safe_Release(iter);
	m_vecObjects.clear();
}
