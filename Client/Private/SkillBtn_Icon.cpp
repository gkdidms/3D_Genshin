#include "SkillBtn_Icon.h"

#include "GameInstance.h"

CSkillBtn_Icon::CSkillBtn_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI{ pDevice, pContext }
{
}

CSkillBtn_Icon::CSkillBtn_Icon(const CSkillBtn_Icon& rhs)
	: CUI{ rhs }
{
}

HRESULT CSkillBtn_Icon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkillBtn_Icon::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	SKILLBTN_ICON_DESC* pDesc = static_cast<SKILLBTN_ICON_DESC*>(pArg);
	m_strComponentPrototypeTag = pDesc->strComponentPrototypeTag;
	m_ParentMatrix = pDesc->ParentMatrix;
	m_fSizeX = pDesc->fSizeX;
	m_fSizeX = pDesc->fSizeY;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(&m_ParentMatrix));

	return S_OK;
}

void CSkillBtn_Icon::Priority_Tick(const _float& fTimeDelta)
{
}

void CSkillBtn_Icon::Tick(const _float& fTimeDelta)
{
}

void CSkillBtn_Icon::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CSkillBtn_Icon::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CSkillBtn_Icon::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxPosTex_UI", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, m_strComponentPrototypeTag, L"Com_Texture", reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkillBtn_Icon::Bind_ResourceData()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_matView)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_matProj)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;
}

CSkillBtn_Icon* CSkillBtn_Icon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSkillBtn_Icon* pInstance = new CSkillBtn_Icon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CSkillBtn_Icon::Clone(void* pArg)
{
	CSkillBtn_Icon* pInstance = new CSkillBtn_Icon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CSkillBtn_Icon::Free()
{
	__super::Free();
}
