#include "MonsterHP.h"

#include "GameInstance.h"

CMonsterHP::CMonsterHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI{ pDevice, pContext}
{
}

HRESULT CMonsterHP::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonsterHP::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		MONSTER_HP_DESC* pDesc = static_cast<MONSTER_HP_DESC*>(pArg);
		m_pParentMatrix = pDesc->pParentMatrix;
		m_pMonsterHP = pDesc->pMonsterHP;
		m_fMonsterMaxHP = pDesc->fMonsterMaxHP;
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CMonsterHP::Priority_Tick(const _float& fTimeDelta)
{
}

void CMonsterHP::Tick(const _float& fTimeDelta)
{
}

void CMonsterHP::Late_Tick(const _float& fTimeDelta)
{
	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));
}

HRESULT CMonsterHP::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	m_pShaderCom->Begin(2);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CMonsterHP::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxPosTex_UI", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_Monster_HP", L"Com_Texture", reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonsterHP::Bind_ResourceData()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

CMonsterHP* CMonsterHP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonsterHP* pInstance = new CMonsterHP(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CMonsterHP::Clone(void* pArg)
{
	CMonsterHP* pInstance = new CMonsterHP(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CMonsterHP::Free()
{
	__super::Free();
}
