#include "Tighnari_Body.h"

#include "GameInstance.h"
#include "StateManager.h"

#include "FlowerArrow.h"
#include "Tighnari_Normal.h"
#include "Effect.h"
#include "Effect_Image.h"

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

	strcpy_s(m_Info.szPlayerbleName, "타이나리");
	m_Info.m_fMaxHp = 10849.f;
	m_Info.m_fHp = m_Info.m_fMaxHp;
	m_Info.m_fAtk = 267.f;
	m_Info.m_fDef = 630.f;
	m_Info.eElementalType = DENDRO;

	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ 46, true, true, false });
	return S_OK;
}

void CTighnari_Body::Priority_Tick(const _float& fTimeDelta)
{
}

void CTighnari_Body::Tick(const _float& fTimeDelta)
{
	if (m_pModelCom->Get_Animation_Finished())
	{
		*m_pState = m_pStateManager->Exit(PLAYER_STATE(*m_pState));
	}

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

	if (FAILED(Create_Object(fTimeDelta)))
		return;
}

void CTighnari_Body::Late_Tick(const _float& fTimeDelta)
{
	if (*m_pState != m_PreState)
	{
		m_PreState = *m_pState;
		m_isCreated = false;
		m_fCurrentTime = 0.f;
	}
		

	XMStoreFloat4x4(&m_pWorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));

	//m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
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

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fFar", m_pGameInstance->Get_CamFar(), sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

void CTighnari_Body::Change_Animation(const _float& fTimeDelta)
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
		m_iAnim = 48;
		m_IsLoop = false;
		m_IsLinear = false;
		break;
	}
	case PLAYER_ELEMENTAL_1:
	{
		m_iAnim = 7;
		m_IsLoop = false;
		m_IsLinear = false;
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
		if (*m_pHill != CPlayer::HILL_END)
		{
			if (*m_pHill == CPlayer::HILL_UP)
			{
				m_iAnim = 40;
				m_IsLoop = true;
			}
			else if (*m_pHill == CPlayer::HILL_DOWN)
			{
				m_iAnim = 37;
				m_IsLoop = true;
			}
		}
		else
		{
			m_iAnim = 36;
			m_IsLoop = true;
		}
		m_IsLinearSpeed = true;
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
		m_iAnim = 42;
		m_IsLoop = false;
		break;
	}
	case PLAYER_SPRINT:
	{
		m_iAnim = 43;
		m_IsLoop = true;
		break;
	}
	case PLAYER_SPRINT_STOP:
	{
		m_iAnim = 44;
		m_IsLoop = false;
		break;
	}
	case PLAYER_SPRINT_TO_RUN:
	{
		m_iAnim = 45;
		m_IsLoop = false;
		m_IsLinear = false;
		break;
	}
	case PLAYER_JUMP:
	{
		m_iAnim = 32;
		m_IsLoop = false;
		break;
	}
	case PLAYER_JUMP_FOR_RUN:
	{
		m_iAnim = 33;
		m_IsLoop = false;
		break;
	}
	case PLAYER_JUMP_FOR_SPRINT:
	{
		m_iAnim = 34;
		m_IsLoop = false;
		break;
	}
	case PLAYER_IDLE:
	{
		m_iAnim = 46;
		m_IsLoop = true;
		m_IsLinear = false;
		break;
	}
	case PLAYER_FALL_GROUND_H:
	{
		m_iAnim = 16;
		m_IsLoop = false;
		break;
	}
	case PLAYER_FALL_GROUND_L:
	{
		m_iAnim = 17;
		m_IsLoop = false;
		break;
	}
	case PLAYER_FALL_GROUND_FOR_RUN:
	{
		m_iAnim = 18;
		m_IsLoop = false;
		break;
	}
	case PLAYER_FALL_GROUND_FOR_SPRINT:
	{
		m_iAnim = 20;
		m_IsLoop = false;
		break;
	}
	case PLAYER_FALL_ATTACK_LOOP: // 떨어짐
	{
		m_iAnim = 13;
		m_IsLoop = true;
		m_IsLinearSpeed = true;
		break;
	}
	case PLAYER_FLY_START:
	{
		m_iAnim = 24;
		m_IsLoop = false;
		break;
	}
	case PLAYER_FLY_NORMAL:
	{
		m_iAnim = 22;
		m_IsLoop = true;
		m_IsLinearSpeed = true;
		break;
	}
	case PLAYER_FLY_TURN_L:
	{
		m_iAnim = 25;
		m_IsLoop = true;
		m_IsLinearSpeed = true;
		break;
	}
	case PLAYER_FLY_TURN_R:
	{
		m_iAnim = 26;
		m_IsLoop = true;
		m_IsLinearSpeed = true;
		break;
	}
	case PLAYER_IDLE_PUT_AWAY:
	{
		m_iAnim = 47;
		m_IsLoop = false;
		m_IsLinear = false;
		break;
	}
	default:
		break;
	}

	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ m_iAnim, m_IsLoop, m_IsLinear, m_IsLinearSpeed });
}

HRESULT CTighnari_Body::Create_Object(const _float& fTimeDetla)
{
	if (m_isCreated)
		return S_OK;

	//Equip_Bow_Ayus_Model bullet 생성   Prototype_GameObject_Skill_Tighnari_Normal
	if (m_pStateManager->isNormalAttack())
	{
		m_fCurrentTime += fTimeDetla;

		if (*m_pState == PLAYER_ATTACK_1 || *m_pState == PLAYER_ATTACK_2)
			m_isCreated = true;
		else if (*m_pState == PLAYER_ATTACK_3)
		{
			if (m_fCurrentTime > 0.4f)
				m_isCreated = true;
			else return S_OK;
		}
		else if (*m_pState == PLAYER_ATTACK_4)
		{
			if (m_fCurrentTime > 0.5f)
				m_isCreated = true;
			else return S_OK;
		}
		

		CEffect::EFFECT_DESC EffectDesc{};

		EffectDesc.pPlayerMatrix = m_pParentMatrix;
		XMStoreFloat4x4(&EffectDesc.RotationMatrix, XMMatrixIdentity());
		EffectDesc.vPos = _float4(0.f, 1.2f, 0.5f, 1.f);
		EffectDesc.vScale = _float3(1.f, 1.f, 1.f);
		EffectDesc.fDuration = 0.1f;
		EffectDesc.iMoveType = CEffect_Image::INCREASE;

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Tighnari_Effect_Arrow_Start"), TEXT("Layer_Trail"), &EffectDesc)))
			return E_FAIL;

		CBullet::BULLET_DESC Desc{};
		Desc.HandCombinedTransformationMatrix = *m_pModelCom->Get_BoneCombinedTransformationMatrix("PRIVATE_RHand");
		Desc.ParentMatrix = m_pWorldMatrix;
		Desc.pTargetPos = Targeting();
		Desc.fSpeedPecSec = 30.f;

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Skill_Tighnari_Normal"), TEXT("Layer_Bullet"), &Desc)))
			return E_FAIL;
		
	}
	else if (m_pStateManager->isElementalBurst())
	{
		
		if (*m_pState == PLAYER_ELEMENTAL_BURST_END)
		{
			m_isCreated = true;

			CBullet::BULLET_DESC Desc{};
			Desc.HandCombinedTransformationMatrix = *m_pModelCom->Get_BoneCombinedTransformationMatrix("PRIVATE_RHand");
			Desc.ParentMatrix = m_pWorldMatrix;
			Desc.pTargetPos = Targeting();
			Desc.fSpeedPecSec = 30.f;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Skill_Tighnari_Busrt"), TEXT("Layer_Bullet"), &Desc)))
				return E_FAIL;
		}
	}

	if (*m_pState == PLAYER_ELEMENTAL_BURST)
	{
		m_isCreated = true;

		CSkillObj::SKILLOBJ_DESC SkillObjDesc{};
		SkillObjDesc.pParentMatrix = m_pParentMatrix;
		SkillObjDesc.pState = m_pState;
		SkillObjDesc.pHandCombinedTransformationMatrix = m_pModelCom->Get_BoneCombinedTransformationMatrix("PRIVATE_RHand");
		if (nullptr == SkillObjDesc.pHandCombinedTransformationMatrix)
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_SkillObj_FlowerArrow", L"Layer_SkillObj", &SkillObjDesc)))
			return E_FAIL;
	}

	

	return S_OK;
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
