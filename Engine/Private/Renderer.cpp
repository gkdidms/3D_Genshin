#include "Renderer.h"

#include "GameInstance.h"
#include "GameObject.h"

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{pDevice}, m_pContext{pContext},
	m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CRenderer::Initialize()
{
	D3D11_VIEWPORT ViewPort{};
	_uint iNumViewPort = 1;

	m_pContext->RSGetViewports(&iNumViewPort, &ViewPort);

	/*Target_Diffuse*/
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Diffuse"), ViewPort.Width, ViewPort.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(1.f, 0.f, 1.f, 0.f))))
		return E_FAIL;

	/*Target_Normal*/
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Normal"), ViewPort.Width, ViewPort.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	/*Target_Depth*/
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Depth"), ViewPort.Width, ViewPort.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	/*Target_Outline*/
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Outline"), ViewPort.Width, ViewPort.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	/*Target_Shader*/
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Shade"), ViewPort.Width, ViewPort.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	/*Target_Bloom*/
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_NonLightDiffuse"), ViewPort.Width, ViewPort.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	/*MRT_GameObjects*/
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Depth"))))
		return E_FAIL;

	/*MRT_LightAcc*/
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Outline"))))
		return E_FAIL;

	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"), VXTPOSTEX::Elements, VXTPOSTEX::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

	/* ȭ���� �� ä���ֱ� ���� ���庯ȯ���. */
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(ViewPort.Width, ViewPort.Height, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());

	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewPort.Width, ViewPort.Height, 0.f, 1.f));

#ifdef _DEBUG
	if (FAILED(m_pGameInstance->Ready_Debug(TEXT("Target_Diffuse"), 100.f, 100.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_Debug(TEXT("Target_Normal"), 100.f, 300.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_Debug(TEXT("Target_Depth"), 100.f, 500.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_Debug(TEXT("Target_Outline"), 300.f, 300.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_Debug(TEXT("Target_Shade"), 300.f, 100.f, 200.f, 200.f)))
		return E_FAIL;
#endif // _DEBUG


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
	Render_LightAcc();
	Render_DeferredResult();

	Render_NonLight();
	Render_Blender();
	Render_UI();

#ifdef _DEBUG
	Render_Debug();
#endif // _DEBUG

}

HRESULT CRenderer::Add_DebugComponent(CComponent* pComponent)
{
	m_DebugComponents.push_back(pComponent);

	Safe_AddRef(pComponent);

	return S_OK;
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
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_GameObjects"))))
		return;

	for (auto& iter : m_Renderers[RENDER_NONBLENDER])
	{
		iter->Render();

		Safe_Release(iter);
	}
	m_Renderers[RENDER_NONBLENDER].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return;
}

void CRenderer::Render_LightAcc()
{
	
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_LightAcc"))))
		return;

	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return;

	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrixInv", m_pGameInstance->Get_Transform_Inverse_Float4x4(CPipeLine::D3DTS_VIEW))))
		return;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrixInv", m_pGameInstance->Get_Transform_Inverse_Float4x4(CPipeLine::D3DTS_PROJ))))
		return;
	if (FAILED(m_pShader->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition_Float4(), sizeof(_float4))))
		return;
	if (FAILED(m_pShader->Bind_RawValue("g_vCamDir", m_pGameInstance->Get_ComLook_Float4(), sizeof(_float4))))
		return;
	if (FAILED(m_pShader->Bind_RawValue("g_fFar", m_pGameInstance->Get_CamFar(), sizeof(_float))))
		return;


	if (FAILED(m_pGameInstance->Bind_RenderTargetSRV(TEXT("Target_Normal"), m_pShader, "g_NormalTexture")))
		return;
	if (FAILED(m_pGameInstance->Bind_RenderTargetSRV(TEXT("Target_Depth"), m_pShader, "g_DepthTexture")))
		return;

	m_pGameInstance->Render_Lights(m_pShader, m_pVIBuffer);

	if (FAILED(m_pGameInstance->End_MRT()))
		return;
}

void CRenderer::Render_DeferredResult() // ����ۿ� Diffuse�� Shade�� ���ؼ� �׷���
{
	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return;

	if (FAILED(m_pGameInstance->Bind_RenderTargetSRV(TEXT("Target_Diffuse"), m_pShader, "g_DiffuseTexture")))
		return;
	if (FAILED(m_pGameInstance->Bind_RenderTargetSRV(TEXT("Target_Shade"), m_pShader, "g_ShadeTexture")))
		return;
	if (FAILED(m_pGameInstance->Bind_RenderTargetSRV(TEXT("Target_Outline"), m_pShader, "g_OutlineTexture")))
		return;

	m_pShader->Begin(3);

	m_pVIBuffer->Render();
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

#ifdef _DEBUG
void CRenderer::Render_Debug()
{
	//for (auto& pDebugCom : m_DebugComponents)
	//{
	//	pDebugCom->Render();
	//	Safe_Release(pDebugCom);
	//}
	//m_DebugComponents.clear();

	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return;

	if (FAILED(m_pGameInstance->Render_Debug(TEXT("MRT_GameObjects"), m_pShader, m_pVIBuffer)))
		return;
	if (FAILED(m_pGameInstance->Render_Debug(TEXT("MRT_LightAcc"), m_pShader, m_pVIBuffer)))
		return;
}
#endif // DEBUG



CRenderer* CRenderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRenderer* pInstance = new CRenderer(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
		Safe_Release(pInstance);

	return pInstance;
}

void CRenderer::Free()
{

#ifdef _DEBUG
	for (auto& iter : m_DebugComponents)
		Safe_Release(iter);
	m_DebugComponents.clear();
#endif // _DEBUG

	for (int i = 0; i < RENDER_END; ++i)
	{
		for (auto& iter : m_Renderers[i])
			Safe_Release(iter);

		m_Renderers[i].clear();
	}

	Safe_Release(m_pShader);
	Safe_Release(m_pVIBuffer);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
}
