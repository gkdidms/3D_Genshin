#include "Boss_Trail.h"

#include "GameInstance.h"

#include "Effect.h"

CBoss_Trail::CBoss_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBlendObject{ pDevice, pContext }
{
}

CBoss_Trail::CBoss_Trail(const CBoss_Trail& rhs)
	: CBlendObject{ rhs }
{
}

HRESULT CBoss_Trail::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBoss_Trail::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	BOSS_TRAIL_DESC* pDesc = static_cast<BOSS_TRAIL_DESC*>(pArg);
	m_pParentMatrix = pDesc->pParentMatrix;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Componenets()))
		return E_FAIL;

	return S_OK;
}

void CBoss_Trail::Priority_Tick(const _float& fTimeDelta)
{
}

void CBoss_Trail::Tick(const _float& fTimeDelta)
{
	_matrix TrailMatrix = XMLoadFloat4x4(m_pParentMatrix);

	TrailMatrix.r[3] = XMVectorSetY(TrailMatrix.r[3], XMVectorGetY(TrailMatrix.r[3]) + 1.2f);

	_float4x4 CopyTrailMatrix;
	XMStoreFloat4x4(&CopyTrailMatrix, TrailMatrix);

	m_pTransformCom->Get_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f), &CopyTrailMatrix);
	m_pTrailBufferCom->Add_Trail(fTimeDelta, XMLoadFloat4x4(&CopyTrailMatrix));

	m_fEffectDelayTime += fTimeDelta;
	m_fEffectLightningTime += fTimeDelta;

	if (m_fEffectDelayDuration <= m_fEffectDelayTime)
	{
		m_fEffectDelayTime = 0.f;

		CEffect::EFFECT_DESC EffectDesc{};
		EffectDesc.fDuration = 0.3f;
		//_matrix ChangeMatrix = TrailMatrix;
		//ChangeMatrix.r[3] = XMVectorSetZ(ChangeMatrix.r[3], XMVectorGetZ(ChangeMatrix.r[3]) + -2.f);
		//XMStoreFloat4x4(&CopyTrailMatrix, ChangeMatrix);
		EffectDesc.pPlayerMatrix = &CopyTrailMatrix;
		if (m_pGameInstance->Add_GameObject(LEVEL_STAGE_BOSS, TEXT("Prototype_GameObject_Effect_Tartaglia_Lightning_3"), TEXT("Layer_Effect"), &EffectDesc))
			return;
	}
	
	if (0.1f <= m_fEffectLightningTime)
	{
		m_fEffectLightningTime = 0.f;

		CEffect::EFFECT_DESC EffectDesc{};
		EffectDesc.fDuration = 0.5f;
		EffectDesc.pPlayerMatrix = &CopyTrailMatrix;
		if (m_pGameInstance->Add_GameObject(LEVEL_STAGE_BOSS, TEXT("Prototype_GameObject_Effect_Tartaglia_Lightning_1"), TEXT("Layer_Effect"), &EffectDesc))
			return;
	}

}

void CBoss_Trail::Late_Tick(const _float& fTimeDelta)
{
	//Compute_ViewZ(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONLIGHT, this);
}

HRESULT CBoss_Trail::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	m_pShaderCom->Begin(7);
	m_pTrailBufferCom->Render();

	return S_OK;
}

HRESULT CBoss_Trail::Add_Componenets()
{
	if (FAILED(__super::Add_Component(LEVEL_STAGE_BOSS, L"Prototype_Component_Shader_VtxPosTex_Skill", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	CVIBuffer_Trail::VIBUFFER_TRAIL_DESC TrailDesc{};
	TrailDesc.iMaxTrail = 30;
	TrailDesc.vInitPosA = _float3(-0.5f, 0.3f, 0.f);
	TrailDesc.vInitPosB = _float3(-0.5f, -0.3f, 0.f);
	if (FAILED(__super::Add_Component(LEVEL_STAGE_BOSS, L"Prototype_Component_VIBuffer_Trail", L"Com_VIBuffer", reinterpret_cast<CComponent**>(&m_pTrailBufferCom), &TrailDesc)))
		return E_FAIL;
	
	if (FAILED(__super::Add_Component(LEVEL_STAGE_BOSS, L"Prototype_Component_Texture_Boss_Trail", L"Com_Texture", reinterpret_cast<CComponent**>(&m_pTextureCom[0]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STAGE_BOSS, L"Prototype_Component_Texture_Boss_Trail_Mask", L"Com_Mask", reinterpret_cast<CComponent**>(&m_pTextureCom[1]))))
		return E_FAIL;

	return S_OK;
}

HRESULT CBoss_Trail::Bind_ResourceData()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom[0]->Bind_ShaderResource(m_pShaderCom, "g_TrailDiffuseTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pTextureCom[1]->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture", 0)))
		return E_FAIL;

	return S_OK;
}

CBoss_Trail* CBoss_Trail::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBoss_Trail* pInstance = new CBoss_Trail(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CBoss_Trail::Clone(void* pArg)
{
	CBoss_Trail* pInstance = new CBoss_Trail(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CBoss_Trail::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTrailBufferCom);
	for (auto& pTexture : m_pTextureCom)
		Safe_Release(pTexture);

}
