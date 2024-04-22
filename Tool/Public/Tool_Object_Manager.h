#pragma once
#include "Base.h"

#include "Tool_Defines.h"

BEGIN(Engine)
class CGameInstance;
class CGameObject;
END

BEGIN(Tool)
class CTool_Object_Manager :
    public CBase
{
    DECLARE_SINGLETON(CTool_Object_Manager)
public:
    enum OBJECTTYPE { OBJECT_MONSTER, OBJECT_DUNGEON, OBJECT_END };
    typedef struct tCloneDesc {
        _uint iIndex = { 0 };
        string strName = { "" };
        wstring strPrototypeVIBufferCom{ L"" };
        wstring strComVIBufferCom{ L"" };

        _bool IsAnimation;

    }CLONE_DESC;

private:
    CTool_Object_Manager();
    virtual ~CTool_Object_Manager() = default;

public:
    const vector<CLONE_DESC> Get_CloneDescs(OBJECTTYPE eType) const { return m_CloneDesc[eType]; }
    vector<class CTool_Object*> Get_Objects() { return m_Objects; }
    vector< CGameObject*> Get_Terrains() { return m_Terrains; }

public:
    HRESULT Initialize();
    void Priority_Tick(const _float& fTimeDelta);
    void Tick(const _float& fTimeDelta);
    void Late_Tick(const float& fTimeDelta);
    HRESULT Render();
    HRESULT Add_CloneObject(OBJECTTYPE eType, wstring strLayerTag, _vector vObjPos, _uint iObjectIndex);

public:
    HRESULT Save(const char* pFileName);
    HRESULT Load(const char* pFileName);
    
private:
    CGameInstance* m_pGameInstance = { nullptr };
    vector<class CTool_Object*> m_Objects;
    vector<CGameObject*> m_Terrains;
    vector<CLONE_DESC> m_CloneDesc[OBJECT_END];
    
public:
    void Release_Object();
    virtual void Free();
};
END
