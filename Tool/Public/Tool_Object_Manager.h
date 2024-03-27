#pragma once
#include "Base.h"

#include "Tool_Defines.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Tool)
class CTool_Object_Manager :
    public CBase
{
    DECLARE_SINGLETON(CTool_Object_Manager)
private:
    CTool_Object_Manager();
    virtual ~CTool_Object_Manager() = default;

public:
    HRESULT Add_CloneObject(const char* pName, wstring strGameObjectTag, wstring strLayerTag, _vector vObjPos);

private:
    CGameInstance* m_pGameInstance = { nullptr };
    
public:
    virtual void Free();
};
END
