#pragma once

#include "Level.h"

#include "EffectTool_Defines.h"
BEGIN(Effect)
class CLevel_Tool :
    public CLevel
{
private:
    CLevel_Tool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CLevel_Tool() = default;

public:
    virtual HRESULT Initialize() override;
    virtual void Tick(const _float& fTimeDelta);
    virtual void Render();

private:
    HRESULT Ready_Lights();
    HRESULT Ready_Camera(const wstring strLayerTag);
    HRESULT Ready_Terrain(const wstring strLayerTag);
    HRESULT Ready_Effect(const wstring strLayerTag);

private:
    class CEffect_Manager* m_pEffectManager = { nullptr };

public:
    static CLevel_Tool* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual void Free() override;
};
END
