#include "Loading_Level.h"
#include "GameInstance.h"

#include "Loader.h"
#include "Logo_Level.h"
#include "GamePlay_Level.h"
#include "Stage_Boss_Level.h"

#include "Background.h"

CLoading_Level::CLoading_Level(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
{
}

HRESULT CLoading_Level::Initialize(LEVEL_STATE eNextLevel)
{
	m_eNextLevel = eNextLevel;
	m_pLoader = CLoader::Create(eNextLevel, m_pDevice, m_pContext);
	if (nullptr == m_pLoader)
		return E_FAIL;

	if (FAILED(Ready_Background(L"Level_Loading")))
		return E_FAIL;

	return S_OK;
}

void CLoading_Level::Tick(const _float& fTimeDelta)
{
	SetWindowText(g_hWnd, m_pLoader->Get_LoadingText());

	if (m_pLoader->is_Finished())
	{
		SetWindowText(g_hWnd, m_pLoader->Get_LoadingText());

		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			if (m_eNextLevel == LEVEL_LOGO)
				m_pGameInstance->Open_Level(LEVEL_LOGO, CLogo_Level::Create(m_pDevice, m_pContext));
			else if (m_eNextLevel == LEVEL_GAMEPLAY)
				m_pGameInstance->Open_Level(LEVEL_GAMEPLAY, CGamePlay_Level::Create(m_pDevice, m_pContext));
			else if (m_eNextLevel == LEVEL_STAGE_BOSS)
				m_pGameInstance->Open_Level(LEVEL_GAMEPLAY, CStage_Boss_Level::Create(m_pDevice, m_pContext));
		}
	}
}

void CLoading_Level::Render()
{
}

HRESULT CLoading_Level::Ready_Background(const wstring strLayerTag)
{
	CBackground::BACKGROUND_DESC Desc{};
	Desc.pProgress = m_pLoader->Get_Progress();

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_LOADING, L"Prototype_GameObject_Background", strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

CLoading_Level* CLoading_Level::Create(LEVEL_STATE eNextLevel, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLoading_Level* pInstance = new CLoading_Level(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel)))
		Safe_Release(pInstance);

	return pInstance;
}

void CLoading_Level::Free()
{
	Safe_Release(m_pLoader);

	__super::Free();
}
