#include "Renderer.h"

#include "GameObject.h"

CRenderer::CRenderer()
{
}

HRESULT CRenderer::Initialize(_uint iMaxLevelIndex)
{
	m_iMaxLevelIndex = iMaxLevelIndex;

	m_Renderers = new list<CGameObject*>[RENDER_END];
	return S_OK;
}

void CRenderer::Add_Renderer(RENDERER_STATE eRenderState,CGameObject* pGameObject)
{
	m_Renderers[eRenderState].push_back(pGameObject);
	Safe_AddRef(pGameObject);
}

void CRenderer::Draw()
{
	Render_Priority();
	Render_NonBlender();
	Render_NonLight();
	Render_Blender();
	Render_UI();
}

void CRenderer::Render_Priority()
{
	for (auto& iter : m_Renderers[RENDER_PRIORITY])
	{
		iter->Render();

		Safe_Release(iter);
	}
	m_Renderers[RENDER_PRIORITY].clear();
		
}

void CRenderer::Render_NonBlender()
{
	for (auto& iter : m_Renderers[RENDER_NONBLENDER])
	{
		iter->Render();

		Safe_Release(iter);
	}
	m_Renderers[RENDER_NONBLENDER].clear();
}

void CRenderer::Render_NonLight()
{
	for (auto& iter : m_Renderers[RENDER_NONLIGHT])
	{
		iter->Render();

		Safe_Release(iter);
	}
	m_Renderers[RENDER_NONLIGHT].clear();
}

void CRenderer::Render_Blender()
{
	for (auto& iter : m_Renderers[RENDER_BLENDER])
	{
		iter->Render();

		Safe_Release(iter);
	}
	m_Renderers[RENDER_BLENDER].clear();
}

void CRenderer::Render_UI()
{
	for (auto& iter : m_Renderers[RENDER_UI])
	{
		iter->Render();

		Safe_Release(iter);
	}
	m_Renderers[RENDER_UI].clear();
}

CRenderer* CRenderer::Create(_uint iMaxLevelIndex)
{
	CRenderer* pInstance = new CRenderer();

	if (FAILED(pInstance->Initialize(iMaxLevelIndex)))
		Safe_Release(pInstance);

	return pInstance;
}

void CRenderer::Free()
{
	for (int i = 0; i < m_iMaxLevelIndex; ++i)
	{
		for (auto& iter : m_Renderers[i])
			Safe_Release(iter);

		m_Renderers[i].clear();
	}
}
