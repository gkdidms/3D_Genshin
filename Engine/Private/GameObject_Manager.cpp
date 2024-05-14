#include "GameObject_Manager.h"

#include "GameObject.h"
#include "Layer.h"

CGameObject_Manager::CGameObject_Manager()
{
}

HRESULT CGameObject_Manager::Initialize(_uint iMaxLevel)
{
	m_iMaxLevel = iMaxLevel;

	m_pLayers = new map<const wstring, CLayer*>[iMaxLevel];

	return S_OK;
}

void CGameObject_Manager::Priority_Tick(const _float& fTimeDelta)
{
	for (int i = 0; i < m_iMaxLevel; ++i)
	{
		for (auto Pair : m_pLayers[i])
		{
			Pair.second->Priority_Tick(fTimeDelta);
		}
	}
}

void CGameObject_Manager::Tick(const _float& fTimeDelta)
{
	for (int i = 0; i < m_iMaxLevel; ++i)
	{
		for (auto Pair : m_pLayers[i])
			Pair.second->Tick(fTimeDelta);
	}
}

void CGameObject_Manager::Late_Tick(const _float& fTimeDelta)
{
	for (int i = 0; i < m_iMaxLevel; ++i)
	{
		for (auto Pair : m_pLayers[i])
			Pair.second->Late_Tick(fTimeDelta);
	}
}

void CGameObject_Manager::Render()
{
	for (int i = 0; i < m_iMaxLevel; ++i)
	{
		for (auto Pair : m_pLayers[i])
			Pair.second->Render();
	}
}

HRESULT CGameObject_Manager::Add_GameObject_Prototype(const wstring strGameObjectTag, CGameObject* pGameObject)
{
	if (nullptr != Find_GameObject_Prototype(strGameObjectTag))
		return E_FAIL;

	m_pPrototypes.emplace(strGameObjectTag, pGameObject);

	return S_OK;
}

HRESULT CGameObject_Manager::Add_GameObject(_uint iLevelIndex, const wstring strGameObjectTag, const wstring strLayerTag, void* pArg)
{
	CGameObject* pPrototype = Find_GameObject_Prototype(strGameObjectTag);
	if (nullptr == pPrototype)
		return E_FAIL;

	CGameObject* pCloneObject = pPrototype->Clone(pArg);
	if (nullptr == pCloneObject)
		return E_FAIL;

	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);
	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();
		if (nullptr == pLayer)
			return E_FAIL;

		pLayer->Add_GameObject(pCloneObject);
		m_pLayers[iLevelIndex].emplace(strLayerTag, pLayer);
	}
	else
		pLayer->Add_GameObject(pCloneObject);
	
	return S_OK;
}

CGameObject* CGameObject_Manager::Clone_Object(const wstring strGameObjectTag, void* pArg)
{
	CGameObject* pPrototype = Find_GameObject_Prototype(strGameObjectTag);
	if (nullptr == pPrototype)
		return nullptr;

	CGameObject* pCloneObject = pPrototype->Clone(pArg);
	if (nullptr == pCloneObject)
		return nullptr;

	return pCloneObject;
}

void CGameObject_Manager::Clear(_uint iLevelIndex)
{
	if (m_iMaxLevel <= iLevelIndex)
		return;

	for (auto& Pair : m_pLayers[iLevelIndex])
	{
		Safe_Release(Pair.second);
	}
		

	m_pLayers[iLevelIndex].clear();
}

CComponent* CGameObject_Manager::Get_GameObject_Component(_uint iLevelIndex, const wstring strLayerTag, const wstring strComponentTag, _uint iIndex)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_Component(strComponentTag, iIndex);
}

CGameObject* CGameObject_Manager::Get_GameObject(_uint iLevelIndex, const wstring strLayerTag, _uint iIndex)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_Objecte(iIndex);
}

vector<CGameObject*> CGameObject_Manager::Get_GameObjects(_uint iLevelIndex, const wstring strLayerTag)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

	if (nullptr == pLayer)
		return vector<CGameObject*>();

	return pLayer->Get_Objects();
}

CGameObject* CGameObject_Manager::Find_GameObject_Prototype(const wstring strGameObjectTag)
{
	auto pPrototype = m_pPrototypes.find(strGameObjectTag);

	if (m_pPrototypes.end() == pPrototype)
		return nullptr;

	return pPrototype->second;
}

CLayer* CGameObject_Manager::Find_Layer(_uint iLevelIndex, const wstring strLayerTag)
{
	auto pLayer = m_pLayers[iLevelIndex].find(strLayerTag);
	if (m_pLayers[iLevelIndex].end() == pLayer)
		return nullptr;

	return pLayer->second;
}

CGameObject_Manager* CGameObject_Manager::Create(_uint iMaxLevel)
{
	CGameObject_Manager* pInstance = new CGameObject_Manager();

	if (FAILED(pInstance->Initialize(iMaxLevel)))
		Safe_Release(pInstance);

	return pInstance;
}

void CGameObject_Manager::Free()
{
	for (auto& Pair : m_pPrototypes)
		Safe_Release(Pair.second);
	m_pPrototypes.clear();

	for (int i = 0; i < m_iMaxLevel; ++i)
	{
		for (auto& Pair : m_pLayers[i])
			Safe_Release(Pair.second);

		m_pLayers[i].clear();
	}
}
