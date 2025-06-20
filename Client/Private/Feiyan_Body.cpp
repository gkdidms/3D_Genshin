#include "Feiyan_Body.h"

#include "MainApp.h"
#include "StateManager.h"

#include "Feiyan_Normal.h"
#include <Effect.h>
#include <Effect_Image.h>
#include <Monster.h>

CFeiyan_Body::CFeiyan_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject_Body{ pDevice, pContext }
{
}

CFeiyan_Body::CFeiyan_Body(const CFeiyan_Body& rhs)
	: CPartObject_Body{ rhs }
{
}

HRESULT CFeiyan_Body::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFeiyan_Body::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	strcpy_s(m_Info.szPlayerbleName, "연비");
	m_Info.m_fMaxHp = 9352.f;
	m_Info.m_fHp = m_Info.m_fMaxHp;
	m_Info.m_fAtk = 229.f;
	m_Info.m_fDef = 728.f;
	m_Info.eElementalType = FIRE;

	return S_OK;
}

void CFeiyan_Body::Priority_Tick(const _float& fTimeDelta)
{
}

void CFeiyan_Body::Tick(const _float& fTimeDelta)
{
	if (m_pModelCom->Get_Animation_Finished())
		*m_pState = m_pStateManager->Exit(PLAYER_STATE(*m_pState));

	Change_Animation(fTimeDelta);

	m_pModelCom->Play_Animation(fTimeDelta, &m_PlayerMovePos);

	_matrix MoveMatrix = XMLoadFloat4x4(&m_PlayerMovePos);
	// 해당 상태일때 이동 값이 0이면 이전 프레임의 이동값을 가져옮
	if ((*m_pState == PLAYER_SPRINT_TO_RUN || *m_pState == PLAYER_SPRINT_START ) && m_PlayerMovePos.m[3][2] <= 0.f)
	{
		m_pModelCom->Bind_AnimSpeed(&MoveMatrix);

		XMStoreFloat4x4(&m_PlayerMovePos, MoveMatrix);
	}

	__super::Move_Pos(fTimeDelta, &MoveMatrix);

	XMStoreFloat4x4(&m_PlayerMovePos, MoveMatrix);

	Create_Bullet(TEXT("Layer_Bullet"), fTimeDelta);
}


void CFeiyan_Body::Late_Tick(const _float& fTimeDelta)
{
	if (m_PreState != *m_pState)
	{
		m_PreState = *m_pState;
		m_isCreated = false;
		m_fCurrentTime = 0.f;
	}
		

	XMStoreFloat4x4(&m_pWorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));

	//m_pGameInstance->Add_Renderer(CRenderer::RENDER_UI, this);
}

HRESULT CFeiyan_Body::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFeiyan_Body::Add_Components()
{
	if (FAILED(__super::Add_Component(CMainApp::g_iCurrentLevel, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(CMainApp::g_iCurrentLevel, L"Prototype_Component_Model_Feiyan", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CFeiyan_Body::Bind_ResourceData()
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

void CFeiyan_Body::Change_Animation(const _float& fTimeDelta)
{
	m_IsLinear = true;
	m_IsLinearSpeed = false;
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
	case PLAYER_EXTRA_ATTACK_START:
	{
		m_iAnim = 15;
		m_IsLoop = false;
		break;
	}
	case PLAYER_EXTRA_ATTACK:
	{
		m_iAnim = 13;
		m_IsLoop = false;
		break;
	}
	case PLAYER_EXTRA_ATTACK_END:
	{
		m_iAnim = 14;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ATTACK_IDLE:
	{
		m_iAnim = 53;
		m_IsLoop = false;
		m_IsLinear = false;
		break;
	}
	case PLAYER_ELEMENTAL_START:
	{
		m_iAnim = 9;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ELEMENTAL_END:
	{
		m_iAnim = 8;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ELEMENTAL_BURST:
	{
		m_iAnim = 12;
		m_IsLoop = false;
		break;
	}
	case PLAYER_ELEMENTAL_BURST_END:
	{
		m_iAnim = 11;
		m_IsLoop = false;
		//m_IsLinear = false;
		break;
	}
	case PLAYER_RUN_START:
	{
		m_iAnim = 39;
		m_IsLoop = false;
		break;
	}
	case PLAYER_RUN:
	{
		if (*m_pHill != CPlayer::HILL_END)
		{
			if (*m_pHill == CPlayer::HILL_UP)
			{
				m_iAnim = 44;
				m_IsLoop = true;
			}
			else if (*m_pHill == CPlayer::HILL_DOWN)
			{
				m_iAnim = 41;
				m_IsLoop = true;
			}
		}
		else
		{
			m_iAnim = 40;
			m_IsLoop = true;
		}
		m_IsLinearSpeed = true;
		break;
	}
	case PLAYER_RUN_STOP:
	{
		m_iAnim = 43;
		m_IsLoop = false;
		break;
	}
	case PLAYER_SPRINT_START:
	{
		m_iAnim = 46;
		m_IsLoop = false;
		break;
	}
	case PLAYER_SPRINT:
	{
		m_iAnim = 47;
		m_IsLoop = true;
		break;
	}
	case PLAYER_SPRINT_STOP:
	{
		m_iAnim = 48;
		m_IsLoop = false;
		break;
	}
	case PLAYER_SPRINT_TO_RUN:
	{
		m_iAnim = 49;
		m_IsLoop = false;
		//m_IsLinear = false;
		break;
	}
	case PLAYER_JUMP:
	{
		m_iAnim = 36;
		m_IsLoop = false;
		break;
	}
	case PLAYER_JUMP_FOR_RUN:
	{
		m_iAnim = 37;
		m_IsLoop = false;
		break;
	}
	case PLAYER_JUMP_FOR_SPRINT:
	{
		m_iAnim = 38;
		m_IsLoop = false;
		break;
	}
	case PLAYER_FALL_GROUND_H:
	{
		m_iAnim = 20;
		m_IsLoop = false;
		break;
	}
	case PLAYER_FALL_GROUND_L:
	{
		m_iAnim = 21;
		m_IsLoop = false;
		break;
	}
	case PLAYER_FALL_GROUND_FOR_RUN:
	{
		m_iAnim = 22;
		m_IsLoop = false;

		// PLAYER_FALL_GROUND_FOR BSH -?>23
		break;
	}
	case PLAYER_FALL_GROUND_FOR_SPRINT:
	{
		m_iAnim = 24;
		m_IsLoop = false;
		break;
	}
	case PLAYER_FALL_ATTACK_LOOP: // 떨어짐
	{
		m_iAnim = 17;
		m_IsLoop = true;
		m_IsLinearSpeed = true;
		break;
	}
	case PLAYER_FLY_START:
	{
		m_iAnim = 28;
		m_IsLoop = false;
		break;
	}
	case PLAYER_FLY_NORMAL:
	{
		m_iAnim = 26;
		m_IsLoop = true;
		m_IsLinearSpeed = true;
		break;
	}
	case PLAYER_FLY_TURN_L:
	{
		m_iAnim = 29;
		m_IsLoop = true;
		m_IsLinearSpeed = true;
		break;
	}
	case PLAYER_FLY_TURN_R:
	{
		m_iAnim = 30;
		m_IsLoop = true;
		m_IsLinearSpeed = true;
		break;
	}
	case PLAYER_IDLE:
	{
		m_iAnim = 50;
		m_IsLoop = true;
		m_IsLinear = false;
		break;
	}
	case PLAYER_IDLE_PUT_AWAY:
	{
		m_iAnim = 51; //-> 52 // 56
		m_IsLoop = false;
		m_IsLinear = false;
		break;
	}
	case PLAYER_HIT:
	{
		m_iAnim = 31;
		m_IsLoop = false;
		break;
	}
	default:
		break;
	}

	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ m_iAnim, m_IsLoop, m_IsLinear, m_IsLinearSpeed });
}

HRESULT CFeiyan_Body::Create_Bullet(const wstring& strLayerTag, const _float& fTimeDelta)
{
	if (m_isCreated)
		return S_OK;

	if (m_pStateManager->isNormalAttack())
	{
		m_fCurrentTime += fTimeDelta;

		CBullet::BULLET_DESC Desc{};

		if (*m_pState == PLAYER_ATTACK_1)
		{
			if (m_fCurrentTime <= 0.2f)
				return S_OK;

			Desc.HandCombinedTransformationMatrix = *m_pModelCom->Get_BoneCombinedTransformationMatrix("PRIVATE_NormalAttack01");
		}
		else if (*m_pState == PLAYER_ATTACK_2)
		{
			if (m_fCurrentTime <= 0.3f)
				return S_OK;

			Desc.HandCombinedTransformationMatrix = *m_pModelCom->Get_BoneCombinedTransformationMatrix("PRIVATE_NormalAttack02");
		}
		else if (*m_pState == PLAYER_ATTACK_3)
		{
			if (m_fCurrentTime <= 0.5f)
				return S_OK;

			Desc.HandCombinedTransformationMatrix = *m_pModelCom->Get_BoneCombinedTransformationMatrix("PRIVATE_NormalAttack03");
		}

		m_isCreated = true;
		Desc.ParentMatrix = m_pParentMatrix;
		Desc.fSpeedPecSec = 10.f;
		_float4 vTargetPos = _float4();
		CGameObject* pMonster = Targeting(&vTargetPos);
		if (nullptr == pMonster)
			XMStoreFloat4(&vTargetPos, XMLoadFloat4x4(m_pParentMatrix).r[3] + XMVector3Normalize(XMLoadFloat4x4(m_pParentMatrix).r[2]) * 10.f);

		Desc.pTargetPos = XMLoadFloat4(&vTargetPos);

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Skill_Feiyan_Normal"), strLayerTag, &Desc)))
			return E_FAIL;

	}

	if (m_pStateManager->isElementalArt())
	{
		if (*m_pState == PLAYER_ELEMENTAL_END)
		{
			m_isCreated = true;

			CEffect::EFFECT_DESC Desc{};
			Desc.fDuration = 1.f;
			CGameObject* pMonster = Targeting();
			if (nullptr == pMonster)
				return S_OK;

			Desc.pPlayerMatrix = dynamic_cast<CTransform*>(pMonster->Get_Component(L"Com_Transform"))->Get_WorldFloat4x4();

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Feiyan_Elemenetal_Art"), strLayerTag, &Desc)))
				return E_FAIL;
		}
	}

	if (m_pStateManager->isElementalBurst())
	{
		if (*m_pState == PLAYER_ELEMENTAL_BURST)
		{
			m_isCreated = true;

			CEffect::EFFECT_DESC Desc{};
			Desc.fDuration = 0.3f;
			Desc.pPlayerMatrix = m_pParentMatrix;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Feiyan_Burst_Start"), strLayerTag, &Desc)))
				return E_FAIL;
		}

		if (*m_pState == PLAYER_ELEMENTAL_BURST_END)
		{
			m_isCreated = true;

			CEffect::EFFECT_DESC Desc{};
			Desc.fDuration = 1.f;
			Desc.pPlayerMatrix = m_pParentMatrix;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Feiyan_Burst_End"), strLayerTag, &Desc)))
				return E_FAIL;
		}
	}

	return S_OK;
}

CFeiyan_Body* CFeiyan_Body::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFeiyan_Body* pInstance = new CFeiyan_Body(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CFeiyan_Body::Clone(void* pArg)
{
	CFeiyan_Body* pInstance = new CFeiyan_Body(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CFeiyan_Body::Free()
{
	__super::Free();
}
