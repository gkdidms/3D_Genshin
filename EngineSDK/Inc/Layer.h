#pragma once
#include "Base.h"

BEGIN(Engine)
class CLayer :
    public CBase
{
private:
    CLayer();
    virtual ~CLayer() = default;

public:
    HRESULT Initialize();
    void Priority_Tick(const _float& fTimeDelta);
    void Tick(const _float& fTimeDelta);
    void Late_Tick(const _float& fTimeDelta);
    void Render();

public:
    HRESULT Add_GameObject(class CGameObject* pObject);

private:
    vector<class CGameObject*> m_vecObjects;

public:
    static CLayer* Create();
    virtual void Free();
};
END
