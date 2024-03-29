#pragma once
#include "Base.h"

#include "Tool_Defines.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Tool)
class CTool_Manager :
    public CBase
{
    DECLARE_SINGLETON(CTool_Manager)

private:
    CTool_Manager();
    virtual ~CTool_Manager() = default;

public:
    static _float3 vWorldMousePos;
    static _float fScaleTerrainX;
    static _float fScaleTerrainZ;

    static _bool mTerrainPicking;

    static vector<CGameObject*> Objects;

public:
    string Get_SaveSeletedFileName() { return m_FileName[m_iSaveFileIndex]; }
    _uint Get_CreateObjectIndex() { return m_iCreateObjectIndex; }
    _float Get_DungeonDegree() { return m_fDungeonDegree; }
    _bool Is_ShowTerrain() { return IsShowTerrain; }
    _bool Is_PickingWithDungeon() { return IsPickingWithDungeon; }

public:
    HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    void Tick(const _float& fTimeDelta);
    void Render();

private:
    void Window_Terrain();
    void Window_Object();
    void Window_MainBar();

    void Modal_Save();
    void Modal_Load();

    void Bind_FileName();

private:
    ID3D11Device* m_pDevice = { nullptr };
    ID3D11DeviceContext* m_pContext = { nullptr };
    class CTool_Object_Manager* m_pObject_Manager = { nullptr };

private:
    char m_szNewFileName[MAX_PATH];
    _bool IsNewFile = { false };

    _bool IsShowSaveModal = { false };
    _bool IsShowLoadModal = { false };
    _bool IsShowTerrain = { false };

    _bool IsPickingWithDungeon = { false };

private:
    vector<string> m_FileName;
    _int m_iSaveFileIndex = { 0 };
    _int m_iLoadFileIndex = { 0 };

private:
    _int m_iCurrentPickingObjectIndex = { 0 };
    _float m_fDungeonDegree = { 0.f };

private:
    _int m_iCreateObjectIndex = { 0 };
    _int m_iCreateDungeonIndex = { 0 };

public:
    void Release_Tool();
    virtual void Free(); 
};

END