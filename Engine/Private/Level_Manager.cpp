#include "Level_Manager.h"

#include "GameInstance.h"
#include "Level.h"

CLevel_Manager::CLevel_Manager()
	: m_pGameInstance{CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CLevel_Manager::Initialize(_uint iMaxLevelIndex)
{
	m_iMaxLevelIndex = iMaxLevelIndex;

	return S_OK;
}

void CLevel_Manager::Tick(const _float& fTimeDelta)
{
	m_pCurLevel->Tick(fTimeDelta);
}

void CLevel_Manager::Render()
{
}

HRESULT CLevel_Manager::Open_Level(_uint iLevelIndex, class CLevel* pLevel)
{
	if (nullptr == pLevel)
		return E_FAIL;

	if (nullptr != m_pCurLevel)
		m_pGameInstance->Clear_Object(m_iPreLevelIndex);

	Safe_Release(m_pCurLevel);

	m_pCurLevel = pLevel;

	m_iLevelIndex = iLevelIndex;

	m_iPreLevelIndex = m_iLevelIndex;

	return S_OK;
}

CLevel_Manager* CLevel_Manager::Create(_uint iMaxLevelIndex)
{
	CLevel_Manager* pInstance = new CLevel_Manager();

	if (FAILED(pInstance->Initialize(iMaxLevelIndex)))
		Safe_Release(pInstance);

	return pInstance;
}

void CLevel_Manager::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pCurLevel);
}
