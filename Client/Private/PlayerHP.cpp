#include "PlayerHP.h"

#include "GameInstance.h"

CPlayerHP::CPlayerHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI{ pDevice, pContext}
{
}

HRESULT CPlayerHP::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayerHP::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		PLAYER_HP_DESC* pDesc = static_cast<PLAYER_HP_DESC*>(pArg);
		m_pParentMatrix = pDesc->pParentMatrix;
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_fSizeX = 510.f;
	m_fSizeY = 15.f;
	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4x4(&m_pParentMatrix).r[3]);

	XMStoreFloat4x4(&m_matView, XMMatrixIdentity());
	XMStoreFloat4x4(&m_matProj, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.0f));



	return S_OK;
}

void CPlayerHP::Priority_Tick(const _float& fTimeDelta)
{
}

void CPlayerHP::Tick(const _float& fTimeDelta)
{
}

void CPlayerHP::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CPlayerHP::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	m_pShaderCom->Begin(1);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CPlayerHP::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxPosTex_UI", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_Avatar_HP", L"Com_Texture", reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayerHP::Bind_ResourceData()
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

CPlayerHP* CPlayerHP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayerHP* pInstance = new CPlayerHP(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CPlayerHP::Clone(void* pArg)
{
	CPlayerHP* pInstance = new CPlayerHP(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPlayerHP::Free()
{
	__super::Free();
}
