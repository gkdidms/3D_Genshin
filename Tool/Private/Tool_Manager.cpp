#include "Tool_Manager.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "Tool_Object.h"
#include "Tool_Object_Manager.h"

IMPLEMENT_SINGLETON(CTool_Manager)


_float3 CTool_Manager::vWorldMousePos = { 0.f, 0.f, 0.f };
_float CTool_Manager::fScaleTerrainX = { 1.f };
_float CTool_Manager::fScaleTerrainZ = { 1.f };
_bool CTool_Manager::mTerrainPicking = { false };

CTool_Manager::CTool_Manager() 
    : m_pObject_Manager{ CTool_Object_Manager::GetInstance() },
    m_pGameInstance{ CGameInstance::GetInstance() }
{
    Safe_AddRef(m_pObject_Manager);
    Safe_AddRef(m_pGameInstance);
}

HRESULT CTool_Manager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(g_hWnd);
    ImGui_ImplDX11_Init(m_pDevice, m_pContext);


    //���丮 �޾Ƽ� ���� �����ϱ�
    Bind_FileName();

	return S_OK;
}

void CTool_Manager::Tick(const _float& fTimeDelta)
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();


    //_bool show_demo_window = true;
    //ImGui::ShowDemoWindow(&show_demo_window);

    Window_Terrain();
    Window_Object();
    Window_MainBar();
    Modal_Save();
    Modal_Load();

    Guizmo_Test();
}

void CTool_Manager::Render()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void CTool_Manager::Window_Terrain()
{
    ImGui::Begin("Terrain");

    ImGui::Checkbox("Terrain Render Off", &IsShowTerrain);
    ImGui::Text("Terrain Scale");
    ImGui::SliderFloat("Terrain Width", &fScaleTerrainX, 0.1f, 10.f);
    ImGui::SliderFloat("Terrain Height", &fScaleTerrainZ, 0.1f, 10.f);

    ImGui::NewLine();
    if (ImGui::BeginListBox("Dungeon"))
    {
        for (int n = 0; n < m_pObject_Manager->Get_CloneDescs(CTool_Object_Manager::OBJECT_DUNGEON).size(); n++)
        {
            const bool is_selected = (m_iCreateDungeonIndex == n);
            if (ImGui::Selectable(m_pObject_Manager->Get_CloneDescs(CTool_Object_Manager::OBJECT_DUNGEON)[n].strName.c_str(), is_selected))
            {
                m_iCreateDungeonIndex = n;
            }

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }

    if (ImGui::Button("Create"))
    {
        m_pObject_Manager->Add_CloneObject("Dungeon_1", 
            CTool_Object_Manager::OBJECT_DUNGEON, 
            L"Prototype_GameObject_Dungeon", 
            L"GameObject_Dungeon", 
            XMVectorSet(0.f, 0.f, 0.f, 1.f), 
            m_iCreateDungeonIndex);
    }

    ImGui::NewLine();

    ImGui::Text("Terrain Rotation");
    ImGui::SliderFloat("Rotaion", &m_fDungeonDegree, 0.f, 360.f);

    ImGui::End();
}

void CTool_Manager::Window_Object()
{
    ImGui::Begin("Object");

    ImGui::Checkbox("Terrain Picking", &mTerrainPicking);
    ImGui::Checkbox("Mesh Picking", &IsPickingWithDungeon);
    ImGui::NewLine();
    ImGui::Text("Picking MousePos : Terrain");

    ImGui::Text("MousePos : %f, %f, %f", vWorldMousePos.x, vWorldMousePos.y, vWorldMousePos.z);

    if (ImGui::BeginListBox("Selete Create Object"))
    {
        for (int n = 0; n < m_pObject_Manager->Get_CloneDescs(CTool_Object_Manager::OBJECT_MONSTER).size(); n++)
        {
            const bool is_selected = (m_iCreateObjectIndex == n);
            if (ImGui::Selectable(m_pObject_Manager->Get_CloneDescs(CTool_Object_Manager::OBJECT_MONSTER)[n].strName.c_str(), is_selected))
            {
                m_iCreateObjectIndex = n;
            }

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }

    ImGui::NewLine();

    ImGui::Text("Created Objects");
    if (ImGui::BeginListBox("Created Object List"))
    {
        vector<CGameObject*> Objects = m_pObject_Manager->Get_Objects();
        for (int n = 0; n < Objects.size(); n++)
        {
            const bool is_selected = (m_iCurrentPickingObjectIndex == n);
            if (ImGui::Selectable(dynamic_cast<CTool_Object*>(Objects[n])->Get_ObjectName().c_str(), is_selected))
            {
                m_iCurrentPickingObjectIndex = n;
            }

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }

    ImGui::End();
}

void CTool_Manager::Window_MainBar()
{
    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("File"))
    {
        ImGui::MenuItem("Open", nullptr, &IsShowLoadModal);// Load
        ImGui::MenuItem("Save", nullptr, &IsShowSaveModal);// Save

        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
}

void CTool_Manager::Modal_Save()
{
    if (IsShowSaveModal)
        ImGui::OpenPopup("SaveFile");
    if (ImGui::BeginPopupModal("SaveFile", NULL, ImGuiWindowFlags_MenuBar))
    {
        if (ImGui::BeginListBox("File Seleted"))
        {
            for (int n = 0; n < m_FileName.size(); n++)
            {
                const bool is_selected = (m_iSaveFileIndex == n);
                if (ImGui::Selectable(m_FileName[n].c_str(), is_selected))
                {
                    m_iSaveFileIndex = n;
                }

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }
        ImGui::Checkbox("Create New File ? ", &IsNewFile);
        ImGui::InputText("New File", m_szNewFileName, MAX_PATH);

        if (ImGui::Button("Save"))
        {
            // �����ϴ� �Լ� ȣ�� & �Һ��� ȣ��
            ImGui::CloseCurrentPopup();            
            IsShowSaveModal = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Close"))
        {
            ImGui::CloseCurrentPopup();
            IsShowSaveModal = false;
        }

        ImGui::EndPopup();
    }
}

void CTool_Manager::Modal_Load()
{
    if (IsShowLoadModal)
        ImGui::OpenPopup("LoadFile");
    if (ImGui::BeginPopupModal("LoadFile", NULL, ImGuiWindowFlags_MenuBar))
    {
        if (ImGui::BeginListBox("File Seleted"))
        {
            for (int n = 0; n < m_FileName.size(); n++)
            {
                const bool is_selected = (m_iLoadFileIndex == n);
                if (ImGui::Selectable(m_FileName[n].c_str(), is_selected))
                {
                    m_iLoadFileIndex = n;
                }

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }

        if (ImGui::Button("Load"))
        {
            // �ҷ����� �Լ� ȣ�� & �Һ��� ȣ��
            ImGui::CloseCurrentPopup();
            IsShowLoadModal = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Close"))
        {
            ImGui::CloseCurrentPopup();
            IsShowLoadModal = false;
        }

        ImGui::EndPopup();
    }
}

void CTool_Manager::Release_Tool()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CTool_Manager::DestroyInstance();
}

void CTool_Manager::Bind_FileName()
{

    // _finddata_t : <io.h>���� �����ϸ� ���� ������ �����ϴ� ����ü
    _wfinddata64_t fd;
    __int64 handle = _wfindfirst64(L"../../Data/*.*", &fd);
    if (handle == -1 || handle == 0)
        return;

	// _findfirst : <io.h>���� �����ϸ� ����ڰ� ������ ��� ������ ���� ù ��° ������ ã�� �Լ�

	int iResult = 0; 

	char szCurPath[128] = "../Data/";
	char szFullPath[128] = ""; 
    char szFileName[MAX_PATH] = "";

    _uint iFileIndex = 0;
	while (iResult != -1)
	{
		//strcpy_s(szFullPath, szCurPath); 

		//// "../Sound/Success.wav"
		//strcat_s(szFullPath, fd.name);
        wstring FileName(fd.name);
        string strFileName(FileName.begin(), FileName.end());
        m_FileName.emplace_back(strFileName);

        //WideCharToMultiByte(CP_UTF8, 0, fd.name, -1, szFileName, sizeof(szFileName), NULL, NULL);
        //m_FileName[iFileIndex++].c_str() = szFileName;

		//_findnext : <io.h>���� �����ϸ� ���� ��ġ�� ������ ã�� �Լ�, ���̻� ���ٸ� -1�� ����
		iResult = _wfindnext64(handle, &fd);
	}

	_findclose(handle);
}

void CTool_Manager::Guizmo_Test()
{
    ImGuizmo::BeginFrame();

    static bool useSnap = false;
    static float snap[3] = { 1.f, 1.f, 1.f };
    static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
    static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
    static bool boundSizing = false;
    static bool boundSizingSnap = false;

    
    float viewManipulateRight = 1280.f;
    float viewManipulateTop = 0;

    //ImGui::SetNextWindowSize(ImVec2(1280, 720));
    //ImGui::SetNextWindowPos(ImVec2(0, 0));

    //ImGui::Begin("Guizmo");
    //ImGuizmo::SetDrawlist();
    //float windowWidth = (float)ImGui::GetWindowWidth();
    //float windowHeight = (float)ImGui::GetWindowHeight();
    //ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
    ImGuizmo::SetRect(0, 0, 1280.f, 720.f);

    //float viewManipulateRight = ImGui::GetWindowPos().x + windowWidth;
    //float viewManipulateTop = ImGui::GetWindowPos().y;

    _matrix cameraView = m_pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_VIEW);
    _matrix cameraProjection = m_pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_PROJ);
    _float viewMatrix[16], projMatrix[16];

    memcpy(&viewMatrix, &cameraView, sizeof(_matrix));
    memcpy(&projMatrix, &cameraProjection, sizeof(_matrix));

    ImGuizmo::DrawGrid(viewMatrix, projMatrix, identityMatrix, 100.f);
    ImGuizmo::DrawCubes(viewMatrix, projMatrix, &objectMatrix[0][0], gizmoCount);
    ImGuizmo::Manipulate(viewMatrix, projMatrix, mCurrentGizmoOperation, mCurrentGizmoMode, objectMatrix[0], NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);

    ImGuizmo::ViewManipulate(viewMatrix, camDistance, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);

    //ImGui::End();
}

void CTool_Manager::Free()
{
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
    Safe_Release(m_pObject_Manager);
    Safe_Release(m_pGameInstance);
}
