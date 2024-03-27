#pragma once
#include "Base.h"

BEGIN(Engine)
class CRenderer :
    public CBase
{

public:
    enum RENDERER_STATE { RENDER_PRIORITY, RENDER_NONBLENDER, RENDER_NONLIGHT, RENDER_BLENDER, RENDER_UI, RENDER_END };

private:
    CRenderer();
    virtual ~CRenderer() = default;

public:
    HRESULT Initialize(_uint iMaxLevelIndex);
    void Add_Renderer(RENDERER_STATE eRenderState, class CGameObject* pGameObject);
    void Draw();

private:
    void Render_Priority();
    void Render_NonBlender();
    void Render_NonLight();
    void Render_Blender();
    void Render_UI();

private:
    list<class CGameObject*>* m_Renderers;

    _uint m_iMaxLevelIndex = { 0 };

public:
    static CRenderer* Create(_uint iMaxLevelIndex);
    virtual void Free();
};
END

