#include "..\Public\Timer_Manager.h"

CTimer_Manager::CTimer_Manager()
{

}

_float CTimer_Manager::Get_TimeDelta(const _tchar * pTimerTag) 
{
	CTimer* pTimer = Find_Timer(pTimerTag);
	if (nullptr == pTimer)
		return 0.f;

	return pTimer->Get_TimeDelta();
}

void CTimer_Manager::Update_TimeDelta(const _tchar * pTimerTag)
{
	CTimer* pTimer = Find_Timer(pTimerTag);
	if (nullptr == pTimer)
		return;

	pTimer->Update_TimeDelta();
}

HRESULT CTimer_Manager::Ready_Timer(const _tchar* pTimerTag)
{
	CTimer*		pTimer = Find_Timer(pTimerTag);

	if (nullptr != pTimer)
		return E_FAIL;

	pTimer = CTimer::Create();
	if (nullptr == pTimer)
		return E_FAIL;

	m_mapTimers.insert({pTimerTag, pTimer});

	return S_OK;
}

CTimer * CTimer_Manager::Find_Timer(const _tchar * pTimerTag)
{
	auto		iter = find_if(m_mapTimers.begin(), m_mapTimers.end(), [&](map<const _tchar*, CTimer*>::value_type Pair)->_bool
	{
		return !lstrcmp(Pair.first, pTimerTag);
	});

	if (iter == m_mapTimers.end())
		return nullptr;
	
	return iter->second;
}

CTimer_Manager * CTimer_Manager::Create()
{
	return new CTimer_Manager();	
}

void CTimer_Manager::Free(void)
{
	for (auto& Pair : m_mapTimers)
		Safe_Release(Pair.second);
	m_mapTimers.clear();
}
