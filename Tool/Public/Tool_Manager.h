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

    static int item_current_idx;

public:
    HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    void Tick(const _float& fTimeDelta);
    void Render();

private:
    void Window_Terrain();
    void Window_Object();

private:
    ID3D11Device* m_pDevice = { nullptr };
    ID3D11DeviceContext* m_pContext = { nullptr };

public:
    void Release_Tool();
    virtual void Free(); 
};

END