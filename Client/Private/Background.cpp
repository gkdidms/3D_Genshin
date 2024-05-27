#include "Background.h"

#include "GameInstance.h"

#include "LoadingBar.h"

CBackground::CBackground(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{pDevice, pContext}
{
}

CBackground::CBackground(const CBackground& rhs)
	: CGameObject{rhs}
{
}

HRESULT CBackground::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBackground::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		BACKGROUND_DESC* pDesc = static_cast<BACKGROUND_DESC*>(pArg);
		m_pProgress = pDesc->pProgress;
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_fSizeX = g_iWinSizeX;
	m_fSizeY = g_iWinSizeY;
	m_fX = g_iWinSizeX >> 1;
	m_fY = g_iWinSizeY >> 1;

	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.9f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.0f));

	return S_OK;
}

void CBackground::Priority_Tick(const _float& fTimeDelta)
{
}

void CBackground::Tick(const _float& fTimeDelta)
{
}

void CBackground::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_UI, this);
}

HRESULT CBackground::Render()
{
	if (FAILED(Bind_Render()))
		return E_FAIL;

	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(m_pLoadingBar->Render()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CBackground::Add_Components()
{
	m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(m_pGameInstance->Add_Component_Clone(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", nullptr));
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pShaderCom = dynamic_cast<CShader*>(m_pGameInstance->Add_Component_Clone(LEVEL_STATIC, L"Prototype_Component_Shader", nullptr));
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	m_pTextureCom = dynamic_cast<CTexture*>(m_pGameInstance->Add_Component_Clone(LEVEL_STATIC, L"Prototype_Component_Texture_Loading", nullptr));
	if (nullptr == m_pTextureCom)
		return E_FAIL;

	CLoadingBar::LOADING_BACK_DESC Desc{};
	Desc.pProgress = m_pProgress;

	m_pLoadingBar = dynamic_cast<CLoadingBar*>(m_pGameInstance->Clone_Object(L"Prototype_GameObject_LoadingBar", &Desc));
	if (nullptr == m_pLoadingBar)
		return E_FAIL;

	return S_OK;
}

HRESULT CBackground::Bind_Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

CBackground* CBackground::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBackground* pInstance = new CBackground(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CBackground::Clone(void* pArg)
{
	CBackground* pInstance = new CBackground(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CBackground::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pLoadingBar);
}
