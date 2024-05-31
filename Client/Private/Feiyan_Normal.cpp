#include "Feiyan_Normal.h"

#include "GameInstance.h"
#include "Feiyan_Normal_Trail.h"
#include "Effect.h"
#include "Effect_Image.h"


CFeiyan_Normal::CFeiyan_Normal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPlayer_Bullet{ pDevice, pContext }
{
}

CFeiyan_Normal::CFeiyan_Normal(const CFeiyan_Normal& rhs)
	: CPlayer_Bullet{ rhs }
{
}

HRESULT CFeiyan_Normal::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFeiyan_Normal::Initialize(void* pArg)
{

	if (nullptr == pArg)
		return E_FAIL;

	BULLET_DESC* pDesc = static_cast<BULLET_DESC*>(pArg);
	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	_matrix WorldMatrix;
	_matrix HandMatrix = XMMatrixIdentity();
	HandMatrix.r[3] = XMLoadFloat4x4(&pDesc->HandCombinedTransformationMatrix).r[3];
	WorldMatrix = m_pTransformCom->Get_WorldMatrix() * HandMatrix * XMLoadFloat4x4(&pDesc->ParentMatrix);

	m_pTransformCom->Set_WorldMatrix(WorldMatrix);
	m_vTargetPos.y = m_vTargetPos.y + 1.f;
	m_pTransformCom->Set_Scale(0.5f, 1.f, 0.5f);
	m_pTransformCom->LookAt(XMLoadFloat4(&m_vTargetPos));

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CFeiyan_Normal::Priority_Tick(const _float& fTimeDelta)
{

}

void CFeiyan_Normal::Tick(const _float& fTimeDelta)
{
	m_fCurrentTime += fTimeDelta;
	if (m_fCurrentTime > m_fDuration)
	{
		m_isDead = false;
		return;
	}
	m_pTransformCom->Go_Straight(fTimeDelta);

	m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
	
	//회전시키기
	m_pTrailBuffer->Tick(fTimeDelta);

	//Check_Coll();

	m_FireParticle->Tick(fTimeDelta);
}

void CFeiyan_Normal::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONLIGHT, this);

	m_FireParticle->Late_Tick(fTimeDelta);
}

HRESULT CFeiyan_Normal::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	m_pShaderCom->Begin(2);
	m_pVIBufferCom->Render();

	//트레일 버퍼
	m_pTrailBuffer->Render();

#ifdef _DEBUG
	m_pColliderCom->Render();
#endif // _DEBUG


	return S_OK;
}

HRESULT CFeiyan_Normal::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxPosTex_Skill", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Skill_Feiyan_Normal_Fire", L"Com_Texture", reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_VIBuffer_RectZ", L"Com_VIBuffer", reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	CBounding_AABB::BOUNDING_AABB_DESC BoundingBoxDesc{};
	BoundingBoxDesc.eType = CCollider::COLLIDER_AABB;
	BoundingBoxDesc.vExtents = _float3(0.3f, 0.3f, 0.3f);
	BoundingBoxDesc.vCenter = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider", L"Com_Collider", reinterpret_cast<CComponent**>(&m_pColliderCom), &BoundingBoxDesc)))
		return E_FAIL;

	CFeiyan_Normal_Trail::FEIYAN_NORMAL_TRAIL_DESC TrailDesc{};
	TrailDesc.pParentMatrix = m_pTransformCom->Get_WorldFloat4x4();
	m_pTrailBuffer = m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_Skill_Feiyan_Normal_Trail"), &TrailDesc);

	if (nullptr == m_pTrailBuffer)
		return E_FAIL;

	CEffect::EFFECT_DESC ParticleDesc{};

	ParticleDesc.pPlayerMatrix = m_pTransformCom->Get_WorldFloat4x4();
	XMStoreFloat4x4(&ParticleDesc.RotationMatrix, XMMatrixRotationX(XMConvertToRadians(-90.f)));
	ParticleDesc.vPos = _float4(0.f, 0.f, 0.f, 1.f);
	ParticleDesc.vScale = _float3(1.f, 1.f, 1.f);
	ParticleDesc.fDuration = 3.f;
	ParticleDesc.isFollowPlayer = true;

	m_FireParticle = m_pGameInstance->Clone_Object(TEXT("Prototype_GameObject_Effect_Feiyan_Normal_Particle"), &ParticleDesc);

	//glow
	CEffect::EFFECT_DESC StartParticleDesc{};

	StartParticleDesc.pPlayerMatrix = m_pTransformCom->Get_WorldFloat4x4();
	XMStoreFloat4x4(&StartParticleDesc.RotationMatrix, XMMatrixIdentity());
	StartParticleDesc.vPos = _float4(0.f, -0.5f, 0.f, 1.f);
	StartParticleDesc.vScale = _float3(1.f, 1.f, 1.f);
	StartParticleDesc.fDuration = 0.4f;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Feiyan_Normal_Start_Particle"), TEXT("Layer_Particle"), &StartParticleDesc)))
		return E_FAIL;

	//Texture
	CEffect::EFFECT_DESC TextureDesc{};

	TextureDesc.pPlayerMatrix = m_pTransformCom->Get_WorldFloat4x4();
	XMStoreFloat4x4(&TextureDesc.RotationMatrix, XMMatrixIdentity());
	TextureDesc.vPos = _float4(0.f, 0.f, 0.f, 1.f);
	TextureDesc.vScale = _float3(1.5f, 1.5f, 1.5f);
	TextureDesc.fDuration = 0.3f;
	TextureDesc.iMoveType = CEffect_Image::SHRINK;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Feiyan_Normal_Start"), TEXT("Layer_Particle"), &TextureDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CFeiyan_Normal::Bind_ResourceData()
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

CFeiyan_Normal* CFeiyan_Normal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFeiyan_Normal* pInstane = new CFeiyan_Normal(pDevice, pContext);

	if (FAILED(pInstane->Initialize_Prototype()))
		Safe_Release(pInstane);

	return pInstane;
}

CGameObject* CFeiyan_Normal::Clone(void* pArg)
{
	CFeiyan_Normal* pInstane = new CFeiyan_Normal(*this);

	if (FAILED(pInstane->Initialize(pArg)))
		Safe_Release(pInstane);

	return pInstane;
}

void CFeiyan_Normal::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);

	Safe_Release(m_pTrailBuffer);
	Safe_Release(m_FireParticle);
}
