#include "Harbinger_Bow.h"

#include "Boss.h"

CHarbinger_Bow::CHarbinger_Bow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CWeapon{ pDevice, pContext }
{
}

CHarbinger_Bow::CHarbinger_Bow(const CHarbinger_Bow& rhs)
	: CWeapon { rhs }
{
}

HRESULT CHarbinger_Bow::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHarbinger_Bow::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CHarbinger_Bow::Priority_Tick(const _float& fTimeDelta)
{
}

void CHarbinger_Bow::Tick(const _float& fTimeDelta)
{
}

void CHarbinger_Bow::Late_Tick(const _float& fTimeDelta)
{
	_matrix		SocketMatrix = XMMatrixIdentity();

	m_isHide = !(*m_pState == CBoss::BOSS_BOW_COVER_ATTACK || *m_pState == CBoss::BOSS_BOW_POWER_ATTACK || *m_pState == CBoss::BOSS_BOW_NORMAL_ATTACK_BS);
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

HRESULT CHarbinger_Bow::Render()
{
	if (m_isHide)
		return S_OK;

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHarbinger_Bow::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Harbinger_Bow", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CHarbinger_Bow::Bind_ResourceData()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CHarbinger_Bow::Change_Animation(const _float& fTimeDelta)
{
}

CHarbinger_Bow* CHarbinger_Bow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHarbinger_Bow* pInstance = new CHarbinger_Bow(pDevice, pContext);
	
	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CHarbinger_Bow::Clone(void* pArg)
{
	CHarbinger_Bow* pInstance = new CHarbinger_Bow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CHarbinger_Bow::Free()
{
	__super::Free();
}
