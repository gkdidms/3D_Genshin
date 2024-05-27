#include "Tartaglia_DualBlade.h"

#include "Boss.h"

CTartaglia_DualBlade::CTartaglia_DualBlade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CWeapon{pDevice, pContext }
{
}

CTartaglia_DualBlade::CTartaglia_DualBlade(const CTartaglia_DualBlade& rhs)
	: CWeapon{ rhs }
{
}

HRESULT CTartaglia_DualBlade::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTartaglia_DualBlade::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CTartaglia_DualBlade::Priority_Tick(const _float& fTimeDelta)
{
}

void CTartaglia_DualBlade::Tick(const _float& fTimeDelta)
{
}

void CTartaglia_DualBlade::Late_Tick(const _float& fTimeDelta)
{
	_matrix		SocketMatrix = XMMatrixIdentity();

	m_isHide = !(*m_pState == CBoss::BOSS_DUALBLADE_HIRAISHIN_BS || *m_pState == CBoss::BOSS_DUALBLADE_HIRAISHIN_AS || *m_pState == CBoss::BOSS_DUALBLADE_HIRAISHIN_LOOP
		|| *m_pState == CBoss::BOSS_DUALBLADE_STRIKE_ATTACK_AS || *m_pState == CBoss::BOSS_DUALBLADE_STRIKE_ATTACK_BS || *m_pState == CBoss::BOSS_DUALBLADE_STRIKE_ATTACK_LOOP 
		|| *m_pState == CBoss::BOSS_DUALBLADE_SWEEP_ATTACK_L || *m_pState == CBoss::BOSS_DUALBLADE_SWEEP_ATTACK_R);
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

HRESULT CTartaglia_DualBlade::Render()
{
	if (m_isHide)
		return S_OK;

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTartaglia_DualBlade::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Tartaglia_DualBlade", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CTartaglia_DualBlade::Bind_ResourceData()
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

void CTartaglia_DualBlade::Change_Animation(const _float& fTimeDelta)
{
}

CTartaglia_DualBlade* CTartaglia_DualBlade::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTartaglia_DualBlade* pInstance = new CTartaglia_DualBlade(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CTartaglia_DualBlade::Clone(void* pArg)
{
	CTartaglia_DualBlade* pInstance = new CTartaglia_DualBlade(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CTartaglia_DualBlade::Free()
{
	__super::Free();
}
