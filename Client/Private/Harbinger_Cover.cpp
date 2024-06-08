#include "Harbinger_Cover.h"

#include "GameInstance.h"
#include "Effect.h"

CHarbinger_Cover::CHarbinger_Cover(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster_Bullet{ pDevice, pContext }
{
}

CHarbinger_Cover::CHarbinger_Cover(const CHarbinger_Cover& rhs)
	: CMonster_Bullet { rhs }
{
}

HRESULT CHarbinger_Cover::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CHarbinger_Cover::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Componenets()))
		return E_FAIL;

	if (nullptr == pArg)
		return E_FAIL;

	BULLET_COVER_DESC* pDesc = static_cast<BULLET_COVER_DESC*>(pArg);
	m_fSpeed = pDesc->fSpeedPecSec;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVector3TransformCoord(pDesc->vPos, XMLoadFloat4x4(pDesc->ParentMatrix)));
	
	XMStoreFloat4(&m_vTargetLook, XMVectorSet(0.f, -1.f, 0.f, 0.f));

	return S_OK;
}

void CHarbinger_Cover::Priority_Tick(const _float& fTimeDelta)
{
}

void CHarbinger_Cover::Tick(const _float& fTimeDelta)
{
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vPosition += XMLoadFloat4(&m_vTargetLook) * m_fSpeed * fTimeDelta;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

	if (m_fHeight >= XMVectorGetY(vPosition))
	{
		//¶¥¿¡ ´êÀ¸¸é
		m_isDead = true;

		CEffect::EFFECT_DESC EffectDesc{};
		EffectDesc.fDuration = 4.f;
		EffectDesc.pPlayerMatrix = m_pTransformCom->Get_WorldFloat4x4();

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_STAGE_BOSS, TEXT("Prototype_Component_GameObject_Harbinger_Cover_AlertCircle_Bullet_End"), TEXT("Layer_Effect"), &EffectDesc)))
		{
			MessageBoxW(g_hWnd, TEXT("Cover Effect »ý¼º ¾ÈµÊ"), 0, 0);
			return;
		}
	}
}

void CHarbinger_Cover::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CHarbinger_Cover::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	m_pShaderCom->Begin(1);
	m_pTrailVIBufferCom->Render();

	return S_OK;
}

HRESULT CHarbinger_Cover::Add_Componenets()
{
	if (FAILED(__super::Add_Component(LEVEL_STAGE_BOSS, L"Prototype_Component_Shader_VtxPosTex_Skill", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STAGE_BOSS, L"Prototype_Component_Texture_Harbinger_Cover_AlertCircle_Bullet", L"Com_Texture", reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	CVIBuffer_Trail::VIBUFFER_TRAIL_DESC TrailDesc{};
	TrailDesc.iMaxTrail = 70;
	TrailDesc.vInitPosA = _float3(-0.3f, 0.f, 0.3f);
	TrailDesc.vInitPosB = _float3(-0.3f, 0.f, -0.3f);
	if (FAILED(__super::Add_Component(LEVEL_STAGE_BOSS, L"Prototype_Component_VIBuffer_Trail", L"Com_TrailBuffer", reinterpret_cast<CComponent**>(&m_pTrailVIBufferCom), &TrailDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHarbinger_Cover::Bind_ResourceData()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	return S_OK;
}

CHarbinger_Cover* CHarbinger_Cover::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHarbinger_Cover* pInstance = new CHarbinger_Cover(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CHarbinger_Cover::Clone(void* pArg)
{
	CHarbinger_Cover* pInstance = new CHarbinger_Cover(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CHarbinger_Cover::Free()
{
	__super::Free();
}
