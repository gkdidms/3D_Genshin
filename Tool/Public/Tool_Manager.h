#pragma once
#include "Base.h"

#include "Tool_Defines.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "ImGuizmo.h"
#include "ImSequencer.h"

BEGIN(Engine)
class CGameObject;
class CGameInstance;
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

public:
    string Get_SaveSeletedFileName() { return m_FileName[m_iSaveFileIndex]; }
    _uint Get_CreateObjectIndex() { return m_iCreateObjectIndex; }
    _float Get_DungeonDegree() { return m_fDungeonDegree; }
    _float3 Get_DungeonPos() { return m_fDungeonPos; }
    _bool Is_ShowTerrain() { return IsShowTerrain; }
    _bool Is_PickingWithDungeon() { return IsPickingWithDungeon; }
    _bool Is_PickingCell() { return m_isNavigationPicking; }
    _bool Is_PickingPlayer() { return m_isPlayerPosPicking; }
    _int Get_CellOption() { return m_iNavigationOption; }

public:
    void Set_DungeonPos(_vector vPos) { XMStoreFloat3(&m_fDungeonPos, vPos); }

public:
    HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    void Tick(const _float& fTimeDelta);
    void Render();

private:
    void Window_Terrain(); // 터레인 관련된 창
    void Window_Object(); // 오브젝트 관련된 창
    void Window_MainBar(); // 상태창 관련된 창
    void Window_Navigation(); // 내비게이션 관련된 창

private:
    void Modal_Save();
    void Modal_Load();

    void Modal_NavigationSave();
    void Modal_NavigationLoad();

private:
    void Bind_FileName();
    void Bind_NavigationFileName();

private:
    void Guizmo_Test();

private:
    ID3D11Device* m_pDevice = { nullptr };
    ID3D11DeviceContext* m_pContext = { nullptr };
    CGameInstance* m_pGameInstance = { nullptr };
    class CTool_Object_Manager* m_pObject_Manager = { nullptr };

private:
    char m_szNewFileName[MAX_PATH];
    char m_szNavigationFileName[MAX_PATH];
    _bool IsNewFile = { false };

    _bool IsShowSaveModal = { false };
    _bool IsShowLoadModal = { false };
    _bool IsShowNavigationSaveModal = { false };
    _bool IsShowNavigationLoadModal = { false };
    _bool IsShowTerrain = { false };

    _bool IsPickingWithDungeon = { false };

private: // Navigation
    _bool m_isNavigationPicking = { false };
    _int m_iNavigationOption = { 0 };

private:
    _bool m_isPlayerPosPicking = { false };

private:
    _int m_iCheckRadioButton = { 0 };

private:
    vector<string> m_FileName;
    vector<string> m_NavigationFileName;

    _int m_iSaveFileIndex = { 0 };
    _int m_iLoadFileIndex = { 0 };
    _int m_iSaveNavigationFileIndex = { 0 };
    _int m_iLoadNavigationFileIndex = { 0 };

private:
    _int m_iCurrentPickingObjectIndex = { -1 };
    _float m_fDungeonDegree = { 0.f };
    _float3 m_fDungeonPos = {};

private:
    _int m_iCreateObjectIndex = { 0 };
    _int m_iCreateDungeonIndex = { 0 };

private:
    _float m_pObjectMatrix[16];

    ImGuizmo::OPERATION mCurrentGizmoOperation = { ImGuizmo::TRANSLATE };
    ImGuizmo::MODE mCurrentGizmoMode = {ImGuizmo::WORLD};
    float m_cameraView[16] =
    { 1.f, 0.f, 0.f, 0.f,
      0.f, 1.f, 0.f, 0.f,
      0.f, 0.f, 1.f, 0.f,
      0.f, 0.f, 0.f, 1.f };

    float camDistance = 8.f;

     float identityMatrix[16] = { 1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f };

     int gizmoCount = 1;

public:
    void Release_Tool();
    virtual void Free(); 
};

END