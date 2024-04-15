#include "PartObject_Ayus.h"

#include "GameInstance.h"

CPartObject_Ayus::CPartObject_Ayus(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject_Weapon{ pDevice, pContext }
{
}

CPartObject_Ayus::CPartObject_Ayus(const CPartObject_Ayus& rhs)
	: CPartObject_Weapon{ rhs }
{
}

HRESULT CPartObject_Ayus::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPartObject_Ayus::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CPartObject_Ayus::Priority_Tick(const _float& fTimeDelta)
{
}

void CPartObject_Ayus::Tick(const _float& fTimeDelta)
{
}

void CPartObject_Ayus::Late_Tick(const _float& fTimeDelta)
{
	XMStoreFloat4x4(&m_pWorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));

	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CPartObject_Ayus::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPartObject_Ayus::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Wanderer", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CPartObject_Ayus::Bind_ResourceData()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CPartObject_Ayus::Change_Animation(const _float& fTimeDelta)
{
}

CPartObject_Ayus* CPartObject_Ayus::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPartObject_Ayus* pInstance = new CPartObject_Ayus(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CPartObject_Ayus::Clone(void* pArg)
{
	CPartObject_Ayus* pInstance = new CPartObject_Ayus(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CPartObject_Ayus::Free()
{
	__super::Free();
}
