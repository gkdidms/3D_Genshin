#include "Input_Manager.h"

CInput_Manager::CInput_Manager()
{
}

void CInput_Manager::Get_ProcessMessageKeyboard(UINT message, WPARAM wParam, LPARAM lParam)
{
	Keyboard::ProcessMessage(message, wParam, lParam);
}

void CInput_Manager::Get_ProcessMessageMouse(UINT message, WPARAM wParam, LPARAM lParam)
{
	Mouse::ProcessMessage(message, wParam, lParam);
}

HRESULT CInput_Manager::Initialize(HWND hWnd)
{
	m_Keyboard = std::make_unique<Keyboard>();
	
	m_Mouse = std::make_unique<Mouse>();
	m_Mouse->SetWindow(hWnd);

	m_KeyStateTracker.Reset();
	m_MouseStateTracker.Reset();

	return S_OK;
}

void CInput_Manager::Tick()
{
	m_eKeys = m_Keyboard->GetState();
	m_KeyStateTracker.Update(m_eKeys);


	m_eMouse = m_Mouse->GetState();
	m_MouseStateTracker.Update(m_eMouse);

	m_vMousePosition.x = m_eMouse.x;
	m_vMousePosition.y = m_eMouse.y;
	if (m_MouseStateTracker.leftButton == Mouse::ButtonStateTracker::PRESSED)
		m_bMouseLeft = m_eMouse.leftButton;
}

CInput_Manager* CInput_Manager::Create(HWND hWnd)
{
	CInput_Manager* pInstance = new CInput_Manager();

	if (FAILED(pInstance->Initialize(hWnd)))
		Safe_Release(pInstance);

	return pInstance;
}

void CInput_Manager::Free()
{
}
