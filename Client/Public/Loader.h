#pragma once
#include "Base.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)
class CLoader :
    public CBase
{
private:
    CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CLoader() = default;

public:
    const _tchar* Get_LoadingText() const {
        return m_szLoadingText;
    }

    _bool is_Finished() const {
        return m_isFinished;
    }

public:
    HRESULT Initialize(LEVEL_STATE eNextLevel);
    HRESULT Loading();

private:
    ID3D11Device* m_pDevice = { nullptr };
    ID3D11DeviceContext* m_pContext = { nullptr };
    CGameInstance* m_pGameInstance = { nullptr };

    HANDLE m_hThread = { };
    LEVEL_STATE m_eNextLevel = { LEVEL_END };
    CRITICAL_SECTION m_Critical_Section = { 0 };

    _tchar					m_szLoadingText[MAX_PATH] = { TEXT("") };
    _bool m_isFinished;

private:
    HRESULT Loading_For_Logo();
    HRESULT Loading_For_GamePlay();
    HRESULT Open_File(LEVEL_STATE eNextLevel);
    HRESULT Ready_Object(const char* pObjectName, const wstring& strLayerTag);

public:
    static CLoader* Create(LEVEL_STATE eNextLevel, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual void Free();
};
END
