#include "Nilou_Body.h"

#include "GameInstance.h"
#include "State_Manager.h"

CNilou_Body::CNilou_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject_Body{ pDevice, pContext } 
{
}

CNilou_Body::CNilou_Body(const CNilou_Body& rhs)
	: CPartObject_Body{ rhs }
{
}

HRESULT CNilou_Body::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CNilou_Body::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ 54, true, true, false });
	return S_OK;
}

void CNilou_Body::Priority_Tick(const _float& fTimeDelta)
{
}

void CNilou_Body::Tick(const _float& fTimeDelta)
{
	if (m_pModelCom->Get_Animation_Finished())
		*m_pState = m_pState_Manager->Exit(PLAYER_STATE(*m_pState));

	Change_Animation(fTimeDelta);

	m_pModelCom->Play_Animation(fTimeDelta, &m_PlayerMovePos);

	// 해당 상태일때 이동 값이 0이면 이전 프레임의 이동값을 가져옮
	if ((*m_pState == PLAYER_SPRINT_TO_RUN || *m_pState == PLAYER_SPRINT_START) && m_PlayerMovePos.m[3][2] <= 0.f)
	{
		_matrix MoveMatrix = XMLoadFloat4x4(&m_PlayerMovePos);
		m_pModelCom->Bind_AnimSpeed(&MoveMatrix);

		XMStoreFloat4x4(&m_PlayerMovePos, MoveMatrix);
	}
}

void CNilou_Body::Late_Tick(const _float& fTimeDelta)
{
	XMStoreFloat4x4(&m_pWorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));

	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CNilou_Body::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CNilou_Body::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Nilou", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CNilou_Body::Bind_ResourceData()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CNilou_Body::Change_Animation(const _float& fTimeDelta)
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
	case PALYER_ATTACK_SPEC:
	{
		m_iAnim = 3;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ATTACK_IDLE:
	{
		m_iAnim = 57;
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
	case PLAYER_ELEMENTAL_1_END:
	{
		m_iAnim = 8;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ELEMENTAL_2:
	{
		m_iAnim = 9;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ELEMENTAL_2_END:
	{
		m_iAnim = 10;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ELEMENTAL_3:
	{
		m_iAnim = 11;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ELEMENTAL_3_END:
	{
		m_iAnim = 12;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ELEMENTAL_SPEC:
	{
		m_iAnim = 13;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ELEMENTAL_SPEC_END:
	{
		m_iAnim = 14;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ELEMENTAL_BURST:
	{
		m_iAnim = 15;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ELEMENTAL_BURST_END:
	{
		m_iAnim = 16;
		m_IsLoop = false;
		//m_IsLinear = false;
		break;
	}
	case PLAYER_RUN_START:
	{
		m_iAnim = 43;
		m_IsLoop = false;
		break;
	}
	case PLAYER_RUN:
	{
		m_iAnim = 44;
		m_IsLoop = true;
		break;
	}
	case PLAYER_RUN_STOP:
	{
		m_iAnim = 47;
		m_IsLoop = false;
		break;
	}
	case PLAYER_SPRINT_START:
	{
		m_iAnim = 50;
		m_IsLoop = false;
		break;
	}
	case PLAYER_SPRINT:
	{
		m_iAnim = 51;
		m_IsLoop = true;
		break;
	}
	case PLAYER_SPRINT_STOP:
	{
		m_iAnim = 52;
		m_IsLoop = false;
		break;
	}
	case PLAYER_SPRINT_TO_RUN:
	{
		m_iAnim = 53;
		m_IsLoop = false;
		//m_IsLinear = false;
		break;
	}
	case PLAYER_JUMP:
	{
		m_iAnim = 40;
		m_IsLoop = false;
		break;
	}
	case PLAYER_JUMP_FOR_RUN:
	{
		m_iAnim = 41;
		m_IsLoop = false;
		break;
	}
	case PLAYER_JUMP_FOR_SPRINT:
	{
		m_iAnim = 42;
		m_IsLoop = false;
		break;
	}
	case PLAYER_IDLE:
	{
		m_iAnim = 54;
		m_IsLoop = true;
		m_IsLinear = false;
		break;
	}
	case PLAYER_IDLE_PUT_AWAY:
	{
		m_iAnim = 55;
		m_IsLoop = true;
		m_IsLinear = false;
		break;
	}
	default:
		break;
	}

	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ m_iAnim, m_IsLoop, m_IsLinear, m_IsLinearSpeed });

}

CNilou_Body* CNilou_Body::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNilou_Body* pInstance = new CNilou_Body(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CNilou_Body::Clone(void* pArg)
{
	CNilou_Body* pInstance = new CNilou_Body(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CNilou_Body::Free()
{
	__super::Free();
}
