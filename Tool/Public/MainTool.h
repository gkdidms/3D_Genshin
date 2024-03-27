#pragma once
#include "Tool_Manager.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Tool)
class CMainTool : public CBase
{
private:
	CMainTool();
	virtual ~CMainTool() = default;

public:
	HRESULT Initialize();
	void Tick(const _float& fTimeDelta);
	void Render();

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	CGameInstance* m_pGameInstance = { nullptr };
	CTool_Manager* m_pTool_Manager = { nullptr };

private:
	HRESULT Open_Level(LEVEL_STATE eLevelState);
	HRESULT Ready_Level_For_Main();

public:
	static CMainTool* Create();
	virtual void Free();
};
END
