#include "Effect_AlertCircle.h"

#include "GameInstance.h"

#include "Effect.h"

CEffect_AlertCircle::CEffect_AlertCircle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffect{ pDevice, pContext }
{
}

CEffect_AlertCircle::CEffect_AlertCircle(const CEffect_AlertCircle& rhs)
	: CEffect{ rhs }
{
}

HRESULT CEffect_AlertCircle::Initialize_Prototype(const _char* szFileName)
{
	if (FAILED(__super::Initialize_Prototype(szFileName)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_AlertCircle::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CEffect_AlertCircle::Priority_Tick(const _float& fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CEffect_AlertCircle::Tick(const _float& fTimeDelta)
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
		EffectDesc.fDuration = 0.3f;

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STAGE_BOSS, TEXT("Prototype_GameObject_Effect_Tartaglia_AiertCircle_Bullet"), TEXT("Layer_Effect"), &EffectDesc)))
			return;

		EffectDesc.fDuration = 0.5f;
		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STAGE_BOSS, TEXT("Prototype_GameObject_Effect_Tartaglia_AiertCircle_Finish"), TEXT("Layer_Effect"), &EffectDesc)))
			return;
	}
}

void CEffect_AlertCircle::Late_Tick(const _float& fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CEffect_AlertCircle::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_AlertCircle::Add_Components()
{
	if (FAILED(__super::Add_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_AlertCircle::File_Open(const _char* szFileName)
{
	if (FAILED(__super::File_Open(szFileName)))
		return E_FAIL;

	return S_OK;
}

CEffect_AlertCircle* CEffect_AlertCircle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* szFilePath)
{
	CEffect_AlertCircle* pInstance = new CEffect_AlertCircle(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(szFilePath)))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CEffect_AlertCircle::Clone(void* pArg)
{
	CEffect_AlertCircle* pInstance = new CEffect_AlertCircle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CEffect_AlertCircle::Free()
{
	__super::Free();
}
