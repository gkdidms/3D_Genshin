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
public:
    enum OBJECTTYPE { OBJECT_MONSTER, OBJECT_DUNGEON, OBJECT_END };
    typedef struct tCloneDesc {
        string strName = { "" };
        wstring strPrototypeVIBufferCom{ L"" };
        wstring strComVIBufferCom{ L"" };

    }CLONE_DESC;

private:
    CTool_Object_Manager();
    virtual ~CTool_Object_Manager() = default;

public:
    const vector<CLONE_DESC> Get_CloneDescs(OBJECTTYPE eType) const { return m_CloneDesc[eType]; }

public:
    HRESULT Initialize();
    HRESULT Add_CloneObject(const char* pName, OBJECTTYPE eType, wstring strGameObjectTag, wstring strLayerTag, _vector vObjPos, _uint iObjectIndex);

private:
    CGameInstance* m_pGameInstance = { nullptr };
    vector<CLONE_DESC> m_CloneDesc[OBJECT_END];
    
public:
    virtual void Free();
};
END
