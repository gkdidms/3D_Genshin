#pragma once
#include "Base.h"
#include "Client_Defines.h"

#include "Command.h"

BEGIN(Client)
class CCommandManager :
    public CBase
{
DECLARE_SINGLETON(CCommandManager)

private:
    CCommandManager();
    virtual ~CCommandManager() = default;

public:
    void Initialize();
    void Tick();
    
private:
    vector<CCommand*> m_Cmms;
    _uint m_iCmmIndex = { 0 };

public:
    virtual void Free() override;

};
END
