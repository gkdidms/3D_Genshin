#include "State.h"

#include "GameInstance.h"

CState::CState()
	: m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

void CState::Free()
{
	Safe_Release(m_pGameInstance);
}
