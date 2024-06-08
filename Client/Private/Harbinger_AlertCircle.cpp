#include "Harbinger_AlertCircle.h"

#include "GameInstance.h"

#include "Effect.h"
#include "Harbinger_Cover.h"

CHarbinger_AlertCircle::CHarbinger_AlertCircle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffect{ pDevice, pContext }
{
}

CHarbinger_AlertCircle::CHarbinger_AlertCircle(const CHarbinger_AlertCircle& rhs)
	: CEffect{ rhs }
{
}

HRESULT CHarbinger_AlertCircle::Initialize_Prototype(const _char* szFileName)
{
	if (FAILED(__super::Initialize_Prototype(szFileName)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHarbinger_AlertCircle::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fCreateNum = 30.f;
	m_RandomNumber = mt19937_64(m_RandomDevice());

	return S_OK;
}

void CHarbinger_AlertCircle::Priority_Tick(const _float& fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CHarbinger_AlertCircle::Tick(const _float& fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fTurnCurrentTime += fTimeDelta;

	if (m_fTurnCurrentTime < m_fTurnDuration)
	{
		m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta);
	}
	else
	{
		if (!m_isCreateBullet) return;

		m_isCreateBullet = false;
		CEffect::EFFECT_DESC EffectDesc{};
		EffectDesc.pPlayerMatrix = m_pTransformCom->Get_WorldFloat4x4();
		EffectDesc.fDuration = 0.7f;

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STAGE_BOSS, TEXT("Prototype_Component_GameObject_Harbinger_Cover_AlertCircle_Start"), TEXT("Layer_Effect"), &EffectDesc)))
			return;

		uniform_real_distribution<float>	RangeX(0.f - 10.f * 0.5f, 0.f + 10.f * 0.5f);
		uniform_real_distribution<float>	RangeZ(0.f - 10.f * 0.5f, 0.f + 10.f * 0.5f);

		//불렛 생성해서 테스트 해보기
		for (size_t i = 0; i < m_fCreateNum; ++i)
		{
			CHarbinger_Cover::BULLET_COVER_DESC BulletDesc{};
			BulletDesc.fSpeedPecSec = 30.f;
			BulletDesc.ParentMatrix = m_pTransformCom->Get_WorldFloat4x4();

			_vector vPos = XMVectorSet(RangeX(m_RandomNumber), 30.f, RangeZ(m_RandomNumber), 1.f);
			BulletDesc.vPos = vPos;

			if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STAGE_BOSS, TEXT("Prototype_GameObject_Skill_Harbinger_Bow_Cover"), TEXT("Layer_Effect"), &EffectDesc)))
				return;
		}
	}
}

void CHarbinger_AlertCircle::Late_Tick(const _float& fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CHarbinger_AlertCircle::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHarbinger_AlertCircle::Add_Components()
{
	if (FAILED(__super::Add_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHarbinger_AlertCircle::File_Open(const _char* szFileName)
{
	if (FAILED(__super::File_Open(szFileName)))
		return E_FAIL;

	return S_OK;
}

CHarbinger_AlertCircle* CHarbinger_AlertCircle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* szFilePath)
{
	CHarbinger_AlertCircle* pInstance = new CHarbinger_AlertCircle(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(szFilePath)))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CHarbinger_AlertCircle::Clone(void* pArg)
{
	CHarbinger_AlertCircle* pInstance = new CHarbinger_AlertCircle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CHarbinger_AlertCircle::Free()
{
	__super::Free();
}
