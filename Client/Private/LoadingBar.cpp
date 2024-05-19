#include "LoadingBar.h"

#include "GameInstance.h"

CLoadingBar::CLoadingBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CLoadingBar::CLoadingBar(const CLoadingBar& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CLoadingBar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLoadingBar::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_fSizeX = 650.f;
	m_fSizeY = 78.f;
	m_fX = g_iWinSizeX >> 1;
	m_fY = g_iWinSizeY >> 1;

	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));

	XMStoreFloat4x4(&m_matView, XMMatrixIdentity());
	XMStoreFloat4x4(&m_matProj, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.0f));

	return S_OK;
}

void CLoadingBar::Priority_Tick(const _float& fTimeDelta)
{
}

void CLoadingBar::Tick(const _float& fTimeDelta)
{
}

void CLoadingBar::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CLoadingBar::Render()
{
	if (FAILED(Bind_Render()))
		return E_FAIL;

	m_pShaderCom->Begin(1);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CLoadingBar::Add_Components()
{
	m_pVIBufferCom = dynamic_cast<CVIBuffer_Rect*>(m_pGameInstance->Add_Component_Clone(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", nullptr));
	if (nullptr == m_pVIBufferCom)
		return E_FAIL;

	m_pShaderCom = dynamic_cast<CShader*>(m_pGameInstance->Add_Component_Clone(LEVEL_STATIC, L"Prototype_Component_Shader", nullptr));
	if (nullptr == m_pShaderCom)
		return E_FAIL;

	m_pTextureCom = dynamic_cast<CTexture*>(m_pGameInstance->Add_Component_Clone(LEVEL_STATIC, L"Prototype_Component_Texture_LoadingBar", nullptr));
	if (nullptr == m_pTextureCom)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoadingBar::Bind_Render()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_matView)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_matProj)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

CLoadingBar* CLoadingBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLoadingBar* pInstance = new CLoadingBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CLoadingBar::Clone(void* pArg)
{
	CLoadingBar* pInstance = new CLoadingBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CLoadingBar::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
}
