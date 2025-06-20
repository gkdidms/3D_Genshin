#include "PlayerHp_Outline.h"

#include "MainApp.h"
#include "GameInstance.h"

#include "Player.h"
#include "PlayerHP.h"

CPlayerHP_Outline::CPlayerHP_Outline(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI{ pDevice, pContext}
{
}

CPlayerHP_Outline::CPlayerHP_Outline(const CPlayerHP_Outline& rhs)
	: CUI { rhs }
{
}

HRESULT CPlayerHP_Outline::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayerHP_Outline::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_fSizeX = 256.f;
	m_fSizeY = 8.f;
	m_fX = g_iWinSizeX >> 1;
	m_fY = g_iWinSizeY >> 1;

	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, m_fY - g_iWinSizeY + m_fSizeY * 0.5f + 50.f, 0.1f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.0f));

	CPlayerHP::PLAYER_HP_DESC tPlayerHPDESC{};
	XMStoreFloat4x4(&tPlayerHPDESC.pParentMatrix, m_pTransformCom->Get_WorldMatrix());
	m_pPlayerHP = dynamic_cast<CPlayerHP*>(m_pGameInstance->Clone_Object(L"Prototype_GameObject_UI_PlayerHP", &tPlayerHPDESC));
	if (nullptr == m_pPlayerHP)
		return E_FAIL;

	return S_OK;
}

void CPlayerHP_Outline::Priority_Tick(const _float& fTimeDelta)
{
}

void CPlayerHP_Outline::Tick(const _float& fTimeDelta)
{
}

void CPlayerHP_Outline::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_UI, this);
}

HRESULT CPlayerHP_Outline::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(m_pPlayerHP->Render()))
		return E_FAIL;

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(CMainApp::g_iCurrentLevel, TEXT("Layer_Player"), 0));
	_tchar strText[MAX_PATH]/* = to_wstring(_int(pPlayer->Get_PlayerbleMaxHP())) + L" \ " + to_wstring(_int(pPlayer->Get_PlayerbleHP()))*/;
	wsprintf(strText, TEXT("%d / %d"), _int(pPlayer->Get_PlayerbleMaxHP()), _int(pPlayer->Get_PlayerbleHP()));
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	
	_float2 vFontPos = _float2(g_iWinSizeX * 0.5f - 40.f, g_iWinSizeY - 60.f);
	m_pGameInstance->Render_Font(L"Font_SDK_SC_20", strText, vFontPos, XMVectorSet(1.f, 1.f, 1.f, 1.f));

	return S_OK;
}

HRESULT CPlayerHP_Outline::Add_Components()
{

	if (FAILED(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(CMainApp::g_iCurrentLevel, L"Prototype_Component_Shader_VtxPosTex_UI", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(CMainApp::g_iCurrentLevel, L"Prototype_Component_Texture_UI_Avatar_HP_Outline", L"Com_Texture", reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CPlayerHP_Outline::Bind_ResourceData()
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

CPlayerHP_Outline* CPlayerHP_Outline::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayerHP_Outline* pInstance = new CPlayerHP_Outline(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CPlayerHP_Outline::Clone(void* pArg)
{
	CPlayerHP_Outline* pInstane = new CPlayerHP_Outline(*this);

	if (FAILED(pInstane->Initialize(pArg)))
		Safe_Release(pInstane);

	return pInstane;
}

void CPlayerHP_Outline::Free()
{
	__super::Free();

	Safe_Release(m_pPlayerHP);
}
