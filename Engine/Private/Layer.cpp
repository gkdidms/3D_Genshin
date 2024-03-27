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
	for (auto& iter : m_vecObjects)
		iter->Tick(fTimeDelta);
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
