#include "BossHP_Outline.h"

#include "GameInstance.h"

#include "Boss.h"
#include "BossHP.h"

CBossHP_Outline::CBossHP_Outline(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI{ pDevice, pContext}
{
}

CBossHP_Outline::CBossHP_Outline(const CBossHP_Outline& rhs)
	: CUI { rhs }
{
}

HRESULT CBossHP_Outline::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBossHP_Outline::Initialize(void* pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_fSizeX = 512.f;
	m_fSizeY = 8.f;
	m_fX = g_iWinSizeX >> 1;
	m_fY = g_iWinSizeY >> 1;

	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX - g_iWinSizeX * 0.5f, m_fY - 80.f, 0.1f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.0f));

	CBossHP::BOSS_HP_DESC tPlayerHPDESC{};
	XMStoreFloat4x4(&tPlayerHPDESC.pParentMatrix, m_pTransformCom->Get_WorldMatrix());
	m_pBossHP = dynamic_cast<CBossHP*>(m_pGameInstance->Clone_Object(L"Prototype_GameObject_UI_BossHP", &tPlayerHPDESC));
	if (nullptr == m_pBossHP)
		return E_FAIL;

	return S_OK;
}

void CBossHP_Outline::Priority_Tick(const _float& fTimeDelta)
{
}

void CBossHP_Outline::Tick(const _float& fTimeDelta)
{
}

void CBossHP_Outline::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_UI, this);
}

HRESULT CBossHP_Outline::Render()
{
	CBoss* pBoss = dynamic_cast<CBoss*>(m_pGameInstance->Get_GameObject(LEVEL_STAGE_BOSS, TEXT("Layer_Boss"), 0));

	_float2 vFontPos = _float2(g_iWinSizeX * 0.5f - 50.f, 20.f);
	m_pGameInstance->Render_Font(L"Font_Nanum_28", TEXT("타르탈리아"), vFontPos, XMVectorSet(1.f, 1.f, 1.f, 1.f));

	wstring strSubName = L"";
	if (pBoss->Get_Page() == CBoss::ONE_PAGE)
		strSubName = TEXT("우인단 집행관 말석");
	else if (pBoss->Get_Page() == CBoss::TWO_PAGE)
		strSubName = TEXT("사안 개방");
	else
		strSubName = TEXT("");

	vFontPos = _float2(g_iWinSizeX * 0.5f - 70.f, 50.f);
	m_pGameInstance->Render_Font(L"Font_Nanum_24", strSubName, vFontPos, XMVectorSet(1.f, 1.f, 1.f, 0.7f));

	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(m_pBossHP->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBossHP_Outline::Add_Components()
{

	if (FAILED(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STAGE_BOSS, L"Prototype_Component_Shader_VtxPosTex_UI", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STAGE_BOSS, L"Prototype_Component_Texture_UI_Avatar_HP_Outline", L"Com_Texture", reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CBossHP_Outline::Bind_ResourceData()
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

CBossHP_Outline* CBossHP_Outline::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBossHP_Outline* pInstance = new CBossHP_Outline(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CBossHP_Outline::Clone(void* pArg)
{
	CBossHP_Outline* pInstane = new CBossHP_Outline(*this);

	if (FAILED(pInstane->Initialize(pArg)))
		Safe_Release(pInstane);

	return pInstane;
}

void CBossHP_Outline::Free()
{
	__super::Free();

	Safe_Release(m_pBossHP);
}
