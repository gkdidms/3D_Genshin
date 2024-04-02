#pragma once

#include "Renderer.h"
#include "Component_Manager.h"
#include "PipeLine.h"
#include "Input_Device.h"

BEGIN(Engine)
class ENGINE_DLL CGameInstance :
    public CBase
{
    DECLARE_SINGLETON(CGameInstance)

private:
    CGameInstance();
    virtual ~CGameInstance() = default;

public:
    HRESULT Initialize_Engine(_uint iMaxLevelIndex, const ENGINE_DESC& Engine_Desc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext);
    void Tick(const _float& fTimeDelta);
    void Draw(const _float4 vClearColor);
    void Draw_From_Tool();

    // 그래픽
public:
    HRESULT Clear_BackBuffer_View(_float4 vClearColor);
    HRESULT Clear_DepthStencil_View();
    HRESULT Present();
    
    // 레벨 매니저
public:
    HRESULT Open_Level(_uint iLevelIndex, class CLevel* pLevel);

public: // 게임오브젝트 매니저
    HRESULT Add_GameObject_Prototype(const wstring strGameObjectTag, class CGameObject* pGameObject);
    HRESULT Add_GameObject(_uint iLevelIndex, const wstring strGameObjectTag, const wstring strLayerTag, void* pArg = nullptr);
    void Clear_Object(_uint iLevelIndex);
    vector<class CGameObject*> Find_GameObjects_Clone(_uint iLevelIndex, const wstring strLayerTag);

public: // 렌더러
    void Add_Renderer(CRenderer::RENDERER_STATE eRenderState, class CGameObject* pGameObject);

public: // 컴포넌트 매니저
    HRESULT Add_Component_Prototype(_uint iLevelIndex, const wstring strComponentTag, CComponent* pComponent);
    CComponent* Add_Component_Clone(_uint iLevelIndex, const wstring strComponentTag, void* pArg = nullptr);

public: // 파이프라인
    const _float4x4* Get_Transform_Float4x4(CPipeLine::D3DTRANSFORMSTATE eState);
    _matrix Get_Transform_Matrix(CPipeLine::D3DTRANSFORMSTATE eState);
    const _float4x4* Get_Transform_Inverse_Float4x4(CPipeLine::D3DTRANSFORMSTATE eState);
    _matrix Get_Transform_Inverse_Matrix(CPipeLine::D3DTRANSFORMSTATE eState);
    const _float4* Get_CamPosition_Float4();
    _vector Get_CamPosition();
    void Set_Transform(CPipeLine::D3DTRANSFORMSTATE eState, _fmatrix matTransform);

public: // 타이머
    _float		Get_TimeDelta(const _tchar* pTimerTag);
    void		Update_TimeDelta(const _tchar* pTimerTag);
    HRESULT		Ready_Timer(const _tchar* pTimerTag);

public: // 키보드 & 마우스
    void Get_ProcessMessageKeyboard(UINT message, WPARAM wParam, LPARAM lParam);
    void Get_ProcessMessageMouse(UINT message, WPARAM wParam, LPARAM lParam);
    _int Get_MouseX();
    _int Get_MouseY();

    _bool Is_KeyPressed(Keyboard::Keys eKeys);
    _bool Is_MouseLeftPressed();

public: //Input_Device
    _byte	Get_DIKeyState(_ubyte byKeyID);
    _byte	Get_DIMouseState(MOUSEKEYSTATE eMouse);
    _long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState);
    CInput_Device::eKEY_STATE GetKeyState(_ubyte byKeyID);
    CInput_Device::eKEY_STATE GetMouseState(MOUSEKEYSTATE eMouseState);

private:
    class CGraphic_Device* m_pGraphic_Device = { nullptr };
    class CInput_Manager* m_pInput_Manager = { nullptr };
    class CInput_Device* m_pInput_Device = { nullptr };
    class CLevel_Manager* m_pLevel_Manager = { nullptr };
    class CGameObject_Manager* m_pGameObject_Manager = { nullptr };
    class CComponent_Manager* m_pComponent_Manager = { nullptr };
    class CTimer_Manager* m_pTimer_Manager = { nullptr };
    class CRenderer* m_pRenderer = { nullptr };
    class CPipeLine* m_pPipeLine = { nullptr };

public:
    void Release_Engine();
    virtual void Free() override;
};

END