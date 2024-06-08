#include "Boss_Range_Bullet.h"

#include "GameInstance.h"

#include "Effect.h"

CBoss_Range_Bullet::CBoss_Range_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster_Bullet{ pDevice, pContext }
{
}

CBoss_Range_Bullet::CBoss_Range_Bullet(const CBoss_Range_Bullet& rhs)
	: CMonster_Bullet{ rhs }
{
}

HRESULT CBoss_Range_Bullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBoss_Range_Bullet::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	BULLET_DESC* pDesc = static_cast<BULLET_DESC*>(pArg);
	m_fSpeed = pDesc->fSpeedPecSec;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	_matrix WorldMatrix;
	//m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45.f));
	WorldMatrix = m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(pDesc->ParentMatrix);

	m_pTransformCom->Set_WorldMatrix(WorldMatrix);
	XMStoreFloat4(&m_vTargetLook, XMVectorSet(0.f, 1.f, 0.f, 0.f));

	m_fDuration = 1.f;
	return S_OK;
}

void CBoss_Range_Bullet::Priority_Tick(const _float& fTimeDelta)
{
}

void CBoss_Range_Bullet::Tick(const _float& fTimeDelta)
{
	if (m_fDuration > m_fCurrentTime)
		m_fCurrentTime += fTimeDelta;
	else m_isDead = true;

	//이동
	Move_Up(fTimeDelta);

	//버퍼 업데이트
	_float4x4 TrailMatrix = *m_pTransformCom->Get_WorldFloat4x4();
	// m_pTransformCom->Get_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f), &TrailMatrix);
	m_pTrailVIBufferCom->Add_Trail(fTimeDelta, XMLoadFloat4x4(&TrailMatrix));

}

void CBoss_Range_Bullet::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CBoss_Range_Bullet::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	m_pShaderCom->Begin(6);
	m_pVIBufferCom->Render();

	m_pShaderCom->Begin(6);
	m_pTrailVIBufferCom->Render();


	return S_OK;
}

HRESULT CBoss_Range_Bullet::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STAGE_BOSS, L"Prototype_Component_Shader_VtxPosTex_Skill", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STAGE_BOSS, L"Prototype_Component_VIBuffer_RectZ", L"Com_VIBuffer", reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STAGE_BOSS, L"Prototype_Component_Texture_Monster_Arrow", L"Com_Texture", reinterpret_cast<CComponent**>(&m_pTextureCom[0]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STAGE_BOSS, L"Prototype_Component_Texture_Skill_Tighnari_Normal_Trail", L"Com_TrailTexture", reinterpret_cast<CComponent**>(&m_pTextureCom[1]))))
		return E_FAIL;

	CVIBuffer_Trail::VIBUFFER_TRAIL_DESC TrailDesc{};
	TrailDesc.iMaxTrail = 70;
	TrailDesc.vInitPosA = _float3(-0.3f, 0.f, 0.3f);
	TrailDesc.vInitPosB = _float3(-0.3f, 0.f, -0.3f);
	if (FAILED(__super::Add_Component(LEVEL_STAGE_BOSS, L"Prototype_Component_VIBuffer_Trail", L"Com_TrailBuffer", reinterpret_cast<CComponent**>(&m_pTrailVIBufferCom), &TrailDesc)))
		return E_FAIL;

	return S_OK;
}


HRESULT CBoss_Range_Bullet::Bind_ResourceData()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom[0]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pTextureCom[1]->Bind_ShaderResource(m_pShaderCom, "g_TrailDiffuseTexture", 0)))
		return E_FAIL;

	return S_OK;
}

void CBoss_Range_Bullet::Move_Up(const _float fTimeDelta)
{
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	vPosition += XMLoadFloat4(&m_vTargetLook) * m_fSpeed * fTimeDelta;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);
}

CBoss_Range_Bullet* CBoss_Range_Bullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBoss_Range_Bullet* pInstance = new CBoss_Range_Bullet(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CBoss_Range_Bullet::Clone(void* pArg)
{
	CBoss_Range_Bullet* pInstance = new CBoss_Range_Bullet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CBoss_Range_Bullet::Free()
{
	__super::Free();

	Safe_Release(m_pTrailVIBufferCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);

	for (auto& pTexture : m_pTextureCom)
		Safe_Release(pTexture);

}
