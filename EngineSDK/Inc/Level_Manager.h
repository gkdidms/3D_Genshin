#pragma once
#include "Base.h"

BEGIN(Engine)
class CLevel_Manager :
    public CBase
{
private:
    CLevel_Manager();
    virtual ~CLevel_Manager() = default;

public:
    HRESULT Initialize(_uint iMaxLevelIndex);
    void Tick(const _float& fTimeDelta);
    void Render();

    HRESULT Open_Level(_uint iLevelIndex, class CLevel* pLevel);

private:
    class CGameInstance* m_pGameInstance = { nullptr };
    class CLevel* m_pCurLevel = { nullptr };

    _uint m_iMaxLevelIndex = { 0 };
    _uint m_iLevelIndex = { 0 };
    _uint m_iPreLevelIndex = { 0 };

public:
    static CLevel_Manager* Create(_uint iMaxLevelIndex);
    virtual void Free() override;
};

END