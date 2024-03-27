#pragma once
#include "Base.h"

BEGIN(Engine)
class CInput_Manager :
    public CBase
{
private:
    CInput_Manager();
    virtual ~CInput_Manager() = default;

public:
    _float4 Get_MousePos() { }
    _float Get_MouseX() { return m_vMousePosition.x; }
    _float Get_MouseY() { return m_vMousePosition.y; }

    _bool Is_KeyPressed(Keyboard::Keys eKeys) { return m_KeyStateTracker.IsKeyPressed(eKeys); }
    _bool Is_MouseLeftPressed() { return m_bMouseLeft; }

public:
    void Get_ProcessMessageKeyboard(UINT message, WPARAM wParam, LPARAM lParam);
    void Get_ProcessMessageMouse(UINT message, WPARAM wParam, LPARAM lParam);

public:
    HRESULT Initialize(HWND hWnd);
    void Tick();

private:
    std::unique_ptr<DirectX::Keyboard> m_Keyboard;
    std::unique_ptr<DirectX::Mouse> m_Mouse;
    DirectX::Keyboard::State m_eKeys;
    DirectX::Mouse::State m_eMouse;

    DirectX::Keyboard::KeyboardStateTracker m_KeyStateTracker;
    DirectX::Mouse::ButtonStateTracker m_MouseStateTracker;

private:
    _float2 m_vMousePosition;
    _bool m_bMouseLeft = { false };

public:
    static CInput_Manager* Create(HWND hWnd);
    virtual void Free() override;
};
END
