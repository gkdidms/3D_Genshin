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
	WANDERER_DESC* pDesc = (WANDERER_DESC*)pArg;

	m_pDirState = pDesc->pDirState;
	m_pElementalAir = pDesc->isElementalAir;

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

	m_pModelCom->Play_Animation(fTimeDelta, &m_PlayerMovePos, m_IsLinear);

	if (*m_pElementalAir && (*m_pState == PLAYER_RUN_START || *m_pState == PLAYER_RUN))
		XMStoreFloat4(&m_PlayerMovePos, XMVectorSet(0.f, 0.f, m_fRunSpeed * fTimeDelta, 1.f));
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
	m_IsLinear = true;
	switch (*m_pState)
	{
	case PLAYER_ATTACK_1:
	{
		m_iAnim = *m_pElementalAir ? 35: 0;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ATTACK_2:
	{
		m_iAnim = *m_pElementalAir ? 36: 1;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ATTACK_3:
	{
		m_iAnim = *m_pElementalAir ? 37: 2;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ATTACK_IDLE:
	{
		m_iAnim = 82;
		m_IsLoop = false;
		m_IsLinear = false;
		break;
	}
	case PLAYER_ELEMENTAL_START: // Air
	{
		m_iAnim = 6;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ELEMENTAL_END: // ÂøÁö
	{
		m_iAnim = 50;
		m_IsLoop = false;
		m_IsLinear = false;
		break;
	}
	case PLAYER_ELEMENTAL_BURST:
	{
		m_iAnim = *m_pElementalAir ? 39: 38;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ELEMENTAL_BURST_END:
	{
		m_iAnim = *m_pElementalAir ? 40 : 41;
		m_IsLoop = false;
		m_IsLinear = false;
		break;
	}
	case PLAYER_RUN_START:
	{
		if (*m_pElementalAir)
		{
			if (*m_pDirState == CPlayer::DIR_STRIGHT)
			{
				m_iAnim = 24;
				m_IsLoop = false;
			}
			else if (*m_pDirState == CPlayer::DIR_BACKWORK)
			{
				m_iAnim = 21;
				m_IsLoop = false;
			}
			else if (*m_pDirState == CPlayer::DIR_LEFT_SIDE)
			{
				m_iAnim = 27;
				m_IsLoop = false;
			}
			else if (*m_pDirState == CPlayer::DIR_RIGHT_SIDE)
			{
				m_iAnim = 30;
				m_IsLoop = false;
			}
		}
		else
		{
			m_iAnim = 69;
			m_IsLoop = false;
		}
		m_IsLinear = false;
		break;
	}
	case PLAYER_RUN:
	{
		if (*m_pElementalAir)
		{
			if (*m_pDirState == CPlayer::DIR_STRIGHT)
			{
				m_iAnim = 25;
				m_IsLoop = true;
			}
			else if (*m_pDirState == CPlayer::DIR_BACKWORK)
			{
				m_iAnim = 22;
				m_IsLoop = true;
			}
			else if (*m_pDirState == CPlayer::DIR_LEFT_SIDE)
			{
				m_iAnim = 28;
				m_IsLoop = true;
			}
			else if (*m_pDirState == CPlayer::DIR_RIGHT_SIDE)
			{
				m_iAnim = 31;
				m_IsLoop = true;
			}
		}
		else
		{
			m_iAnim = 70;
			m_IsLoop = true;
		}
		break;
	}
	case PLAYER_RUN_STOP:
	{
		if (*m_pElementalAir)
		{
			if (*m_pDirState == CPlayer::DIR_STRIGHT)
			{
				m_iAnim = 23;
				m_IsLoop = false;
			}
			else if (*m_pDirState == CPlayer::DIR_BACKWORK)
			{
				m_iAnim = 20;
				m_IsLoop = false;
			}
			else if (*m_pDirState == CPlayer::DIR_LEFT_SIDE)
			{
				m_iAnim = 26;
				m_IsLoop = false;
			}
			else if (*m_pDirState == CPlayer::DIR_RIGHT_SIDE)
			{
				m_iAnim = 29;
				m_IsLoop = false;
			}
		}
		else
		{
			m_iAnim = 73;
			m_IsLoop = false;
		}
		break;
	}
	case PLAYER_SPRINT_START:
	{
		m_iAnim = 76;
		m_IsLoop = false;
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
		m_IsLinear = false;
		break;
	}
	case PLAYER_SPRINT_STOP:
	{
		m_iAnim = 78;
		m_IsLoop = false;
		break;
	}
	case PLAYER_IDLE:
	{
		m_iAnim = *m_pElementalAir ? 34 : 80;
		m_IsLoop = true;
		m_IsLinear = false;
		break;
	}
	default:
		break;
	}

	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ m_iAnim, m_IsLoop });
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
