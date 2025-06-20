#include "Tool_Manager.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "Tool_Object.h"
#include "Tool_Object_Manager.h"
#include "Tool_Terrain.h"

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


    //디렉토리 받아서 파일 저장하기
    Bind_FileName();
    Bind_NavigationFileName();

	return S_OK;
}

void CTool_Manager::Tick(const _float& fTimeDelta)
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();


    _bool show_demo_window = true;
    ImGui::ShowDemoWindow(&show_demo_window);

    Window_Terrain();
    Window_Object();
    Window_MainBar();
    Window_Navigation();

    Modal_Save();
    Modal_Load();

    Modal_NavigationSave();
    Modal_NavigationLoad();

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

    ImGui::Checkbox("Terrain Render Off", &m_isShowTerrain);
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
        m_pObject_Manager->Add_CloneObject( 
            CTool_Object_Manager::OBJECT_DUNGEON, 
            L"GameObject_Dungeon", 
            XMVectorSet(0.f, 0.f, 0.f, 1.f), 
            0,
            m_iCreateDungeonIndex);
    }

    ImGui::NewLine();

    ImGui::Text("Terrain Rotation");
    ImGui::SliderFloat("Rotaion", &m_fDungeonDegree, 0.f, 360.f);

    ImGui::Text("Terrain X");
    ImGui::SliderFloat("X", &m_fDungeonPos.x, 0.f, 1000.f);

    ImGui::Text("Terrain y");
    ImGui::SliderFloat("y", &m_fDungeonPos.y, 0.f, 1000.f);

    ImGui::Text("Terrain z");
    ImGui::SliderFloat("z", &m_fDungeonPos.z, 0.f, 1000.f);

    ImGui::End();
}

void CTool_Manager::Window_Object()
{
    ImGui::Begin("Object");

    ImGui::Checkbox("Terrain Picking", &mTerrainPicking);
    ImGui::Checkbox("Mesh Picking", &m_isPickingWithDungeon);

    ImGui::NewLine();
    ImGui::Text("Picking MousePos : PlayerPos");
    ImGui::Checkbox("Player Picking", &m_isPlayerPosPicking);
    ImGui::InputFloat3("PlayerPos", m_pObject_Manager->Get_PlayerPos());
    ImGui::Text("Player Navigation Index"); ImGui::SameLine();

    _int iIndex = m_pObject_Manager->Get_PlayerNavigationIndex();
    ImGui::InputInt("Index", &iIndex);

    ImGui::NewLine();
    ImGui::Text("Picking MousePos : Terrain");

    ImGui::Text("MousePos : %f, %f, %f", vWorldMousePos.x, vWorldMousePos.y, vWorldMousePos.z);

    if (ImGui::BeginListBox("Selete Create Monster"))
    {
        for (int n = 0; n < m_pObject_Manager->Get_CloneDescs(CTool_Object_Manager::OBJECT_SCENE).size(); n++)
        {
            const bool is_selected = (m_iCreateMonsterIndex == n);
            if (ImGui::Selectable(m_pObject_Manager->Get_CloneDescs(CTool_Object_Manager::OBJECT_SCENE)[n].strName.c_str(), is_selected))
            {
                m_iCreateMonsterIndex = n;
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
        vector<CTool_Object*> Objects = m_pObject_Manager->Get_Objects();
        for (int n = 0; n < Objects.size(); n++)
        {
            const bool is_selected = (m_iCurrentPickingObjectIndex == n);
            if (ImGui::Selectable(Objects[n]->Get_ObjectName().c_str(), is_selected))
            {
                m_iCurrentPickingObjectIndex = n;

                vector<CTool_Object*> Objects = m_pObject_Manager->Get_Objects();
                _matrix ObjectMatrix = Objects[m_iCurrentPickingObjectIndex]->m_pTransformCom->Get_WorldMatrix();
                
                memcpy(&m_pObjectMatrix, &ObjectMatrix, sizeof(_matrix));   
            }

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }



    if (ImGui::Button("Remove"))
    {
        m_pObject_Manager->Remove_Object(m_iCurrentPickingObjectIndex);
        m_iCurrentPickingObjectIndex = m_pObject_Manager->Get_Objects().size() - 1;
        if (m_pObject_Manager->Get_Objects().size() <= 0)
        {
            m_iCurrentPickingObjectIndex = -1;
            ZeroMemory(m_pObjectMatrix, sizeof(_matrix));
        }
        else {
            vector<CTool_Object*> Objects = m_pObject_Manager->Get_Objects();
            _matrix ObjectMatrix = Objects[m_iCurrentPickingObjectIndex]->m_pTransformCom->Get_WorldMatrix();

            memcpy(&m_pObjectMatrix, &ObjectMatrix, sizeof(_matrix));
        }
    }

    ImGui::Text(ImGuizmo::IsOver(ImGuizmo::TRANSLATE) ? "Over translate gizmo" : "");

    if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
        mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
        mCurrentGizmoOperation = ImGuizmo::ROTATE;
    ImGui::SameLine();
    if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
        mCurrentGizmoOperation = ImGuizmo::SCALE;

    float matrixTranslation[3], matrixRotation[3], matrixScale[3];
    ImGuizmo::DecomposeMatrixToComponents(m_pObjectMatrix, matrixTranslation, matrixRotation, matrixScale);
    ImGui::InputFloat3("Tr", matrixTranslation);
    ImGui::InputFloat3("Rt", matrixRotation);
    ImGui::InputFloat3("Sc", matrixScale);
    ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, m_pObjectMatrix);

    if (m_iCurrentPickingObjectIndex != -1)
    {
        vector<CTool_Object*> Objects = m_pObject_Manager->Get_Objects();
        _matrix ObjectMatrix = Objects[m_iCurrentPickingObjectIndex]->m_pTransformCom->Get_WorldMatrix();
        memcpy(&ObjectMatrix, &m_pObjectMatrix, sizeof(_float) * 16);
        Objects[m_iCurrentPickingObjectIndex]->m_pTransformCom->Set_State(CTransform::STATE_RIGHT, ObjectMatrix.r[0]);
        Objects[m_iCurrentPickingObjectIndex]->m_pTransformCom->Set_State(CTransform::STATE_UP, ObjectMatrix.r[1]);
        Objects[m_iCurrentPickingObjectIndex]->m_pTransformCom->Set_State(CTransform::STATE_LOOK, ObjectMatrix.r[2]);
        Objects[m_iCurrentPickingObjectIndex]->m_pTransformCom->Set_State(CTransform::STATE_POSITION, ObjectMatrix.r[3]);
    }

    if (m_iCurrentPickingObjectIndex == -1)
    {
        ImGui::End();
        return;
    }
        

    //수정
    vector<CTool_Object*> Objects = m_pObject_Manager->Get_Objects();
    _int iPathIndex = Objects[m_iCurrentPickingObjectIndex]->Get_PathIndex();

    if (ImGui::InputInt("Path Index", &iPathIndex))
        Objects[m_iCurrentPickingObjectIndex]->Set_PathIndex(iPathIndex);

    ImGui::End();
}

void CTool_Manager::Window_MainBar() // 상태바 윈도우 
{
    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("File"))
    {
        ImGui::MenuItem("FileOpen", nullptr, &m_isShowLoadModal);// FileLoad
        ImGui::MenuItem("FileSave", nullptr, &m_isShowSaveModal);// FileSave
        ImGui::MenuItem("NavigationOpen", nullptr, &m_isShowNavigationLoadModal);// NavigationLoad
        ImGui::MenuItem("NavigationSave", nullptr, &m_isShowNavigationSaveModal);// NavigationSave

        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
}

void CTool_Manager::Window_Navigation() // 내비게이션 윈도우
{
    ImGui::Begin("Navigation");

    ImGui::Checkbox("Navigation Picking", &m_isNavigationPicking);
    ImGui::Checkbox("Cell Remove Picking", &m_isCellRemove);
    ImGui::RadioButton("None", &m_iNavigationOption, 0); ImGui::SameLine();
    ImGui::RadioButton("Stairs", &m_iNavigationOption, 1); ImGui::SameLine();
    ImGui::RadioButton("Fly", &m_iNavigationOption, 2); ImGui::SameLine();

    ImGui::End();
}

void CTool_Manager::Modal_Save() // 파일 저장
{
    if (m_isShowSaveModal)
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
            // 저장하는 함수 호출 & 불변수 호출
            if (IsNewFile)
                m_pObject_Manager->Save(m_szNewFileName);
            else
                m_pObject_Manager->Save(m_FileName[m_iSaveFileIndex].c_str());

            Bind_FileName();
            ImGui::CloseCurrentPopup();            
            m_isShowSaveModal = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Close"))
        {
            ImGui::CloseCurrentPopup();
            m_isShowSaveModal = false;
        }

        ImGui::EndPopup();
    }
}

void CTool_Manager::Modal_Load() // 파일 로드
{
    if (m_isShowLoadModal)
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
            // 불러오는 함수 호출 & 불변수 호출
            m_pObject_Manager->Load(m_FileName[m_iLoadFileIndex].c_str());

            ImGui::CloseCurrentPopup();
            m_isShowLoadModal = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Close"))
        {
            ImGui::CloseCurrentPopup();
            m_isShowLoadModal = false;
        }

        ImGui::EndPopup();
    }
}

void CTool_Manager::Modal_NavigationSave() // 내비게이션 파일 저장 
{
    if (m_isShowNavigationSaveModal)
        ImGui::OpenPopup("SaveNavigationFile");

    if (ImGui::BeginPopupModal("SaveNavigationFile", NULL, ImGuiWindowFlags_MenuBar))
    {
        if (ImGui::BeginListBox("File Seleted"))
        {
            for (int n = 0; n < m_NavigationFileName.size(); n++)
            {
                const bool is_selected = (m_iSaveNavigationFileIndex == n);
                if (ImGui::Selectable(m_NavigationFileName[n].c_str(), is_selected))
                {
                    m_iSaveNavigationFileIndex = n;
                }

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }
        ImGui::Checkbox("Create New File ? ", &IsNewFile);
        ImGui::InputText("New File", m_szNavigationFileName, MAX_PATH);

        if (ImGui::Button("Save"))
        {
            // 저장하는 함수 호출 & 불변수 호출
            if (IsNewFile)
                m_pObject_Manager->Cell_Save(m_szNavigationFileName);
            else
                m_pObject_Manager->Cell_Save(m_NavigationFileName[m_iSaveNavigationFileIndex].c_str());

            Bind_NavigationFileName();
            ImGui::CloseCurrentPopup();
            m_isShowNavigationSaveModal = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Close"))
        {
            ImGui::CloseCurrentPopup();
            m_isShowNavigationSaveModal = false;
        }

        ImGui::EndPopup();
    }
}

void CTool_Manager::Modal_NavigationLoad() // 내비게이션 파일 로드
{
    if (m_isShowNavigationLoadModal)
        ImGui::OpenPopup("LoadNavigationFile");

    if (ImGui::BeginPopupModal("LoadNavigationFile", NULL, ImGuiWindowFlags_MenuBar))
    {
        if (ImGui::BeginListBox("File Seleted"))
        {
            for (int n = 0; n < m_NavigationFileName.size(); n++)
            {
                const bool is_selected = (m_iLoadNavigationFileIndex == n);
                if (ImGui::Selectable(m_NavigationFileName[n].c_str(), is_selected))
                {
                    m_iLoadNavigationFileIndex = n;
                }

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }

        if (ImGui::Button("Load"))
        {
            // 불러오는 함수 호출 & 불변수 호출
            m_pObject_Manager->Cell_Load(m_NavigationFileName[m_iLoadNavigationFileIndex].c_str());

            ImGui::CloseCurrentPopup();
            m_isShowNavigationLoadModal = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Close"))
        {
            ImGui::CloseCurrentPopup();
            m_isShowNavigationLoadModal = false;
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
    m_FileName.clear();
    // _finddata_t : <io.h>에서 제공하며 파일 정보를 저장하는 구조체
    _wfinddata64_t fd;
    __int64 handle = _wfindfirst64(L"../../Data/Stage/*.*", &fd);
    if (handle == -1 || handle == 0)
        return;

	// _findfirst : <io.h>에서 제공하며 사용자가 설정한 경로 내에서 가장 첫 번째 파일을 찾는 함수

	int iResult = 0; 

	char szCurPath[128] = "../Data/Stage/";
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

		//_findnext : <io.h>에서 제공하며 다음 위치의 파일을 찾는 함수, 더이상 없다면 -1을 리턴
		iResult = _wfindnext64(handle, &fd);
	}

	_findclose(handle);
}

void CTool_Manager::Bind_NavigationFileName()
{
    m_NavigationFileName.clear();

    _wfinddata64_t fd;
    __int64 handle = _wfindfirst64(L"../../Data/Navigation/*.*", &fd);
    if (handle == -1 || handle == 0)
        return;

    int iResult = 0;

    char szCurPath[128] = "../Data/Navigation/";
    char szFullPath[128] = "";
    char szFileName[MAX_PATH] = "";

    _uint iFileIndex = 0;
    while (iResult != -1)
    {
        wstring FileName(fd.name);
        string strFileName(FileName.begin(), FileName.end());
        m_NavigationFileName.emplace_back(strFileName);

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

    
    float viewManipulateRight = g_iWinSizeX;
    float viewManipulateTop = 0;

    ImGuizmo::SetRect(0, 0, g_iWinSizeX, g_iWinSizeY);

    _matrix cameraView = m_pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_VIEW);
    _matrix viewInverse = m_pGameInstance->Get_Transform_Inverse_Matrix(CPipeLine::D3DTS_VIEW); // 카메라 월드 
    _matrix cameraProjection = m_pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_PROJ);
    _float viewMatrix[16], projMatrix[16], viewInverseMatrix[16];

    memcpy(&viewMatrix, &cameraView, sizeof(_matrix));
    memcpy(&viewInverseMatrix, &viewInverse, sizeof(_matrix));
    memcpy(&projMatrix, &cameraProjection, sizeof(_matrix));

    //ImGuizmo::DrawGrid(viewMatrix, projMatrix, identityMatrix, 100.f);

    if (m_iCurrentPickingObjectIndex != -1)
        ImGuizmo::Manipulate(viewMatrix, projMatrix, mCurrentGizmoOperation, mCurrentGizmoMode, m_pObjectMatrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);
   
    ImGuizmo::ViewManipulate(viewInverseMatrix, camDistance, ImVec2(viewManipulateRight - 128, viewManipulateTop), ImVec2(128, 128), 0x10101010);

    memcpy(&cameraView, &viewInverseMatrix, sizeof(_float) * 16);
    m_pGameInstance->Set_Transform(CPipeLine::D3DTS_VIEW, XMMatrixInverse(nullptr, cameraView));
}

void CTool_Manager::Free()
{
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
    Safe_Release(m_pObject_Manager);
    Safe_Release(m_pGameInstance);
}
