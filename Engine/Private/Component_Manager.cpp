#include "Component_Manager.h"

CComponent_Manager::CComponent_Manager()
{
}

HRESULT CComponent_Manager::Initialize(_uint iMaxLevelIndex)
{
    m_iMaxLevelIndex = iMaxLevelIndex;

    m_Prototypes = new map<const wstring, CComponent*>[iMaxLevelIndex];

    return S_OK;
}

HRESULT CComponent_Manager::Add_Component_Prototype(_uint iLevelIndex, const wstring strComponentTag, CComponent* pComponent)
{
    if (nullptr != Find_Component_Prototype(iLevelIndex, strComponentTag))
        return E_FAIL;

    m_Prototypes[iLevelIndex].emplace(strComponentTag, pComponent);

    return S_OK;
}

CComponent* CComponent_Manager::Add_Component_Clone(_uint iLevelIndex, const wstring strComponentTag, void* pArg)
{
    CComponent* pPrototypeComponent = Find_Component_Prototype(iLevelIndex, strComponentTag);
    if (nullptr == pPrototypeComponent)
        return nullptr;

    CComponent* pCloneComponent = pPrototypeComponent->Clone(pArg);
    if (nullptr == pCloneComponent)
        return nullptr;

    return pCloneComponent;
}

void CComponent_Manager::Clear(_uint iLevelIndex)
{
    if (m_iMaxLevelIndex <= iLevelIndex)
        return;

    for (auto& pComponents : m_Prototypes[iLevelIndex])
        Safe_Release(pComponents.second);

    m_Prototypes[iLevelIndex].clear();
}

CComponent* CComponent_Manager::Find_Component_Prototype(_uint iLevelIndex, const wstring strComponentTag)
{
    auto Pair = m_Prototypes[iLevelIndex].find(strComponentTag);

    if (m_Prototypes[iLevelIndex].end() == Pair)
        return nullptr;

    return Pair->second;
}

CComponent_Manager* CComponent_Manager::Create(_uint iMaxLevelIndex)
{
    CComponent_Manager* pInstance = new CComponent_Manager();

    if (FAILED(pInstance->Initialize(iMaxLevelIndex)))
        Safe_Release(pInstance);

    return pInstance;
}

void CComponent_Manager::Free()
{
    for (int i = 0; i < m_iMaxLevelIndex; ++i)
    {
        for (auto& Pair : m_Prototypes[i])
        {
            Safe_Release(Pair.second);
        }
        m_Prototypes[i].clear();
    }
}
