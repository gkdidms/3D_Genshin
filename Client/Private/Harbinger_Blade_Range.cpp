#include "Harbinger_Blade_Range.h"

#include "GameInstance.h"
#include "Effect.h"
#include "Bullet.h"

CHarbinger_Blade_Range::CHarbinger_Blade_Range(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBoss_Skill{ pDevice, pContext }
{
}

CHarbinger_Blade_Range::CHarbinger_Blade_Range(const CHarbinger_Blade_Range& rhs)
	: CBoss_Skill {rhs }
{
}

HRESULT CHarbinger_Blade_Range::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHarbinger_Blade_Range::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fDelayDuration = 0.5f;
	m_fDelayTime = m_fDelayDuration; // 처음 tick 진입 시 오브젝트를 만들기 위해서 초기화를 Duration에 맞춰준다.

	return S_OK;
}

void CHarbinger_Blade_Range::Priority_Tick(const _float& fTimeDelta)
{
}

void CHarbinger_Blade_Range::Tick(const _float& fTimeDelta)
{
	if (m_iMaxNum <= m_iCount)
	{
		m_isDead = true;
		return;
	}

	if (m_fDelayDuration > m_fDelayTime)
		m_fDelayTime += fTimeDelta;
	else
	{
		m_iCount += 1;
		m_fDelayTime = 0.f;

		CEffect::EFFECT_DESC EffectDesc{};

		_matrix TargetMatrix = XMLoadFloat4x4(m_pParentMatrix);
		TargetMatrix.r[3] = TargetMatrix.r[3] + (m_ParentLook * (m_fRange * m_iCount));

		_float4x4 ParentMatrix;
		XMStoreFloat4x4(&ParentMatrix, TargetMatrix);
		EffectDesc.pPlayerMatrix = &ParentMatrix;
		EffectDesc.fDuration = 1.f;

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STAGE_BOSS, TEXT("Prototype_GameObject_Effect_Harbinger_Bow_Range"), TEXT("Layer_Trail"), &EffectDesc)))
			return;

		//Bullet
		CBullet::BULLET_DESC BulletDesc{};
		BulletDesc.ParentMatrix = &ParentMatrix;
		BulletDesc.fSpeedPecSec = 30.f;
		
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STAGE_BOSS, TEXT("Prototype_GameObject_Skill_Harbinger_Bow_Range_Bullet"), TEXT("Layer_Effect"), &BulletDesc)))
			return;
	}
}

void CHarbinger_Blade_Range::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CHarbinger_Blade_Range::Render()
{
	return S_OK;
}

CHarbinger_Blade_Range* CHarbinger_Blade_Range::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHarbinger_Blade_Range* pInstance = new CHarbinger_Blade_Range(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CHarbinger_Blade_Range::Clone(void* pArg)
{
	CHarbinger_Blade_Range* pInstance = new CHarbinger_Blade_Range(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CHarbinger_Blade_Range::Free()
{
	__super::Free();
}
