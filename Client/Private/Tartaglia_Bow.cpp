#include "Tartaglia_Bow.h"

#include "Boss.h"

CTartaglia_Bow::CTartaglia_Bow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CWeapon{ pDevice, pContext}
{
}

CTartaglia_Bow::CTartaglia_Bow(const CTartaglia_Bow& rhs)
	: CWeapon{rhs }
{
}

HRESULT CTartaglia_Bow::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTartaglia_Bow::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CTartaglia_Bow::Priority_Tick(const _float& fTimeDelta)
{
}

void CTartaglia_Bow::Tick(const _float& fTimeDelta)
{
}

void CTartaglia_Bow::Late_Tick(const _float& fTimeDelta)
{
	_matrix		SocketMatrix = XMMatrixIdentity();

	m_isHide = !(*m_pState == CBoss::BOSS_BOW_COVER_ATTACK_AS || *m_pState == CBoss::BOSS_BOW_COVER_ATTACK_BS || *m_pState == CBoss::BOSS_BOW_COVER_ATTACK_LOOP 
		|| *m_pState == CBoss::BOSS_BOW_NORMAL_ATTACK_BS || *m_pState == CBoss::BOSS_BOW_NORMAL_ATTACK_AS || *m_pState == CBoss::BOSS_BOW_NORMAL_ATTACK_LOOP 
		|| *m_pState == CBoss::BOSS_BOW_COMBO_ATTACK || *m_pState == CBoss::BOSS_BOW_RANGE_ATTACK_BS || *m_pState == CBoss::BOSS_BOW_RANGE_ATTACK_AS || *m_pState == CBoss::BOSS_BOW_RANGE_ATTACK_LOOP_1 || *m_pState == CBoss::BOSS_BOW_RANGE_ATTACK_LOOP_2
		|| *m_pState == CBoss::BOSS_BOW_POWER_ATTACK_BS ||*m_pState == CBoss::BOSS_BOW_POWER_ATTACK_AS ||*m_pState == CBoss::BOSS_BOW_POWER_ATTACK_LOOP);
	if (!m_isHide)
	{
		SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

		SocketMatrix.r[0] = XMVector3Normalize(SocketMatrix.r[0]);
		SocketMatrix.r[1] = XMVector3Normalize(SocketMatrix.r[1]);
		SocketMatrix.r[2] = XMVector3Normalize(SocketMatrix.r[2]);

		XMStoreFloat4x4(&m_pWorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));

		m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
	}
}

HRESULT CTartaglia_Bow::Render()
{
	if (m_isHide)
		return S_OK;

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTartaglia_Bow::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STAGE_BOSS, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STAGE_BOSS, L"Prototype_Component_Model_Tartaglia_Bow", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CTartaglia_Bow::Bind_ResourceData()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fFar", m_pGameInstance->Get_CamFar(), sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

void CTartaglia_Bow::Change_Animation(const _float& fTimeDelta)
{
}

CTartaglia_Bow* CTartaglia_Bow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTartaglia_Bow* pInstance = new CTartaglia_Bow(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CTartaglia_Bow::Clone(void* pArg)
{
	CTartaglia_Bow* pInstance = new CTartaglia_Bow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CTartaglia_Bow::Free()
{
	__super::Free();
}
