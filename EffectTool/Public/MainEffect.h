#pragma once
#include "Base.h"

#include "EffectTool_Manager.h"

BEGIN(Effect)
class CMainEffect :
    public CBase
{
private:
    CMainEffect();
    virtual ~CMainEffect() = default;

public:
    HRESULT Initialize();
    void Tick(const _float& fTimeDelta);
    void Render();

private:
    ID3D11Device* m_pDevice = { nullptr };
    ID3D11DeviceContext* m_pContext = { nullptr };
    CGameInstance* m_pGameInstance = { nullptr };
    CEffectTool_Manager* m_pEffectTool_Manager = { nullptr };
    class CEffect_Manager* m_pEffectManager = { nullptr };
private:
    HRESULT Open_Level(LEVEL_STATE eLevelState);

    HRESULT Loading_Components();
    HRESULT Loading_GameObjects();

public:
    static CMainEffect* Create();
    virtual void Free() override;
};
END
