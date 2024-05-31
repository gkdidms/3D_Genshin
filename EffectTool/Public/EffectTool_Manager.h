#pragma once
#include "Base.h"

#include "GameInstance.h"

#include "EffectTool_Defines.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "ImGuizmo.h"
#include "ImSequencer.h"

#include "ImGuiFileDialog.h"

BEGIN(Effect)
class CEffectTool_Manager :
    public CBase
{
    DECLARE_SINGLETON(CEffectTool_Manager)

private:
    CEffectTool_Manager();
    virtual ~CEffectTool_Manager() = default;

public:
    HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    void Tick(const _float& fTimeDelta);
    void Render();

private:
    void Window_Effect();
    void Window_MainBar();
    void Window_EffectPatch();

private:
    void Effect_Save();
    void Effect_Load();
    void Bind_FileName();

    void Guizmo();

private:
    ID3D11Device* m_pDevice = { nullptr };
    ID3D11DeviceContext* m_pContext = { nullptr };
    CGameInstance* m_pGameInstance = { nullptr };
    class CEffect_Manager* m_pEffectManager = { nullptr };

private:
    string m_strTextureFilePath = { "" };
    string m_strMeshFilePath = { "" };
    string m_strMaskFilePath = { "" };
    string m_strNoiseFilePath = { "" };
    _bool m_isMask = { false };
    _bool m_isNoise = { false };

    vector<string> m_FileName;
    _bool m_isShowLoadModal = { false };
    _bool m_isShowSaveModal = { false };
    _bool m_isNewFile = { false };
    _char m_szNewFileName[MAX_PATH] = {};

    _int m_iSaveFileIndex = { 0 };
    _int m_iLoadFileIndex = { 0 };
    _int m_iEffectType = { 0 };
    _int m_iTextureNum = { 0 };

private: // Patch
    _int m_iPatchIndex = { -1 };
    _float3 m_vPatchPos = {};
    _float3 m_vPatchScale = {};
    _float m_fPatchRotation = { 0.f };

    _float4x4 m_EffectMatrix = {};

    ImGuizmo::OPERATION mCurrentGizmoOperation = { ImGuizmo::TRANSLATE };
    ImGuizmo::MODE mCurrentGizmoMode = { ImGuizmo::WORLD };

private:
    _int m_ParticleType = { 0 };
    _int m_iNumInstance = { 0 };
    _float3 m_vOffsetPos = {};
    _float3 m_vPivotPos = {};
    _float3 m_vRange = {};
    _float2 m_vSize = {};
    _float2 m_vSpeed = {};
    _float2 m_vPower = {};
    _float2 m_vLifeTime = {};
    _bool m_isLoop = { false };

private:
    _bool m_isTrailBuffer = {false};
    _float m_fTextureSpeed = { 0.f };
    _float m_fRotateSpeed = { 45.f };

    _float2 m_vUV = {};
    _float4 m_vColor = {};

    _int m_iShaderPass = { 0 };

public:
    void Release_Tool();
    virtual void Free();
};
END
