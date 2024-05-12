#include "Harbinger_Blade.h"

#include "Boss.h"

CHarbinger_Blade::CHarbinger_Blade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CWeapon{ pDevice, pContext }
{
}

CHarbinger_Blade::CHarbinger_Blade(const CHarbinger_Blade& rhs)
	: CWeapon { rhs }
{
}

HRESULT CHarbinger_Blade::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHarbinger_Blade::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;
	
	return S_OK;
}

void CHarbinger_Blade::Priority_Tick(const _float& fTimeDelta)
{
}

void CHarbinger_Blade::Tick(const _float& fTimeDelta)
{
}

void CHarbinger_Blade::Late_Tick(const _float& fTimeDelta)
{
	_matrix		SocketMatrix = XMMatrixIdentity();

	m_isHide = !(*m_pState == CBoss::BOSS_BLADE_EXTRA_ATTACK || *m_pState == CBoss::BOSS_BLADE_RANGE_ATTACK);
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

HRESULT CHarbinger_Blade::Render()
{
	if (m_isHide)
		return S_OK;

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHarbinger_Blade::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Harbinger_Blade", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CHarbinger_Blade::Bind_ResourceData()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CHarbinger_Blade::Change_Animation(const _float& fTimeDelta)
{
}

CHarbinger_Blade* CHarbinger_Blade::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHarbinger_Blade* pInstance = new CHarbinger_Blade(pDevice, pContext);
	
	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CHarbinger_Blade::Clone(void* pArg)
{
	CHarbinger_Blade* pInstance = new CHarbinger_Blade(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CHarbinger_Blade::Free()
{
	__super::Free();
}
