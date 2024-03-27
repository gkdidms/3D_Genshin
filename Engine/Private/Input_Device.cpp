
#include "..\Public\Input_Device.h"



Engine::CInput_Device::CInput_Device(void)
{

}

HRESULT Engine::CInput_Device::Ready_InputDev(HINSTANCE hInst, HWND hWnd)
{

	// DInput 컴객체를 생성하는 함수
	if (FAILED(DirectInput8Create(hInst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pInputSDK,
		NULL)))
		return E_FAIL;

	// 키보드 객체 생성
	if (FAILED((m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr))))
		return E_FAIL;

	// 생성된 키보드 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// 장치에 대한 독점권을 설정해주는 함수, (클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수)
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pKeyBoard->Acquire();


	// 마우스 객체 생성
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	// 생성된 마우스 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// 장치에 대한 독점권을 설정해주는 함수, 클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pMouse->Acquire();

	m_vecKey.reserve(DIK_END);

	for (int i = 0; i < 256; ++i)
	{
		m_vecKey.push_back(tKeyInfo{ eKEY_STATE::NONE,false });
	}

	for (int i = 0; i < DIM_END; ++i)
		m_vecMouseState.push_back(eKEY_STATE::NONE);

	return S_OK;
}

void Engine::CInput_Device::Update_InputDev(void)
{
	m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);

	for (size_t i = 0; i < 256; i++)
	{
		if (m_byKeyState[i] & 0x80)
		{
			if (m_vecKey[i].IsPrePush)
			{
				//이전에도 눌려있었으면
				m_vecKey[i].eState = eKEY_STATE::HOLD;
			}
			else
			{
				m_vecKey[i].eState = eKEY_STATE::TAP;
			}
			//현재프레임 눌려있었으니 true
			m_vecKey[i].IsPrePush = true;
		}
		else// 키가 안눌려있었을때
		{
			if (m_vecKey[i].IsPrePush)
			{
				// 이전에 눌려있었다.
				m_vecKey[i].eState = eKEY_STATE::AWAY;
			}
			else
			{
				m_vecKey[i].eState = eKEY_STATE::NONE;
			}
			m_vecKey[i].IsPrePush = false;
		}
	}

	for (int i = 0; i < DIM_END; ++i)
	{
		if (m_tMouseState.rgbButtons[i] & 0x80)
		{
			if (m_vecMouseState[i] == eKEY_STATE::NONE)
				m_vecMouseState[i] = eKEY_STATE::TAP;
			else
				m_vecMouseState[i] = eKEY_STATE::HOLD;
		}
		else
		{
			if (m_vecMouseState[i] == eKEY_STATE::HOLD || m_vecMouseState[i] == eKEY_STATE::TAP)
				m_vecMouseState[i] = eKEY_STATE::AWAY;
			else
				m_vecMouseState[i] = eKEY_STATE::NONE;
		}
	}
}

CInput_Device * CInput_Device::Create(HINSTANCE hInst, HWND hWnd)
{
	CInput_Device*		pInstance = new CInput_Device();

	if (FAILED(pInstance->Ready_InputDev(hInst, hWnd)))
	{
		MSG_BOX("Failed to Created : CInput_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CInput_Device::Free(void)
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pInputSDK);
}

