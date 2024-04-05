#pragma once

#include "Base.h"

BEGIN(Engine)

class CPicking :
    public CBase
{
private:
    CPicking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CPicking() = default;

public:
    HRESULT Initialize(HWND hWnd);
    _vector Picking(_bool* isSuccess);

private:
    ID3D11Device* m_pDevice = { nullptr };
    ID3D11DeviceContext* m_pContext = { nullptr };
    class CGameInstance* m_pGameInstance = { nullptr };

    HWND m_hWnd;

    _float m_fWinSizeX = { 0.f };
    _float m_fWinSizeY = { 0.f };

public:
    static CPicking* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd);
    virtual void Free() override;
};

END