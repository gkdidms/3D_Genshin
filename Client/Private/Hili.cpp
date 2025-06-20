#include "Hili.h"

#include "GameInstance.h"

#include "BT_Hili.h"
#include "BT_Hili_Club.h"
#include "BT_Hili_CrossBow.h"

#include "Hili_Weapon_Club.h"
#include "Hili_Weapon_Crossbow.h"

#include "MonsterHP_Outline.h"


CHili::CHili(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster{pDevice, pContext}
{
}

CHili::CHili(const CHili& rhs)
	: CMonster{ rhs }
{
}

HRESULT CHili::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHili::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	HILI_DESC* pDesc = static_cast<HILI_DESC*>(pArg);
	m_Weapon = pDesc->eWeapon;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_UI()))
		return E_FAIL;

	return S_OK;
}

void CHili::Priority_Tick(const _float& fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CHili::Tick(const _float& fTimeDelta)
{
	m_pWeapon->Tick(fTimeDelta);
}

void CHili::Late_Tick(const _float& fTimeDelta)
{
	m_pWeapon->Late_Tick(fTimeDelta);
}

HRESULT CHili::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (FAILED(m_pWeapon->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHili::Add_Components()
{
	return S_OK;
}

HRESULT CHili::Bind_ResourceData()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fFar", m_pGameInstance->Get_CamFar(), sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

void CHili::Change_Animation(const _float& fTimeDelta)
{
	_uint iAnim = { 0 };
	_bool isLoop = { false };
	_bool isLinearSpeed = { true };

	switch (m_CurrentState)
	{
	case CHili::HILI_IDLE:
	{
		isLinearSpeed = false;
		iAnim = m_Weapon == HILI_WEAPON_CLUB ? 16 : 37;
		isLoop = true;

		m_strStateRank = 'D';
		break;
	}
	case CHili::HILI_NORMAL_ATK:
	{
		iAnim = m_Weapon == HILI_WEAPON_CLUB ? 14 : 29;
		isLoop = false;

		m_strStateRank = 'B';
		break;
	}
	case CHili::HILI_THUMP_ATK:
	{
		iAnim =  21;
		isLoop = false;
		m_strStateRank = 'B';
		break;
	}
	case CHili::HILI_TRIPLE_ATK:
	{
		iAnim = 22;
		isLoop = false;
		m_strStateRank = 'B';
		break;
	}
	case CHili::HILI_HIT:
	{
		iAnim = m_Weapon == HILI_WEAPON_CLUB ? 9 : 31;
		isLoop = false;

		m_strStateRank = 'D';
		break;
	}
	case CHili::HILI_RUN:
	{
		iAnim = m_Weapon == HILI_WEAPON_CLUB ? 15 : 38;
		isLoop = true;

		m_strStateRank = 'B';
		break;
	}
	case CHili::HILI_DIE:
	{
		isLinearSpeed = false;
		iAnim = m_Weapon == HILI_WEAPON_CLUB ? 3 : 43;
		isLoop = true;
		m_strStateRank = 'A';
		break;
	}
	case CHili::HILI_WALK_FORWARD:
		isLinearSpeed = true;
		iAnim = m_Weapon == HILI_WEAPON_CLUB ? 24 : 40;
		isLoop = true;

		m_strStateRank = 'C';
		break;
	case CHili::HILI_WALK_BACK:
		isLinearSpeed = true;
		iAnim = m_Weapon == HILI_WEAPON_CLUB ? 23 : 39;
		isLoop = true;
		m_strStateRank = 'C';
		break;

	case HILI_BORN:
	{
		iAnim = 0;
		isLoop = false;
		m_strStateRank = 'D';
		break;
	}
	case HILI_TAUNT_1:
	{
		iAnim = 18;
		isLoop = false;
		m_strStateRank = 'D';
		break;
	}
	case HILI_TAUNT_2:
		iAnim = 19;
		isLoop = false;
		m_strStateRank = 'D';
		break;
	case CHili::HILI_END:
		break;
	default:
		break;
	}

	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ iAnim, isLoop, true, isLinearSpeed });
}

HRESULT CHili::Ready_Object()
{
	CBT_Hili::BT_HILI_DESC HiliDesc{};
	HiliDesc.pCollider = m_pColliderCom;
	HiliDesc.pModel = m_pModelCom;
	HiliDesc.pState = &m_CurrentState;
	HiliDesc.pTargetMatrix = m_pTargetMatrix;
	HiliDesc.pTransformCom = m_pTransformCom;
	HiliDesc.pWeaponType = &m_Weapon;
	HiliDesc.pInfo = &m_Info;

	CHili_Weapon::HILI_WEAPON_DESC Desc = {};
	Desc.pTargetCombinedTransformationMatrix = m_pTransformCom->Get_WorldFloat4x4();
	Desc.pHandCombinedTransformationMatrix = m_pModelCom->Get_BoneCombinedTransformationMatrix("Multi_Right");
	Desc.pState = &m_CurrentState;
	Desc.fAtk = m_Info.fAtk;

	if (m_Weapon == CHili::HILI_WEAPON_CLUB)
	{
		m_pBT = CBT_Hili_Club::Create(&HiliDesc);

		m_pWeapon = dynamic_cast<CHili_Weapon_Club*>(m_pGameInstance->Clone_Object(L"Prototype_GameObject_Monster_Hili_Weapon_Club", &Desc));
	}
		
	else if (m_Weapon == CHili::HILI_WEAPON_CROSSBOW)
	{
		m_pBT = CBT_Hili_CrossBow::Create(&HiliDesc);

		m_pWeapon = dynamic_cast<CHili_Weapon_Crossbow*>(m_pGameInstance->Clone_Object(L"Prototype_GameObject_Monster_Hili_Weapon_Crossbow", &Desc));
	}
		
	if (nullptr == m_pBT)
		return E_FAIL;

	if (nullptr == m_pWeapon)
		return E_FAIL;

	return S_OK;
}

HRESULT CHili::Ready_UI()
{
	CMonsterHP_Outline::MONSTER_HP_OUTLINE_DESC Desc{};
	Desc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	Desc.pMonsterHP = &m_Info.fHP;
	Desc.fMonsterMaxHP = m_Info.fMaxHp;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, L"Prototype_GameObject_UI_MonsterHP_Outline", L"Layer_UI", &Desc)))
		return E_FAIL;

	return S_OK;
}


void CHili::SetUp_OnTerrain(const _float& fTimeDelta)
{
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vPos = XMVectorSetY(vPos, m_pNavigation->Compute_Height(vPos));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
}

void CHili::Free()
{
	__super::Free();

	Safe_Release(m_pWeapon);
}





