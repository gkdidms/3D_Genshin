#include "Weapon_Alaya.h"

#include "GameInstance.h"
#include "State_Manager.h"

CWeapon_Alaya::CWeapon_Alaya(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CWeapon{ pDevice, pContext }
{
}

CWeapon_Alaya::CWeapon_Alaya(const CWeapon_Alaya& rhs)
	: CWeapon{ rhs }
{
}

HRESULT CWeapon_Alaya::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWeapon_Alaya::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pTransformCom->Scaling(10.f, 10.f, 10.f);

	return S_OK;
}

void CWeapon_Alaya::Priority_Tick(const _float& fTimeDelta)
{
}

void CWeapon_Alaya::Tick(const _float& fTimeDelta)
{
}

void CWeapon_Alaya::Late_Tick(const _float& fTimeDelta)
{
	if (*m_pState == PLAYER_ATTACK_1
		|| *m_pState == PLAYER_ATTACK_2
		|| *m_pState == PLAYER_ATTACK_3
		|| *m_pState == PLAYER_ATTACK_4
		|| *m_pState == PLAYER_ELEMENTAL_BURST
		|| *m_pState == PLAYER_ELEMENTAL_BURST_END)
	{
		m_isHide = false;
		_matrix SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

		SocketMatrix.r[0] = XMVector3Normalize(SocketMatrix.r[0]);
		SocketMatrix.r[1] = XMVector3Normalize(SocketMatrix.r[1]);
		SocketMatrix.r[2] = XMVector3Normalize(SocketMatrix.r[2]);

		XMStoreFloat4x4(&m_pWorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));

		m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
	}
	else
		m_isHide = true;
}

HRESULT CWeapon_Alaya::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeapon_Alaya::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Alaya", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CWeapon_Alaya::Bind_ResourceData()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CWeapon_Alaya::Change_Animation(const _float& fTimeDelta)
{
}

CWeapon_Alaya* CWeapon_Alaya::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWeapon_Alaya* pInstance = new CWeapon_Alaya(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CWeapon_Alaya::Clone(void* pArg)
{
	CWeapon_Alaya* pInstance = new CWeapon_Alaya(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CWeapon_Alaya::Free()
{
	__super::Free();
}
