#include "StateElementalArt.h"

#include "StateElementalArt_Nilou.h"
#include "StateElementalArt_Tighanri.h"
#include "StateElementalArt_Wanderer.h"
#include "StateElementalArt_Yae.h"

#include "StateManager.h"


CStateElementalArt::CStateElementalArt()
{
}

HRESULT CStateElementalArt::Initialize()
{
	m_ElemenetalArt.emplace_back(CStateElementalArt_Nilou::Create());
	m_ElemenetalArt.emplace_back(CStateElementalArt_Tighanri::Create());
	m_ElemenetalArt.emplace_back(CStateElementalArt_Wanderer::Create());
	m_ElemenetalArt.emplace_back(CStateElementalArt_Yae::Create());

	return S_OK;
}

PLAYER_STATE CStateElementalArt::Enter(CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	return m_ElemenetalArt[pStateManager.Get_Playerble()]->Enter(pStateManager, CurrentState);
}

PLAYER_STATE CStateElementalArt::Update(const _float& fTimeDelta, CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	return m_ElemenetalArt[pStateManager.Get_Playerble()]->Update(fTimeDelta, pStateManager, CurrentState);
}

PLAYER_STATE CStateElementalArt::Exit(CStateManager& pStateManager, PLAYER_STATE CurrentState)
{
	return m_ElemenetalArt[pStateManager.Get_Playerble()]->Exit(pStateManager, CurrentState);
}

CStateElementalArt* CStateElementalArt::Create()
{
	CStateElementalArt* pInstance = new CStateElementalArt();

	if (FAILED(pInstance->Initialize()))
		Safe_Release(pInstance);

	return pInstance;
}

void CStateElementalArt::Free()
{
	for (auto& pElementalArt : m_ElemenetalArt)
		Safe_Release(pElementalArt);
	m_ElemenetalArt.clear();

	__super::Free();
}
