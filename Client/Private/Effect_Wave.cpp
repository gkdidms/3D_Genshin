#include "Effect_Wave.h"

#include "GameInstance.h"

CEffect_Wave::CEffect_Wave(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffect{ pDevice, pContext }
{
}

CEffect_Wave::CEffect_Wave(const CEffect_Wave& rhs)
	: CEffect{ rhs }
{
}

HRESULT CEffect_Wave::Initialize_Prototype(const _char* szFileName)
{
	if (FAILED(__super::Initialize_Prototype(szFileName)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Wave::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CEffect_Wave::Priority_Tick(const _float& fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CEffect_Wave::Tick(const _float& fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fDelayCurrentTime += fTimeDelta;

	if (m_fDelayCurrentTime <= m_fDelayCurrentTime)
	{
		m_fDelayCurrentTime = 0.f;
		//트레일 생성
		EFFECT_DESC EffectDesc{};
		EffectDesc.pPlayerMatrix = m_pTransformCom->Get_WorldFloat4x4();
		EffectDesc.fDuration = 0.6f;

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STAGE_BOSS, TEXT("Prototype_GameObject_Effect_Tartaglia_Wave_Trail_Effect"), TEXT("Layer_Effect"), &EffectDesc)))
			return;
	}

}

void CEffect_Wave::Late_Tick(const _float& fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CEffect_Wave::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Wave::Add_Components()
{
	if (FAILED(__super::Add_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Wave::File_Open(const _char* szFileName)
{
	if (FAILED(__super::File_Open(szFileName)))
		return E_FAIL;

	return S_OK;
}

CEffect_Wave* CEffect_Wave::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* szFilePath)
{
	CEffect_Wave* pInstance = new CEffect_Wave(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(szFilePath)))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CEffect_Wave::Clone(void* pArg)
{
	CEffect_Wave* pInstance = new CEffect_Wave(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CEffect_Wave::Free()
{
	__super::Free();
}
