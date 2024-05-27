#include "Harbinger_DualBlade.h"

#include "Boss.h"

CHarbinger_DualBlade::CHarbinger_DualBlade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CWeapon{ pDevice, pContext }
{
}

CHarbinger_DualBlade::CHarbinger_DualBlade(const CHarbinger_DualBlade& rhs)
	: CWeapon { rhs }
{
}

HRESULT CHarbinger_DualBlade::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHarbinger_DualBlade::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;
	
	return S_OK;
}

void CHarbinger_DualBlade::Priority_Tick(const _float& fTimeDelta)
{
}

void CHarbinger_DualBlade::Tick(const _float& fTimeDelta)
{
}

void CHarbinger_DualBlade::Late_Tick(const _float& fTimeDelta)
{
	_matrix		SocketMatrix = XMMatrixIdentity();

	m_isHide = !(*m_pState == CBoss::BOSS_DUALBLADE_CYCLE_ATTACK || *m_pState == CBoss::BOSS_DUALBLADE_HIRAISHIN_BS || *m_pState == CBoss::BOSS_DUALBLADE_HIRAISHIN_AS 
		|| *m_pState == CBoss::BOSS_DUALBLADE_STRIKE_ATTACK || *m_pState == CBoss::BOSS_DUALBLADE_NORMAL_ATTACK_1 || *m_pState == CBoss::BOSS_DUALBLADE_NORMAL_ATTACK_2 || 
		*m_pState == CBoss::BOSS_DUALBLADE_SWEEP_ATTACK_L || *m_pState == CBoss::BOSS_DUALBLADE_SWEEP_ATTACK_R);
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

HRESULT CHarbinger_DualBlade::Render()
{
	if (m_isHide)
		return S_OK;

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHarbinger_DualBlade::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Harbinger_DualBlade", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CHarbinger_DualBlade::Bind_ResourceData()
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

void CHarbinger_DualBlade::Change_Animation(const _float& fTimeDelta)
{
}

CHarbinger_DualBlade* CHarbinger_DualBlade::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHarbinger_DualBlade* pInstance = new CHarbinger_DualBlade(pDevice, pContext);
	
	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CHarbinger_DualBlade::Clone(void* pArg)
{
	CHarbinger_DualBlade* pInstance = new CHarbinger_DualBlade(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CHarbinger_DualBlade::Free()
{
	__super::Free();
}
