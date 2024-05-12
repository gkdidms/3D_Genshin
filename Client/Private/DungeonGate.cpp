#include "DungeonGate.h"

#include "GameInstance.h"

CDungeonGate::CDungeonGate(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSceneObj{ pDevice, pContext}
{
}

CDungeonGate::CDungeonGate(const CDungeonGate& rhs)
	: CSceneObj{ rhs }
{
}

HRESULT CDungeonGate::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDungeonGate::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CDungeonGate::Priority_Tick(const _float& fTimeDelta)
{
}

void CDungeonGate::Tick(const _float& fTimeDelta)
{
	_float4x4 MoveMatrix;
	m_pModelCom->Play_Animation(fTimeDelta, &MoveMatrix);

	m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());
}

void CDungeonGate::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CDungeonGate::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

#ifdef _DEBUG
	m_pColliderCom->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CDungeonGate::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_DungeonGate", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	CBounding_AABB::BOUNDING_AABB_DESC Desc{};

	Desc.eType = CCollider::COLLIDER_AABB;
	Desc.vExtents = _float3(0.6f, 0.5f, 0.6f);
	Desc.vCenter = _float3(0.f, Desc.vExtents.y, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider", L"Com_Collider", reinterpret_cast<CComponent**>(&m_pColliderCom), &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDungeonGate::Bind_ResourceData()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CDungeonGate::Change_Animation(const _float& fTimeDelta)
{
}

CDungeonGate* CDungeonGate::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDungeonGate* pInstance = new CDungeonGate(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CDungeonGate::Clone(void* pArg)
{
	CDungeonGate* pInstance = new CDungeonGate(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CDungeonGate::Free()
{
	__super::Free();
}
