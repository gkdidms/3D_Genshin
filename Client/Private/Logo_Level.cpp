#include "Logo_Level.h"

#include "GameInstance.h"
#include "Loading_Level.h"

CLogo_Level::CLogo_Level(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CLevel{ pDevice, pContext }
{
}

HRESULT CLogo_Level::Initialize()
{
    return S_OK;
}

void CLogo_Level::Tick(const _float& fTimeDelta)
{
    SetWindowText(g_hWnd, TEXT("Logo ÀÔ´Ï´Ù."));

    if (GetAsyncKeyState(VK_RETURN))
    {
        m_pGameInstance->Open_Level(LEVEL_LOADING, CLoading_Level::Create(LEVEL_STAGE_BOSS, m_pDevice, m_pContext));
    }
}

void CLogo_Level::Render()
{
}

CLogo_Level* CLogo_Level::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CLogo_Level* pInstance = new CLogo_Level(pDevice, pContext);

    if (FAILED(pInstance->Initialize()))
        Safe_Release(pInstance);

    return pInstance;
}

void CLogo_Level::Free()
{
    __super::Free();
}
