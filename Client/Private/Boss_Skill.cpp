#include "Boss_Skill.h"

CBoss_Skill::CBoss_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

HRESULT CBoss_Skill::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBoss_Skill::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	BOSS_SKILL_DESC* pDesc = static_cast<BOSS_SKILL_DESC*>(pArg);
	m_pParentMatrix = pDesc->pParentMatrix;
	m_pTargetMatrix = pDesc->pTargetMatrix;
	m_ParentLook = pDesc->pParentLook;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CBoss_Skill::Priority_Tick(const _float& fTimeDelta)
{
}

void CBoss_Skill::Tick(const _float& fTimeDelta)
{
}

void CBoss_Skill::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CBoss_Skill::Render()
{
	return S_OK;
}

void CBoss_Skill::Free()
{
	__super::Free();
}
