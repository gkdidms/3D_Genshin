#include "Tighnari_Body.h"

#include "GameInstance.h"
#include "State_Manager.h"

CTighnari_Body::CTighnari_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject_Body{ pDevice, pContext } 
{
}

CTighnari_Body::CTighnari_Body(const CTighnari_Body& rhs)
	: CPartObject_Body{ rhs }
{
}

HRESULT CTighnari_Body::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTighnari_Body::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ 45, true });
	return S_OK;
}

void CTighnari_Body::Priority_Tick(const _float& fTimeDelta)
{
}

void CTighnari_Body::Tick(const _float& fTimeDelta)
{
	if (m_pModelCom->Get_Animation_Finished())
	{
		*m_pState = m_pState_Manager->Exit(PLAYER_STATE(*m_pState));
	}

	Change_Animation(fTimeDelta);

	m_pModelCom->Play_Animation(fTimeDelta, &m_PlayerMovePos, m_IsLinear);
}

void CTighnari_Body::Late_Tick(const _float& fTimeDelta)
{
	XMStoreFloat4x4(&m_pWorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));

	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CTighnari_Body::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTighnari_Body::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Tighnari", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CTighnari_Body::Bind_ResourceData()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CTighnari_Body::Change_Animation(const _float& fTimeDelta)
{
	m_IsLinear = true;
	switch (*m_pState)
	{
	case PLAYER_ATTACK_1:
	{
		m_iAnim = 0;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ATTACK_2:
	{
		m_iAnim = 1;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ATTACK_3:
	{
		m_iAnim = 2;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ATTACK_4:
	{
		m_iAnim = 3;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ATTACK_IDLE:
	{
		m_iAnim = 50;
		m_IsLoop = false;
		m_IsLinear = false;
		break;
	}
	case PLAYER_ELEMENTAL_1:
	{
		m_iAnim = 7;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ELEMENTAL_END:
	{
		m_iAnim = 8;
		m_IsLoop = false;
		m_IsLinear = false;
		break;
	}
	case PLAYER_ELEMENTAL_BURST:
	{
		m_iAnim = 9;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ELEMENTAL_BURST_END:
	{
		m_iAnim = 10;
		m_IsLoop = false;
		m_IsLinear = false;
		break;
	}
	case PLAYER_RUN_START:
	{
		m_iAnim = 35;
		m_IsLoop = false;
		break;
	}
	case PLAYER_RUN:
	{
		m_iAnim = 36;
		m_IsLoop = true;
		break;
	}
	case PLAYER_RUN_STOP:
	{
		m_iAnim = 39;
		m_IsLoop = false;
		break;
	}
	case PLAYER_SPRINT_START:
	{
		m_iAnim = 44;
		m_IsLoop = false;
		break;
	}
	case PLAYER_SPRINT:
	{
		m_iAnim = 45;
		m_IsLoop = true;
		break;
	}
	case PLAYER_SPRINT_STOP:
	{
		m_iAnim = 46;
		m_IsLoop = false;
		break;
	}
	case PLAYER_SPRINT_TO_RUN:
	{
		m_iAnim = 47;
		m_IsLoop = false;
		m_IsLinear = false;
		break;
	}
	case PLAYER_IDLE:
	{
		m_iAnim = 48;
		m_IsLoop = true;
		m_IsLinear = false;
		break;
	}
	case PLAYER_IDLE_PUT_AWAY:
	{
		m_iAnim = 49;
		m_IsLoop = false;
		m_IsLinear = false;
		break;
	}
	default:
		break;
	}

	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ m_iAnim, m_IsLoop });
}

CTighnari_Body* CTighnari_Body::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTighnari_Body* pInstance = new CTighnari_Body(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CTighnari_Body::Clone(void* pArg)
{
	CTighnari_Body* pInstance = new CTighnari_Body(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CTighnari_Body::Free()
{
	__super::Free();
}
