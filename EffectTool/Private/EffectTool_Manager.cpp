#include "EffectTool_Manager.h"

#include "Effect_Manager.h"

#include "Effect_Mesh.h"
#include "Effect_Default.h"
#include "Tool_Effect_Point.h"
#include "Effect_Anim_Model.h"
#include <Effect_Trail.h>

IMPLEMENT_SINGLETON(CEffectTool_Manager)

CEffectTool_Manager::CEffectTool_Manager()
	: m_pGameInstance{ CGameInstance::GetInstance() },
    m_pEffectManager { CEffect_Manager::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
    Safe_AddRef(m_pEffectManager);
}

HRESULT CEffectTool_Manager::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	Safe_AddRef(m_pDevice);

	m_pContext = pContext;
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

	////���丮 �޾Ƽ� ���� �����ϱ�
	Bind_FileName();

	return S_OK;
}

void CEffectTool_Manager::Tick(const _float& fTimeDelta)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	_bool show_demo_window = true;
	ImGui::ShowDemoWindow(&show_demo_window);


	Window_Effect();
	Window_MainBar();
    Window_Model();

    Effect_Save();
    Effect_Load();
    Window_EffectPatch();

    Guizmo();
}

void CEffectTool_Manager::Render()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void CEffectTool_Manager::Window_Effect()
{
	ImGui::Begin("Effect");

    ImGui::RadioButton("Particle", &m_iEffectType, 0);ImGui::SameLine();
    ImGui::RadioButton("Effect_Img", &m_iEffectType, 1); ImGui::SameLine();
    ImGui::RadioButton("Effect_Mesh", &m_iEffectType, 2);ImGui::SameLine();
    ImGui::RadioButton("Effect_Trail", &m_iEffectType, 3);ImGui::SameLine();
    ImGui::RadioButton("Effect_Cell", &m_iEffectType, 4); 

    ImGui::NewLine();

    // Texture �����
    if (ImGui::Button("Open TextureFile")) {
        IGFD::FileDialogConfig config;
        config.path = "../../Client/Bin/Resources/Textures/";
        ImGuiFileDialog::Instance()->OpenDialog("ChooseTextureFileDlgKey", "Choose File", ".png, .jpg, .dds", config);
    }
    
    // display
    if (ImGuiFileDialog::Instance()->Display("ChooseTextureFileDlgKey")) {
        if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
            m_strTextureFilePath = ImGuiFileDialog::Instance()->GetFilePathName();
            string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
            // action
        }

        // close
        ImGuiFileDialog::Instance()->Close();
    }

    ImGui::InputInt("Texture Count : ", &m_iTextureNum);

    char szTextureFileName[MAX_PATH] = "";
    sprintf_s(szTextureFileName, "TextureFileName : %s", m_strTextureFilePath.c_str());
    ImGui::Text(szTextureFileName);

    ImGui::Checkbox("Mask Texture? ", &m_isMask);
    if (m_isMask)
    {
        ImGui::NewLine();

        // Mask �����
        if (ImGui::Button("Open MaskFile")) {
            IGFD::FileDialogConfig config;
            config.path = "../../Client/Bin/Resources/Textures/";
            ImGuiFileDialog::Instance()->OpenDialog("ChooseMaskFileDlgKey", "Choose File", ".png, .jpg, .dds", config);
        }

        // display
        if (ImGuiFileDialog::Instance()->Display("ChooseMaskFileDlgKey")) {
            if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
                m_strMaskFilePath = ImGuiFileDialog::Instance()->GetFilePathName();
                string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
                // action
            }
            // close
            ImGuiFileDialog::Instance()->Close();
        }

        char szMaskFileName[MAX_PATH] = "";
        sprintf_s(szMaskFileName, "MaskFileName : %s", m_strMaskFilePath.c_str());
        ImGui::Text(szMaskFileName);

        ImGui::NewLine();
    }
    ImGui::NewLine();

    ImGui::Checkbox("Noise Texture? ", &m_isNoise);
    if (m_isNoise)
    {
        ImGui::NewLine();

        // Mask �����
        if (ImGui::Button("Open NoiseFile")) {
            IGFD::FileDialogConfig config;
            config.path = "../../Client/Bin/Resources/Textures/";
            ImGuiFileDialog::Instance()->OpenDialog("ChooseNoiseFileDlgKey", "Choose File", ".png, .jpg, .dds", config);
        }

        // display
        if (ImGuiFileDialog::Instance()->Display("ChooseNoiseFileDlgKey")) {
            if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
                m_strNoiseFilePath = ImGuiFileDialog::Instance()->GetFilePathName();
                string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
                // action
            }
            // close
            ImGuiFileDialog::Instance()->Close();
        }

        char szNoiseFileName[MAX_PATH] = "";
        sprintf_s(szNoiseFileName, "NoiseFileName : %s", m_strNoiseFilePath.c_str());
        ImGui::Text(szNoiseFileName);

        ImGui::NewLine();
    }
    ImGui::NewLine();

    // Mesh ����� (mesh ������ .dat ���Ϸ� ��������)
    if (ImGui::Button("Open MeshFile")) {
        IGFD::FileDialogConfig config;
        config.path = "../../Data/EffectMesh/";
        ImGuiFileDialog::Instance()->OpenDialog("ChooseMeshFileDlgKey", "Choose File", ".dat", config);
    }
    // display
    if (ImGuiFileDialog::Instance()->Display("ChooseMeshFileDlgKey")) {
        if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
            m_strMeshFilePath = ImGuiFileDialog::Instance()->GetFilePathName();
            string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
            // action
        }

        // close
        ImGuiFileDialog::Instance()->Close();
    }

    char szMeshFileName[MAX_PATH] = "";
    sprintf_s(szMeshFileName, "MeshFileName : %s", m_strMeshFilePath.c_str());
    ImGui::Text(szMeshFileName);

    ImGui::NewLine();

    //��ƼŬ �غ�
    ImGui::NewLine();

    ImGui::RadioButton("Spread", &m_ParticleType, 1); ImGui::SameLine();
    ImGui::RadioButton("Drop", &m_ParticleType, 0); ImGui::SameLine();
    ImGui::RadioButton("Foutain", &m_ParticleType, 2);

    ImGui::InputInt("Instance Num", &m_iNumInstance);
    ImGui::InputFloat3("Offset", (_float*)&m_vOffsetPos);
    ImGui::InputFloat3("Pivot", (_float*)&m_vPivotPos);
    ImGui::InputFloat3("Range", (_float*)&m_vRange);
    ImGui::InputFloat2("Size", (_float*)&m_vSize);
    ImGui::InputFloat2("Speed", (_float*)&m_vSpeed);
    ImGui::InputFloat2("Power", (_float*)&m_vPower);
    ImGui::InputFloat2("LifeTime", (_float*)&m_vLifeTime);
    ImGui::Checkbox("Loop", &m_isLoop);

    if (ImGui::Button("Effect/Particle Create Start"))
    {
        //�����ϱ�
        //Ÿ�Ժ��� ��� ������ ������ 
        wstring strTextureFilePath = TEXT("");
        strTextureFilePath.assign(m_strTextureFilePath.begin(), m_strTextureFilePath.end());

        if (m_iEffectType == CEffect_Manager::PARTICLE)
        {
            CTool_Effect_Point::EFFECT_POINT_DESC EffectDesc{};
            EffectDesc.iNumInstance = m_iNumInstance;
            EffectDesc.vOffsetPos = m_vOffsetPos;
            EffectDesc.vPivotPos = m_vPivotPos;
            EffectDesc.vRange = m_vRange;
            EffectDesc.vSize = m_vSize;
            EffectDesc.vSpeed = m_vSpeed;
            EffectDesc.vLifeTime = m_vLifeTime;
            EffectDesc.vPower = m_vPower;
            EffectDesc.isLoop = m_isLoop;
            EffectDesc.iParticleType = m_ParticleType;
            strcpy_s(EffectDesc.strTextureFilePath, m_strTextureFilePath.c_str());
            EffectDesc.iEffectType = m_iEffectType;
            EffectDesc.isMask = m_isMask;
            EffectDesc.strMaskFilePath = m_strMaskFilePath;
            EffectDesc.isNoise = m_isNoise;
            EffectDesc.strNoiseFilePath = m_strNoiseFilePath;
            EffectDesc.iTextureNum = m_iTextureNum;

            if (FAILED(m_pEffectManager->Add_Effect(CEffect_Manager::EFFECT_TYPE(m_iEffectType), &EffectDesc)))
            {
                MessageBoxW(g_hWnd, L"��ƼŬ ���� ����", 0, 0);
            }
        }
        else if (m_iEffectType == CEffect_Manager::EFFECT_MESH || m_iEffectType == CEffect_Manager::EFFECT_TRAIL)
        {
            CEffect_Mesh::EFFECT_MESH_DESC EffectDesc{};
           
            strcpy_s(EffectDesc.strTextureFilePath, m_strTextureFilePath.c_str());
            strcpy_s(EffectDesc.szModelFilePath, m_strMeshFilePath.c_str());
            EffectDesc.iEffectType = m_iEffectType;
            EffectDesc.isMask = m_isMask;
            EffectDesc.strMaskFilePath = m_strMaskFilePath;
            EffectDesc.isNoise = m_isNoise;
            EffectDesc.strNoiseFilePath = m_strNoiseFilePath;
            EffectDesc.iTextureNum = m_iTextureNum;

            if (FAILED(m_pEffectManager->Add_Effect(CEffect_Manager::EFFECT_TYPE(m_iEffectType), &EffectDesc)))
            {
                MessageBoxW(g_hWnd, L"�޽� ���� ����", 0, 0);
            }
        }
        else if (m_iEffectType == CEffect_Manager::EFFECT_IMG || m_iEffectType == CEffect_Manager::EFFECT_CELL)
        {
            CEffect_Default::EFFECT_DEFAULT_DESC EffectDesc{};
            strcpy_s(EffectDesc.strTextureFilePath, m_strTextureFilePath.c_str());
            EffectDesc.iEffectType = m_iEffectType;
            EffectDesc.fRotatePecSec = m_fRotateSpeed;
            EffectDesc.fSpeedPecSec = m_fTextureSpeed;
            EffectDesc.iTextureNum = m_iTextureNum;
            EffectDesc.isNoise = m_isNoise;
            EffectDesc.strNoiseFilePath = m_strNoiseFilePath;
            EffectDesc.isMask = m_isMask;
            EffectDesc.strMaskFilePath = m_strMaskFilePath;

            if (FAILED(m_pEffectManager->Add_Effect(CEffect_Manager::EFFECT_TYPE(m_iEffectType), &EffectDesc)))
            {
                MessageBoxW(g_hWnd, L"�ؽ��� ���� ����", 0, 0);
            }
        }
    }

	ImGui::End();
}

void CEffectTool_Manager::Window_MainBar() // ���¹� ������ 
{
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("File"))
	{
		ImGui::MenuItem("FileOpen", nullptr, &m_isShowLoadModal);// FileLoad
		ImGui::MenuItem("FileSave", nullptr, &m_isShowSaveModal);// FileSave

		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
}

void CEffectTool_Manager::Window_EffectPatch()
{
    ImGui::Begin("Patch");

    vector<CTool_Effect*> Effects = m_pEffectManager->Get_Effects();

    if (ImGui::BeginListBox("EffectList"))
    {    
        for (int n = 0; n < Effects.size(); n++)
        {
            const bool is_selected = (m_iPatchIndex == n);
            string strName = to_string(n);
            if (ImGui::Selectable(strName.c_str(), is_selected))
            {
                m_iPatchIndex = n;

                _matrix ObjectMatrix = Effects[m_iPatchIndex]->m_pTransformCom->Get_WorldMatrix();

                memcpy(&m_EffectMatrix, &ObjectMatrix, sizeof(_matrix));
            }

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }

    if (ImGui::Button("Remove"))
    {
        m_pEffectManager->Remove_Effect(m_iPatchIndex);

        vector<CTool_Effect*> Effects = m_pEffectManager->Get_Effects();
        m_iPatchIndex = Effects.size() - 1;
        if (Effects.size() <= 0)
        {
            m_iPatchIndex = -1;
            ZeroMemory(&m_EffectMatrix, sizeof(_float4x4));
        }
        else {
            _matrix EffectMatrix = Effects[m_iPatchIndex]->m_pTransformCom->Get_WorldMatrix();
            memcpy(&m_EffectMatrix, &EffectMatrix, sizeof(_matrix));
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
    ImGuizmo::DecomposeMatrixToComponents((_float*)&m_EffectMatrix, matrixTranslation, matrixRotation, matrixScale);
    ImGui::InputFloat3("Tr", matrixTranslation);
    ImGui::InputFloat3("Rt", matrixRotation);
    ImGui::InputFloat3("Sc", matrixScale);
    ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, (_float*)&m_EffectMatrix);

    if (m_iPatchIndex != -1)
        Effects[m_iPatchIndex]->m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&m_EffectMatrix));
    
    if (m_iPatchIndex == -1)
    {
        ImGui::End();
        return;
    }
        
    CTool_Effect* pCurrentEffect = Effects[m_iPatchIndex];

    if (pCurrentEffect->Get_EffectType() == CEffect_Manager::EFFECT_IMG)
    {
        // Ÿ�� ����
        CEffect_Default* pTexture = dynamic_cast<CEffect_Default*>(pCurrentEffect);
        _int iType = pTexture->Get_TextureMoveType();
        // INCREASE, SHRINK, RUN
        if (ImGui::RadioButton("INCREASE", &iType, 0)) // Ȯ��
            pTexture->Set_TextureMoveType(iType);
        else if (ImGui::RadioButton("SHRINK", &iType, 1)) // ���
            pTexture->Set_TextureMoveType(iType);
        else if (ImGui::RadioButton("SPREAK", &iType, 2)) // ������
            pTexture->Set_TextureMoveType(iType);
        else if (ImGui::RadioButton("TRAIL", &iType, 3)) // Ʈ���� ���۰��� �뵵
            pTexture->Set_TextureMoveType(iType);
        else if (ImGui::RadioButton("STOP", &iType, 4)) // end
            pTexture->Set_TextureMoveType(iType);

        _float fSpeed = pTexture->Get_TextureMoveSpeed();

        if (ImGui::InputFloat("Speed", &fSpeed))
            pTexture->Set_TextureMoveSpeed(fSpeed);

    }
    if (pCurrentEffect->Get_EffectType() == CEffect_Manager::EFFECT_MESH)
    {
        CEffect_Mesh* pMesh = dynamic_cast<CEffect_Mesh*>(pCurrentEffect);
        _float2 vUV = pMesh->Get_UV();
        if (ImGui::InputFloat2("UV", (_float*)&vUV))
        {
            pMesh->Set_UV(vUV);
        }
    }
    else if (pCurrentEffect->Get_EffectType() == CEffect_Manager::EFFECT_TRAIL)
    {
        CEffect_Trail* pTrail = dynamic_cast<CEffect_Trail*>(pCurrentEffect);
        _float2 vUV = pTrail->Get_UV();
        if (ImGui::InputFloat2("UV", (_float*)&vUV))
        {
            pTrail->Set_UV(vUV);
        }

        ImGui::NewLine();

        _int iTrailType = pTrail->Get_TrailMoveType();
        if (ImGui::RadioButton("Non", &iTrailType, 0))
            pTrail->Set_TrailMoveType(iTrailType);
        if (ImGui::RadioButton("Nilou_Flower", &iTrailType, 1))
            pTrail->Set_TrailMoveType(iTrailType);
        if (ImGui::RadioButton("Non", &iTrailType, 2))
            pTrail->Set_TrailMoveType(iTrailType);

        ImGui::NewLine();
    }

    _int iCurrentRendererType = pCurrentEffect->Get_RendererType();

    if (ImGui::RadioButton("NONLIGHT", &iCurrentRendererType, CRenderer::RENDER_NONLIGHT))
        pCurrentEffect->Set_RendererType(iCurrentRendererType);
    else if (ImGui::RadioButton("BLAND", &iCurrentRendererType, CRenderer::RENDER_BLENDER))
        pCurrentEffect->Set_RendererType(iCurrentRendererType);

    ImGui::NewLine();

    //������ ����
    _bool isFrameLoop = pCurrentEffect->isFrameLoop();
    if (ImGui::Checkbox("FrameLoop", &isFrameLoop))
        pCurrentEffect->Set_FrameLoop(isFrameLoop);

    ImGui::NewLine();

    //���� �ð�
    _float fStartTime = pCurrentEffect->Get_StartTime();
    _float fDurationTime = pCurrentEffect->Get_DurationTime();

    if (ImGui::InputFloat("StartTime : ", &fStartTime))
        pCurrentEffect->Set_StartTime(fStartTime);
    if (ImGui::InputFloat("DurationTime : ", &fDurationTime))
        pCurrentEffect->Set_DurationTime(fDurationTime);

    if (ImGui::Button("Reset"))
        pCurrentEffect->Reset();
    if (ImGui::Button("All Reset"))
        m_pEffectManager->All_Reset();

    ImGui::NewLine();

    //���� ���� 
    m_vColor = pCurrentEffect->Get_Color();
    if (ImGui::InputFloat4("Color : ", (_float*)&m_vColor))
        pCurrentEffect->Set_Color(m_vColor);

    ImGui::NewLine();


    m_iShaderPass = pCurrentEffect->Get_ShaderPass();
    _uint iEffectType = pCurrentEffect->Get_EffectType();
    //���̴� �ɼ� �����ϱ� (����Ʈ ���� �ٸ��� �ֱ�)
    // ���̴����� �� �� �ֵ��� �����ϱ�
    if (iEffectType == CEffect_Manager::PARTICLE)
    {
        if (ImGui::RadioButton("All Color Alpha", &m_iShaderPass, 0))
            pCurrentEffect->Set_ShaderPass(m_iShaderPass);
        if (ImGui::RadioButton("Spread", &m_iShaderPass, 1))
            pCurrentEffect->Set_ShaderPass(m_iShaderPass);
        if (ImGui::RadioButton("Nilou_Water", &m_iShaderPass, 2))
            pCurrentEffect->Set_ShaderPass(m_iShaderPass);
        if (ImGui::RadioButton("Feiyan_Burst", &m_iShaderPass, 3))
            pCurrentEffect->Set_ShaderPass(m_iShaderPass);
    }
    else if (iEffectType == CEffect_Manager::EFFECT_IMG)
    {
        if (ImGui::RadioButton("Default", &m_iShaderPass, 0))
            pCurrentEffect->Set_ShaderPass(m_iShaderPass);
        if (ImGui::RadioButton("Color", &m_iShaderPass, 1))
            pCurrentEffect->Set_ShaderPass(m_iShaderPass);
        if (ImGui::RadioButton("FeiyanFire", &m_iShaderPass, 2))
            pCurrentEffect->Set_ShaderPass(m_iShaderPass);
        if (ImGui::RadioButton("Noise Non Mask", &m_iShaderPass, 3))
            pCurrentEffect->Set_ShaderPass(m_iShaderPass);
        if (ImGui::RadioButton("Tighnari_Elemenetal", &m_iShaderPass, 4))
            pCurrentEffect->Set_ShaderPass(m_iShaderPass);
        if (ImGui::RadioButton("Tighnari_Normal", &m_iShaderPass, 5))
            pCurrentEffect->Set_ShaderPass(m_iShaderPass);
        if (ImGui::RadioButton("Tartaglia_Rush", &m_iShaderPass, 6))
            pCurrentEffect->Set_ShaderPass(m_iShaderPass);
        if (ImGui::RadioButton("Tartaglia_AlertCircle", &m_iShaderPass, 7))
            pCurrentEffect->Set_ShaderPass(m_iShaderPass);
        if (ImGui::RadioButton("Tartaglia_AlertCircle_Bullet", &m_iShaderPass, 8))
            pCurrentEffect->Set_ShaderPass(m_iShaderPass);
        if (ImGui::RadioButton("Tartaglia_Bullet_Ring", &m_iShaderPass, 9))
            pCurrentEffect->Set_ShaderPass(m_iShaderPass);
        if (ImGui::RadioButton("Tartaglia_Bullet_Ring_Background", &m_iShaderPass, 10))
            pCurrentEffect->Set_ShaderPass(m_iShaderPass);
        if (ImGui::RadioButton("Tartaglia_Lightning_Red", &m_iShaderPass, 11))
            pCurrentEffect->Set_ShaderPass(m_iShaderPass);
        if (ImGui::RadioButton("Tartaglia_Lightning_Green", &m_iShaderPass, 12))
            pCurrentEffect->Set_ShaderPass(m_iShaderPass);
        if (ImGui::RadioButton("Water_Color", &m_iShaderPass, 13))
            pCurrentEffect->Set_ShaderPass(m_iShaderPass);
    }
    else if (iEffectType == CEffect_Manager::EFFECT_MESH)
    {
        if (ImGui::RadioButton("UV_TEST", &m_iShaderPass, 0))
            pCurrentEffect->Set_ShaderPass(m_iShaderPass);
    }
    else if (iEffectType == CEffect_Manager::EFFECT_TRAIL)
    {
        if (ImGui::RadioButton("Default_Trail", &m_iShaderPass, 0))
            pCurrentEffect->Set_ShaderPass(m_iShaderPass);
        if (ImGui::RadioButton("Water_Trail", &m_iShaderPass, 1))
            pCurrentEffect->Set_ShaderPass(m_iShaderPass);
        if (ImGui::RadioButton("Water_Ring_Trail", &m_iShaderPass, 2))
            pCurrentEffect->Set_ShaderPass(m_iShaderPass);
        if (ImGui::RadioButton("Non_Mask", &m_iShaderPass, 3))
            pCurrentEffect->Set_ShaderPass(m_iShaderPass);
        if (ImGui::RadioButton("Tighnari_Elemenetal", &m_iShaderPass, 4))
            pCurrentEffect->Set_ShaderPass(m_iShaderPass);
        if (ImGui::RadioButton("Water_Bullet", &m_iShaderPass, 5))
            pCurrentEffect->Set_ShaderPass(m_iShaderPass);
        if (ImGui::RadioButton("Tartaglia_Wave_Bullet", &m_iShaderPass, 6))
            pCurrentEffect->Set_ShaderPass(m_iShaderPass);
        if (ImGui::RadioButton("Tartaglia_Lightning_Ring", &m_iShaderPass, 7))
            pCurrentEffect->Set_ShaderPass(m_iShaderPass);
        if (ImGui::RadioButton("Tartaglia_Sweep_Trail", &m_iShaderPass, 8))
            pCurrentEffect->Set_ShaderPass(m_iShaderPass);
        if (ImGui::RadioButton("Tartaglia_Extra_Trail", &m_iShaderPass, 9))
            pCurrentEffect->Set_ShaderPass(m_iShaderPass);
    }

    ImGui::End();
}

void CEffectTool_Manager::Window_Model()
{
    ImGui::Begin("Model");

    // Model Deata ����
    if (ImGui::Button("Open ModelDataFile")) {
        IGFD::FileDialogConfig config;
        config.path = "../../Data/";
        ImGuiFileDialog::Instance()->OpenDialog("ChooseModelDataFileDlgKey", "Choose File", ".dat", config);
    }

    // display
    if (ImGuiFileDialog::Instance()->Display("ChooseModelDataFileDlgKey")) {
        if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
            m_strModelDataFilePath = ImGuiFileDialog::Instance()->GetFilePathName();
            string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
            // action
        }

        // close
        ImGuiFileDialog::Instance()->Close();
    }


    // Model ����
    if (ImGui::Button("Open ModelFile")) {
        IGFD::FileDialogConfig config;
        config.path = "../../Client/Bin/Resources/Models/";
        ImGuiFileDialog::Instance()->OpenDialog("ChooseModelFileDlgKey", "Choose File", ".png, .jpg, .dds", config);
    }

    // display
    if (ImGuiFileDialog::Instance()->Display("ChooseModelFileDlgKey")) {
        if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
            m_strModelFilePath = ImGuiFileDialog::Instance()->GetFilePathName();
            string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
            // action
        }

        // close
        ImGuiFileDialog::Instance()->Close();
    }

    if (ImGui::Button("Create Model"))
    {
        CEffect_Anim_Model::EFFECT_ANIM_MODEL Desc{};
        Desc.strModelDataFilePath = m_strModelDataFilePath;
        Desc.strModelFilePath = m_strModelFilePath;

        if (FAILED(m_pEffectManager->Create_Model(&Desc)))
        {
            MessageBoxW(g_hWnd, L"�� ���� ����", 0, 0);
        }
    }

    CEffect_Anim_Model* pModel = m_pEffectManager->Get_Model();
    if (pModel == nullptr)
    {
        ImGui::End();

        return;
    }

    if (ImGui::InputInt("Anim Index", &m_iAnimIndex))
    {
        pModel->Set_ModelAnimIndex(m_iAnimIndex);
    }

    ImGui::End();
}

void CEffectTool_Manager::Guizmo()
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
    _matrix viewInverse = m_pGameInstance->Get_Transform_Inverse_Matrix(CPipeLine::D3DTS_VIEW); // ī�޶� ���� 
    _matrix cameraProjection = m_pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_PROJ);
    _float viewMatrix[16], projMatrix[16], viewInverseMatrix[16];

    memcpy(&viewMatrix, &cameraView, sizeof(_matrix));
    memcpy(&viewInverseMatrix, &viewInverse, sizeof(_matrix));
    memcpy(&projMatrix, &cameraProjection, sizeof(_matrix));

    //ImGuizmo::DrawGrid(viewMatrix, projMatrix, identityMatrix, 100.f);

    if (m_iPatchIndex != -1)
        ImGuizmo::Manipulate(viewMatrix, projMatrix, mCurrentGizmoOperation, mCurrentGizmoMode, (_float*)&m_EffectMatrix, NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);

    memcpy(&cameraView, &viewInverseMatrix, sizeof(_float) * 16);
    m_pGameInstance->Set_Transform(CPipeLine::D3DTS_VIEW, XMMatrixInverse(nullptr, cameraView));
}

void CEffectTool_Manager::Effect_Save() // ���� ����
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
        ImGui::Checkbox("Create New File ? ", &m_isNewFile);
        ImGui::InputText("New File", m_szNewFileName, MAX_PATH);

        if (ImGui::Button("Save"))
        {
            // �����ϴ� �Լ� ȣ�� & �Һ��� ȣ��
            if (m_isNewFile)
                m_pEffectManager->Save(m_szNewFileName);
            else
                m_pEffectManager->Save(m_FileName[m_iSaveFileIndex].c_str());

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

void CEffectTool_Manager::Effect_Load() // ���� �ε�
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
            // �ҷ����� �Լ� ȣ�� & �Һ��� ȣ��
            m_pEffectManager->Load(m_FileName[m_iLoadFileIndex].c_str());
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

void CEffectTool_Manager::Bind_FileName()
{
    m_FileName.clear();
    // _finddata_t : <io.h>���� �����ϸ� ���� ������ �����ϴ� ����ü
    _wfinddata64_t fd;
    __int64 handle = _wfindfirst64(L"../../Data/Effect/*.*", &fd);
    if (handle == -1 || handle == 0)
        return;

    // _findfirst : <io.h>���� �����ϸ� ����ڰ� ������ ��� ������ ���� ù ��° ������ ã�� �Լ�

    int iResult = 0;

    char szCurPath[128] = "../Data/Effect/";
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

void CEffectTool_Manager::Release_Tool()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	CEffectTool_Manager::DestroyInstance();
}

void CEffectTool_Manager::Free()
{
    Safe_Release(m_pEffectManager);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
}
