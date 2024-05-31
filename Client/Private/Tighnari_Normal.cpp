#include "Tighnari_Normal.h"

#include "GameInstance.h"
#include "Effect.h"
#include "Effect_Image.h"

CTighnari_Normal::CTighnari_Normal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPlayer_Bullet{ pDevice, pContext }
{
}

CTighnari_Normal::CTighnari_Normal(const CTighnari_Normal& rhs)
	: CPlayer_Bullet{ rhs }
{
}

HRESULT CTighnari_Normal::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTighnari_Normal::Initialize(void* pArg)
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
	_matrix HandMatrix = XMMatrixIdentity();
	HandMatrix.r[3] = XMLoadFloat4x4(&pDesc->HandCombinedTransformationMatrix).r[3];
	WorldMatrix = m_pTransformCom->Get_WorldMatrix() * HandMatrix * XMLoadFloat4x4(&pDesc->ParentMatrix);

	m_pTransformCom->Set_WorldMatrix(WorldMatrix);
	m_vTargetPos.y = m_vTargetPos.y + 1.f;
	m_pTransformCom->Set_Scale(0.3f, 1.f, 0.3f);

	m_fHeight = pDesc->ParentMatrix.m[3][1]; // y 값 저장

	XMStoreFloat4(&m_vTargetLook, XMVector3Normalize(XMLoadFloat4(&m_vTargetPos) - m_pTransformCom->Get_State(CTransform::STATE_POSITION)));

	return S_OK;
}

void CTighnari_Normal::Priority_Tick(const _float& fTimeDelta)
{
}

void CTighnari_Normal::Tick(const _float& fTimeDelta)
{
	//이동
	m_isDead = Move_Arrow(fTimeDelta);

	//버퍼 업데이트
	_float4x4 TrailMatrix = *m_pTransformCom->Get_WorldFloat4x4();
	m_pTransformCom->Get_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f), &TrailMatrix);
	m_pTrailVIBufferCom->Add_Trail(fTimeDelta, XMLoadFloat4x4(&TrailMatrix));

	m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
 }

void CTighnari_Normal::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONLIGHT, this);
}

HRESULT CTighnari_Normal::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	m_pShaderCom->Begin(3);
	m_pVIBufferCom->Render();

	m_pShaderCom->Begin(4);
	m_pTrailVIBufferCom->Render();

#ifdef _DEBUG
	m_pColliderCom->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CTighnari_Normal::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxPosTex_Skill", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Skill_Tighnari_Normal_Trail", L"Com_Texture", reinterpret_cast<CComponent**>(&m_pTextureCom[0]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Skill_Tighnari_Normal_Trail", L"Com_TrailTexture", reinterpret_cast<CComponent**>(&m_pTextureCom[1]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_VIBuffer_RectZ", L"Com_VIBuffer", reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;


	CBounding_AABB::BOUNDING_AABB_DESC BoundingBoxDesc{};
	BoundingBoxDesc.eType = CCollider::COLLIDER_AABB;
	BoundingBoxDesc.vExtents = _float3(0.3f, 0.3f, 0.3f);
	BoundingBoxDesc.vCenter = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider", L"Com_Collider", reinterpret_cast<CComponent**>(&m_pColliderCom), &BoundingBoxDesc)))
		return E_FAIL;

	CVIBuffer_Trail::VIBUFFER_TRAIL_DESC TrailDesc{};
	TrailDesc.iMaxTrail = 30;
	TrailDesc.vInitPosA = _float3(-0.3f, 0.3f, 0.f);
	TrailDesc.vInitPosB = _float3(-0.3f, -0.3f, 0.f);
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_VIBuffer_Trail", L"Com_TrailBuffer", reinterpret_cast<CComponent**>(&m_pTrailVIBufferCom), &TrailDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTighnari_Normal::Bind_ResourceData()
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

_bool CTighnari_Normal::Move_Arrow(const _float& fTimeDelta)
{
	m_fAccelTime += fTimeDelta * 0.01f;

	_float fTemp = (m_fPower * m_fAccelTime) - (9.8f * m_fAccelTime * m_fAccelTime * 0.5f);

	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float fY = XMVectorGetY(vPosition);
	vPosition += XMLoadFloat4(&m_vTargetLook) * m_fSpeed * fTimeDelta;
	vPosition = XMVectorSetY(vPosition, fY - fTemp);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

	if (m_fHeight >= XMVectorGetY(vPosition))
	{
		CEffect::EFFECT_DESC EffectDesc{};

		EffectDesc.pPlayerMatrix = m_pTransformCom->Get_WorldFloat4x4();
		XMStoreFloat4x4(&EffectDesc.RotationMatrix, XMMatrixIdentity());
		EffectDesc.vPos = _float4(0.f, 0.5f, 0.f, 1.f);
		EffectDesc.vScale = _float3(1.f, 1.f, 1.f);
		EffectDesc.fDuration = 0.2f;
		EffectDesc.iMoveType = CEffect_Image::SHRINK;

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Tighnari_Effect_Arrow_Start"), TEXT("Layer_Trail"), &EffectDesc)))
			return E_FAIL;

		// 충돌했을때 이펙트 출력
		return true;
	}

	return false;
}

CTighnari_Normal* CTighnari_Normal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTighnari_Normal* pInstance = new CTighnari_Normal(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CTighnari_Normal::Clone(void* pArg)
{
	CTighnari_Normal* pInstance = new CTighnari_Normal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CTighnari_Normal::Free()
{
	__super::Free();

	for (auto& pTexture : m_pTextureCom)
		Safe_Release(pTexture);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTrailVIBufferCom);
}
