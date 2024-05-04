#include "Hili.h"

#include "GameInstance.h"


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
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CHili::Priority_Tick(const _float& fTimeDelta)
{
}

void CHili::Tick(const _float& fTimeDelta)
{
}

void CHili::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CHili::Render()
{
	if (FAILED(__super::Render()))
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
		iAnim = 16;
		isLoop = true;
		break;
	}
	case CHili::HILL_ATTACK:
	{
		iAnim = 14;
		isLoop = false;
		break;
	}
	case CHili::HILI_HIT:
	{
		iAnim = 9;
		isLoop = false;
		break;
	}
	case CHili::HILI_RUN:
	{
		iAnim = 15;
		isLoop = true;
		break;
	}
	case CHili::HILI_DIE:
	{
		isLinearSpeed = false;
		iAnim = 3;
		isLoop = true;
		break;
	}
	case CHili::HILI_WALK_FORWARD:
		isLinearSpeed = true;
		iAnim = 24;
		isLoop = true;
		break;
	case CHili::HILI_END:
		break;
	default:
		break;
	}

	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ iAnim, isLoop, true, isLinearSpeed });
}

void CHili::Free()
{
	__super::Free();
}
