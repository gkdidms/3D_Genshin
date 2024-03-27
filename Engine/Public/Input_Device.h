#ifndef InputDev_h__
#define InputDev_h__

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CInput_Device : public CBase
{
public:
	enum eKEY_STATE
	{
		NONE,
		TAP, //누를??
		HOLD,//누르고 있을때
		AWAY, // ?H을때
		KEY_END,
	};

	struct tKeyInfo
	{
		eKEY_STATE	eState; //키의 상태값
		bool		IsPrePush;	//이전프레임에 눌렀는지 안눌렀는지
	};

private:
	CInput_Device(void);
	virtual ~CInput_Device(void) = default;
	
public:
	_byte	Get_DIKeyState(_ubyte byKeyID)			{ return m_byKeyState[byKeyID]; }
	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse) 	
	{ 	return m_tMouseState.rgbButtons[eMouse]; 	}

	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)	
	{	
		return *(((_long*)&m_tMouseState) + eMouseState);	
	}

	eKEY_STATE GetKeyState(_ubyte byKeyID) { return m_vecKey[byKeyID].eState; }
	eKEY_STATE GetMouseState(MOUSEKEYSTATE eMouseState) { return m_vecMouseState[eMouseState]; }
	
public:
	HRESULT Ready_InputDev(HINSTANCE hInst, HWND hWnd);
	void	Update_InputDev(void);

private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;

private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse	= nullptr;

private:
	_byte					m_byKeyState[256];		// 키보드에 있는 모든 키값을 저장하기 위한 변수
	DIMOUSESTATE			m_tMouseState;	

	vector<eKEY_STATE> m_vecMouseState;
	vector<tKeyInfo>		m_vecKey;

public:
	static CInput_Device* Create(HINSTANCE hInst, HWND hWnd);
	virtual void	Free(void);

};
END
#endif // InputDev_h__
