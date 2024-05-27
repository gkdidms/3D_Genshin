#pragma once
#include "Client_Defines.h"

#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)
class CMainApp : public CBase 
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT Initialize();
	void Tick(const _float& fTimeDelta);
	void Render();

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	CGameInstance* m_pGameInstance = { nullptr };

private:
	_tchar m_szFPS[MAX_PATH] = { L"" };
	_float	m_fTimeAcc = {};
	_uint	m_iNumRender = {};

private:
	HRESULT Open_Level();

private:
	HRESULT Ready_Prototype_Components();
	HRESULT Ready_Prototype_GameObjects();
	HRESULT Ready_Font();

public:
	static CMainApp* Create();
	virtual void Free() override;
};
END
