#include "FoxTail_Burst.h"

CFoxTail_Burst::CFoxTail_Burst(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSkillObj{ pDevice, pContext }
{
}

CFoxTail_Burst::CFoxTail_Burst(const CFoxTail_Burst& rhs)
	: CSkillObj{ rhs }
{
}

HRESULT CFoxTail_Burst::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFoxTail_Burst::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f));
	return S_OK;
}

void CFoxTail_Burst::Priority_Tick(const _float& fTimeDelta)
{
}

void CFoxTail_Burst::Tick(const _float& fTimeDelta)
{
	Change_Animation(fTimeDelta);

	_float4x4 pMoveMatrix;
	m_pModelCom->Play_Animation(fTimeDelta, &pMoveMatrix);
}

void CFoxTail_Burst::Late_Tick(const _float& fTimeDelta)
{
	m_isHide = !(*m_pState == PLAYER_ELEMENTAL_BURST || *m_pState == PLAYER_ELEMENTAL_BURST_END);
	if (!m_isHide)
	{
		XMStoreFloat4x4(&m_pWorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));

		m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
	}
}

HRESULT CFoxTail_Burst::Render()
{
	if (m_isHide) return S_OK;

	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFoxTail_Burst::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_FoxTail_Burst", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CFoxTail_Burst::Bind_ResourceData()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CFoxTail_Burst::Change_Animation(const _float& fTimeDelta)
{
}

CFoxTail_Burst* CFoxTail_Burst::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFoxTail_Burst* pInstance = new CFoxTail_Burst(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CFoxTail_Burst::Clone(void* pArg)
{
	CFoxTail_Burst* pInstance = new CFoxTail_Burst(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CFoxTail_Burst::Free()
{
	__super::Free();
}
