#include "Tighnari_Burst.h"

#include "GameInstance.h"

CTighnari_Burst::CTighnari_Burst(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPlayer_Bullet{ pDevice, pContext }
{
}

CTighnari_Burst::CTighnari_Burst(const CTighnari_Burst& rhs)
	: CPlayer_Bullet{ rhs }
{
}

HRESULT CTighnari_Burst::Initialize_Prototype()
{
	return S_OK;
}

//5개가 나가는데 각각 나가는 방향이 다름.
HRESULT CTighnari_Burst::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	TIGHNARI_BURST_DESC* pDesc = static_cast<TIGHNARI_BURST_DESC*>(pArg);
	m_fSpeed = pDesc->fSpeedPecSec;
	m_iIndex = pDesc->iIndex;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	_matrix WorldMatrix;
	_matrix HandMatrix = XMMatrixIdentity();
	m_pTransformCom->WorldRotation(XMVectorSet(1.f, 0.f, 0.f, 0.f), XMConvertToRadians(180.f));
	HandMatrix.r[3] = XMLoadFloat4x4(&pDesc->HandCombinedTransformationMatrix).r[3];
	WorldMatrix = m_pTransformCom->Get_WorldMatrix() * HandMatrix * XMLoadFloat4x4(&pDesc->ParentMatrix);

	m_pTransformCom->Set_WorldMatrix(WorldMatrix);
	
	m_vTargetPos.y = m_vTargetPos.y + 1.5f;

	m_fHeight = pDesc->ParentMatrix.m[3][1]; // y 값 저장

	XMStoreFloat4(&m_vTargetLook, XMVector3Normalize(XMLoadFloat4(&m_vTargetPos) - m_pTransformCom->Get_State(CTransform::STATE_POSITION)));

	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ 1, false, false, false });

	return S_OK;
}

void CTighnari_Burst::Priority_Tick(const _float& fTimeDelta)
{
}

void CTighnari_Burst::Tick(const _float& fTimeDelta)
{
	//이동
	m_isDead = Move_Arrow(fTimeDelta);

	//버퍼 업데이트
	_float4x4 TrailMatrix = *m_pTransformCom->Get_WorldFloat4x4();
	m_pTransformCom->Get_Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f), &TrailMatrix);
	m_pTrailVIBufferCom->Add_Trail(fTimeDelta, XMLoadFloat4x4(&TrailMatrix));

	m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());

	_float4x4 MoveMatrix;
	m_pModelCom->Play_Animation(fTimeDelta, &MoveMatrix);
 }

void CTighnari_Burst::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONLIGHT, this);
}

HRESULT CTighnari_Burst::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (int i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
		{
			if (FAILED(m_pTextureCom[0]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
				return E_FAIL;
		}

		m_pShaderCom->Begin(0);
		m_pModelCom->Render(i);
	}

	if (FAILED(Bind_TrailResourceData()))
		return E_FAIL;

	m_pTrailShaderCom->Begin(4);
	m_pTrailVIBufferCom->Render();

#ifdef _DEBUG
	m_pColliderCom->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CTighnari_Burst::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimMesh_Skill", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_FlowerArrow", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxPosTex_Skill", L"Com_TrailShader", reinterpret_cast<CComponent**>(&m_pTrailShaderCom))))
		return E_FAIL;
	
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Skill_Tighnari_Burst", L"Com_Texture", reinterpret_cast<CComponent**>(&m_pTextureCom[0]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_Skill_Tighnari_Burst_Trail", L"Com_TrailTexture", reinterpret_cast<CComponent**>(&m_pTextureCom[1]))))
		return E_FAIL;

	CBounding_AABB::BOUNDING_AABB_DESC BoundingBoxDesc{};
	BoundingBoxDesc.eType = CCollider::COLLIDER_AABB;
	BoundingBoxDesc.vExtents = _float3(0.2f, 0.2f, 0.2f);
	BoundingBoxDesc.vCenter = _float3(0.f, BoundingBoxDesc.vExtents.y, 0.f);

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

HRESULT CTighnari_Burst::Bind_ResourceData()
{

	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	/*if (FAILED(m_pShaderCom->Bind_RawValue("g_fFar", m_pGameInstance->Get_CamFar(), sizeof(_float))))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CTighnari_Burst::Bind_TrailResourceData()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pTrailShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pTrailShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pTrailShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom[1]->Bind_ShaderResource(m_pTrailShaderCom, "g_TrailDiffuseTexture", 0)))
		return E_FAIL;

	return S_OK;
}

_bool CTighnari_Burst::Move_Arrow(const _float& fTimeDelta)
{
	m_fAccelTime += fTimeDelta * 0.0001f;

	_float fTemp = (m_fPower * m_fAccelTime) - (9.8f * m_fAccelTime * m_fAccelTime * 0.5f);

	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float fY = XMVectorGetY(vPosition);
	vPosition += XMLoadFloat4(&m_vTargetLook) * m_fSpeed * fTimeDelta;
	vPosition = XMVectorSetY(vPosition, fY - fTemp);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

	if (m_fHeight >= XMVectorGetY(vPosition))
	{
		// 충돌했을때 이펙트 출력
		return true;
	}

	return false;
}

CTighnari_Burst* CTighnari_Burst::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTighnari_Burst* pInstance = new CTighnari_Burst(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CTighnari_Burst::Clone(void* pArg)
{
	CTighnari_Burst* pInstance = new CTighnari_Burst(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CTighnari_Burst::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pTrailShaderCom);
	for (auto& pTexture : m_pTextureCom)
		Safe_Release(pTexture);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTrailVIBufferCom);
}
