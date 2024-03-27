#include "Tool_Manager.h"

#include "GameObject.h"
#include "Tool_Object.h"

IMPLEMENT_SINGLETON(CTool_Manager)


_float3 CTool_Manager::vWorldMousePos = { 0.f, 0.f, 0.f };
_float CTool_Manager::fScaleTerrainX = { 1.f };
_float CTool_Manager::fScaleTerrainZ = { 1.f };
_bool CTool_Manager::mTerrainPicking = { false };
_int CTool_Manager::item_current_idx = { 0 };

vector<CGameObject*> CTool_Manager::Objects;

CTool_Manager::CTool_Manager()
{
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
}

void CTool_Manager::Render()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void CTool_Manager::Window_Terrain()
{
    ImGui::Begin("Terrain");

    ImGui::Text("Terrain Scale");
    ImGui::SliderFloat("Terrain Width", &fScaleTerrainX, 0.1f, 10.f);
    ImGui::SliderFloat("Terrain Height", &fScaleTerrainZ, 0.1f, 10.f);

    ImGui::End();
}

void CTool_Manager::Window_Object()
{
    ImGui::Begin("Object");

    ImGui::Checkbox("Terrain Picking", &mTerrainPicking);
    ImGui::NewLine();
    ImGui::Text("Picking MousePos : Terrain");

    ImGui::Text("MousePos : %f, %f, %f", vWorldMousePos.x, vWorldMousePos.y, vWorldMousePos.z);

    if (ImGui::BeginListBox("Created Object List"))
    {
        for (int n = 0; n < Objects.size(); n++)
        {
            const bool is_selected = (item_current_idx == n);
            if (ImGui::Selectable(dynamic_cast<CTool_Object*>(Objects[n])->Get_ObjectName(), is_selected))
            {
                item_current_idx = n;
            }

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }

    ImGui::End();
}

void CTool_Manager::Release_Tool()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CTool_Manager::DestroyInstance();
}

void CTool_Manager::Free()
{
    for (auto& iter : Objects)
        Safe_Release(iter);

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
