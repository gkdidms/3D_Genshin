#include "FireCore.h"

#include "GameInstance.h"

CFireCore::CFireCore(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSceneObj{ pDevice, pContext}
{
}

CFireCore::CFireCore(const CFireCore& rhs)
	: CSceneObj{ rhs }
{
}

HRESULT CFireCore::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFireCore::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (nullptr != pArg)
	{
		FIRECORE_DESC* pDesc = static_cast<FIRECORE_DESC*>(pArg);
		m_pTargetMatrix = pDesc->pTargetMatrix;
	}

	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ FIRECORE_REST, false, false ,false });

	m_CurrentState = FIRECORE_REST;

	return S_OK;
}

void CFireCore::Priority_Tick(const _float& fTimeDelta)
{
}

void CFireCore::Tick(const _float& fTimeDelta)
{
	Change_Animation(fTimeDelta);

	_float4x4 MoveMatrix;
	m_pModelCom->Play_Animation(fTimeDelta, &MoveMatrix);

	m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void CFireCore::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CFireCore::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

#ifdef _DEBUG
	m_pColliderCom->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CFireCore::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_FireCore", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	CBounding_AABB::BOUNDING_AABB_DESC Desc{};

	Desc.eType = CCollider::COLLIDER_AABB;
	Desc.vExtents = _float3(1.f, 1.f, 1.f);
	Desc.vCenter = _float3(0.f, Desc.vExtents.y, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider", L"Com_Collider", reinterpret_cast<CComponent**>(&m_pColliderCom), &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CFireCore::Bind_ResourceData()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CFireCore::Change_Animation(const _float& fTimeDelta)
{
	//플레이어가 반경 안에 들어오면 START
	_vector vTargetPos = XMLoadFloat4x4(m_pTargetMatrix).r[3];
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_vector vDistance = vTargetPos - vPos;
	_float fDistance = sqrtf(XMVectorGetX(vDistance) * XMVectorGetX(vDistance) + XMVectorGetY(vDistance) * XMVectorGetY(vDistance) + XMVectorGetZ(vDistance) * XMVectorGetZ(vDistance));

	m_CurrentState = fDistance <= m_fDistanceToTarget ? FIRECORE_START : FIRECORE_END;

	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ _uint(m_CurrentState), false, false, false });
}

CFireCore* CFireCore::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFireCore* pInstance = new CFireCore(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CFireCore::Clone(void* pArg)
{
	CFireCore* pInstance = new CFireCore(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CFireCore::Free()
{
	__super::Free();
}
