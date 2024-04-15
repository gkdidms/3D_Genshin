#include "Wanderer_Body.h"

#include "GameInstance.h"
#include "State_Manager.h"

CWanderer_Body::CWanderer_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject_Body{ pDevice, pContext } 
{
}

CWanderer_Body::CWanderer_Body(const CWanderer_Body& rhs)
	: CPartObject_Body{ rhs }
{
}

HRESULT CWanderer_Body::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWanderer_Body::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ 80, true });
	return S_OK;
}

void CWanderer_Body::Priority_Tick(const _float& fTimeDelta)
{
}

void CWanderer_Body::Tick(const _float& fTimeDelta)
{
	if (m_pModelCom->Get_Animation_Finished())
		*m_pState = m_pState_Manager->Exit(PLAYER_STATE(*m_pState));

	Change_Animation(fTimeDelta);

	if (*m_pState == PLAYER_RUN || *m_pState == PLAYER_RUN_START || *m_pState == PLAYER_SPRINT_START || *m_pState == PLAYER_SPRINT || *m_pState == PLAYER_SPRINT_TO_RUN)
	{
		m_pModelCom->Play_Animation(fTimeDelta, &m_vCurrentPos);
	}
	else
	{
		m_pModelCom->Play_Animation(fTimeDelta, &m_vCurrentPos, !m_IsLoop);

		XMStoreFloat4(&m_PlayerMovePos, XMLoadFloat4(&m_vCurrentPos) - XMLoadFloat4(&m_vPrePos));
		m_PlayerMovePos.w = 1.f;
		m_vPrePos = m_vCurrentPos;
	}

}

void CWanderer_Body::Late_Tick(const _float& fTimeDelta)
{
	XMStoreFloat4x4(&m_pWorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));

	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CWanderer_Body::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWanderer_Body::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Wanderer", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CWanderer_Body::Bind_ResourceData()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CWanderer_Body::Change_Animation(const _float& fTimeDelta)
{
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
	case PLAYER_ATTACK_IDLE:
	{
		m_iAnim = 82;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ELEMENTAL_1:
	{
		m_iAnim = 4;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ELEMENTAL_END:
	{
		m_iAnim = 5;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ELENENTAL_BURST:
	{
		m_iAnim = 38;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ELENENTAL_BURST_END:
	{
		m_iAnim = 41;
		m_IsLoop = false;
		break;
	}
	case PLAYER_RUN_START:
	{
		m_iAnim = 69;
		m_IsLoop = false;
		XMStoreFloat4(&m_PlayerMovePos, XMVectorSet(0.f, 0.f, m_fRunSpeed * fTimeDelta, 1.f));
		break;
	}
	case PLAYER_RUN:
	{
		m_iAnim = 70;
		m_IsLoop = true;
		break;
	}
	case PLAYER_SPRINT_START:
	{
		m_iAnim = 76;
		m_IsLoop = false;
		XMStoreFloat4(&m_PlayerMovePos, XMVectorSet(0.f, 0.f, m_fRunSpeed * fTimeDelta, 1.f));
		break;
	}
	case PLAYER_SPRINT:
	{
		m_iAnim = 77;
		m_IsLoop = true;
		break;
	}
	case PLAYER_SPRINT_TO_RUN:
	{
		m_iAnim = 79;
		m_IsLoop = false;
		break;
	}
	case PLAYER_SPRINT_STOP:
	{
		m_iAnim = 78;
		m_IsLoop = false;
		break;
	}
	case PLAYER_RUN_STOP:
	{
		m_iAnim = 73;
		m_IsLoop = false;
		break;
	}
	case PLAYER_IDLE:
	{
		m_iAnim = 80;
		m_IsLoop = true;
		break;
	}
	default:
		break;
	}

	if (m_pModelCom->Get_LoopAnimation_Finished() || m_iPreAnim != m_iAnim)
		XMStoreFloat4(&m_vPrePos, XMVectorSet(0.f, 0.f, 0.f, 1.f));

	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ m_iAnim, m_IsLoop });

	m_iPreAnim = m_iAnim;

}

CWanderer_Body* CWanderer_Body::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWanderer_Body* pInstance = new CWanderer_Body(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CWanderer_Body::Clone(void* pArg)
{
	CWanderer_Body* pInstance = new CWanderer_Body(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CWanderer_Body::Free()
{
	__super::Free();
}
