#pragma once
#include "Base.h"

#include "Effect_Default.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Effect)
class CEffect_Manager :
    public CBase
{

    DECLARE_SINGLETON(CEffect_Manager)

public:
    enum EFFECT_TYPE { PARTICLE, EFFECT_IMG, EFFECT_MESH, EFFECT_TRAIL, EFFECT_BURST, EFFECT_END };

private:
    CEffect_Manager();
    virtual ~CEffect_Manager() = default;

public:
    vector<CTool_Effect*> Get_Effects() const { return m_Effects; }

public:
    HRESULT Initialize();
    void Tick(const _float& fTimeDelta);
    HRESULT Add_Effect(EFFECT_TYPE eType, void* pArg = nullptr, _uint iShaderPass = 0);
    void Remove_Effect(_int iIndex);
    
    HRESULT Save(const _char* szFileName);
    HRESULT Load(const _char* szFileName);

private:
    vector<CTool_Effect*> m_Effects;
    CGameInstance* m_pGameInstance = { nullptr };

private:
    void Remove_All();
public:
    void Release_EffectManager();
    virtual void Free() override;
};
END
