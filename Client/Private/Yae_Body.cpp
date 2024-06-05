#include "Yae_Body.h"

#include "MainApp.h"
#include "GameInstance.h"
#include "StateManager.h"

#include "SkillObj.h"

CYae_Body::CYae_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject_Body{ pDevice, pContext } 
{
}

CYae_Body::CYae_Body(const CYae_Body& rhs)
	: CPartObject_Body{ rhs }
{
}

HRESULT CYae_Body::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CYae_Body::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	strcpy_s(m_Info.szPlayerbleName, "야에 미코");
	m_Info.m_fMaxHp = 10372.f;
	m_Info.m_fHp = m_Info.m_fMaxHp;
	m_Info.m_fAtk = 339.f;
	m_Info.m_fDef = 568.f;
	m_Info.eElementalType = ELECTRIC;

	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ 48, true, true, false });
	return S_OK;
}

void CYae_Body::Priority_Tick(const _float& fTimeDelta)
{
}

void CYae_Body::Tick(const _float& fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (m_pModelCom->Get_Animation_Finished())
		*m_pState = m_pStateManager->Exit(PLAYER_STATE(*m_pState));

	Change_Animation(fTimeDelta);

	m_pModelCom->Play_Animation(fTimeDelta, &m_PlayerMovePos);

	_matrix MoveMatrix = XMLoadFloat4x4(&m_PlayerMovePos);
	// 해당 상태일때 이동 값이 0이면 이전 프레임의 이동값을 가져옮
	if ((*m_pState == PLAYER_SPRINT_TO_RUN || *m_pState == PLAYER_SPRINT_START) && m_PlayerMovePos.m[3][2] <= 0.f)
	{
		m_pModelCom->Bind_AnimSpeed(&MoveMatrix);

		XMStoreFloat4x4(&m_PlayerMovePos, MoveMatrix);
	}

	__super::Move_Pos(fTimeDelta, &MoveMatrix);
	XMStoreFloat4x4(&m_PlayerMovePos, MoveMatrix);

	Create_Object();// 스킬 오브젝트 생성
}

void CYae_Body::Late_Tick(const _float& fTimeDelta)
{
	if (*m_pState != m_PreState)
		m_PreState = *m_pState;

	XMStoreFloat4x4(&m_pWorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));

	//m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CYae_Body::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CYae_Body::Add_Components()
{
	if (FAILED(__super::Add_Component(CMainApp::g_iCurrentLevel, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(CMainApp::g_iCurrentLevel, L"Prototype_Component_Model_Yae", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CYae_Body::Bind_ResourceData()
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

void CYae_Body::Change_Animation(const _float& fTimeDelta)
{
	m_IsLinearSpeed = false;
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
		m_iAnim = 2;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ATTACK_3:
	{
		m_iAnim = 3;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ATTACK_IDLE:
	{
		m_iAnim = 56;
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
		m_IsLinear = false;
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
		m_IsLinear = false;
		break;
	}
	case PLAYER_ELEMENTAL_BURST:
	{
		m_iAnim = 11;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ELEMENTAL_BURST_END:
	{
		m_iAnim = 12;
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
		if (*m_pHill != CPlayer::HILL_END)
		{
			if (*m_pHill == CPlayer::HILL_UP)
			{
				m_iAnim = 45;
				m_IsLoop = true;
			}
			else if (*m_pHill == CPlayer::HILL_DOWN)
			{
				m_iAnim = 42;
				m_IsLoop = true;
			}
		}
		else
		{
			m_iAnim = 41;
			m_IsLoop = true;
		}
		m_IsLinear = false;
		m_IsLinearSpeed = true;
		break;
	}
	case PLAYER_RUN_STOP:
	{

		m_iAnim = 44;
		m_IsLoop = false;
		break;
	}
	case PLAYER_SPRINT_START:
	{
		m_iAnim = 49;
		m_IsLoop = false;
		break;
	}
	case PLAYER_SPRINT:
	{
		m_iAnim = 50;
		m_IsLoop = true;
		break;
	}
	case PLAYER_SPRINT_TO_RUN:
	{
		m_iAnim = 52;
		m_IsLoop = false;
		m_IsLinear = false;
		break;
	}
	case PLAYER_SPRINT_STOP:
	{
		m_iAnim = 51;
		m_IsLoop = false;
		break;
	}
	case PLAYER_JUMP:
	{
		m_iAnim = 37;
		m_IsLoop = false;
		break;
	}
	case PLAYER_JUMP_FOR_RUN:
	{
		m_iAnim = 38;
		m_IsLoop = false;
		break;
	}
	case PLAYER_JUMP_FOR_SPRINT:
	{
		m_iAnim = 39;
		m_IsLoop = false;
		break;
	}
	case PLAYER_FALL_GROUND_H:
	{
		m_iAnim = 21;
		m_IsLoop = false;
		break;
	}
	case PLAYER_FALL_GROUND_L:
	{
		m_iAnim = 22;
		m_IsLoop = false;
		break;
	}
	case PLAYER_FALL_GROUND_FOR_RUN:
	{
		m_iAnim = 52;
		m_IsLoop = false;
		break;
	}
	case PLAYER_FALL_GROUND_FOR_SPRINT:
	{
		m_iAnim = 23;
		m_IsLoop = false;
		break;
	}
	case PLAYER_FALL_ATTACK_LOOP: // 떨어짐
	{
		m_iAnim = 25;
		m_IsLoop = true;
		m_IsLinearSpeed = true;
		break;
	}
	case PLAYER_FLY_START:
	{
		m_iAnim = 29;
		m_IsLoop = false;
		break;
	}
	case PLAYER_FLY_NORMAL:
	{
		m_iAnim = 27;
		m_IsLoop = true;
		m_IsLinearSpeed = true;
		break;
	}
	case PLAYER_FLY_TURN_L:
	{
		m_iAnim = 30;
		m_IsLoop = true;
		m_IsLinearSpeed = true;
		break;
	}
	case PLAYER_FLY_TURN_R:
	{
		m_iAnim = 31;
		m_IsLoop = true;
		m_IsLinearSpeed = true;
		break;
	}
	case PLAYER_IDLE:
	{
		m_iAnim = 53;
		m_IsLoop = true;
		break;
	}
	case PLAYER_IDLE_PUT_AWAY:
	{
		m_iAnim = 54; // 55
		m_IsLoop = true;
		m_IsLinear = false;
		break;
	}
	default:
		break;
	}

	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ m_iAnim, m_IsLoop, m_IsLinear, m_IsLinearSpeed });

}

HRESULT CYae_Body::Create_Object()
{
	if (*m_pState == m_PreState)
		return S_OK;

	// E 스킬 사용 시 
	_bool isElementalArt = *m_pState == PLAYER_ELEMENTAL_1_END
		|| *m_pState == PLAYER_ELEMENTAL_2_END
		|| *m_pState == PLAYER_ELEMENTAL_3_END;

	if (isElementalArt)
	{
		CSkillObj::SKILLOBJ_DESC Desc = {};
		Desc.pParentMatrix = m_pParentMatrix;
		if(FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SkillObj_FoxTower"), TEXT("Layer_SKillObj"), &Desc)))
			return E_FAIL;
	}

	_bool isAtk = *m_pState == PLAYER_ATTACK_1 || *m_pState == PLAYER_ATTACK_2 || *m_pState == PLAYER_ATTACK_3;

	if (isAtk)
	{
		CSkillObj::SKILLOBJ_DESC Desc = {};
		Desc.pParentMatrix = &m_pWorldMatrix;
		Desc.pState = m_pState;

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_SkillObj_BlackFox"), TEXT("Layer_SKillObj"), &Desc)))
			return E_FAIL;
	}
}

CYae_Body* CYae_Body::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CYae_Body* pInstance = new CYae_Body(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CYae_Body::Clone(void* pArg)
{
	CYae_Body* pInstance = new CYae_Body(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CYae_Body::Free()
{
	__super::Free();
}
