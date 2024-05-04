#include "Slime_Large_Water.h"

#include "GameInstance.h"

CSlime_Large_Water::CSlime_Large_Water(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSlime_Large{ pDevice, pContext}
{
}

CSlime_Large_Water::CSlime_Large_Water(const CSlime_Large_Water& rhs)
	: CSlime_Large{ rhs }
{
}

HRESULT CSlime_Large_Water::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSlime_Large_Water::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CSlime_Large_Water::Priority_Tick(const _float& fTimeDelta)
{
}

void CSlime_Large_Water::Tick(const _float& fTimeDelta)
{
	__super::Change_Animation(fTimeDelta);

	_float4x4 MoveMatrix;
	m_pModelCom->Play_Animation(fTimeDelta, &MoveMatrix);
}

void CSlime_Large_Water::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CSlime_Large_Water::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	m_pColliderCom->Render();

	return S_OK;
}

HRESULT CSlime_Large_Water::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Slime_Water", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	CBounding_AABB::BOUNDING_AABB_DESC Desc{};
	Desc.eType = CCollider::COLLIDER_AABB;
	Desc.vExtents = _float3(0.5f, 0.5f, 0.5f);
	Desc.vCenter = _float3(0.f, Desc.vExtents.y, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider", L"Com_Collider", reinterpret_cast<CComponent**>(&m_pColliderCom), &Desc)))
		return E_FAIL;

	CNavigation::NAVIGATION_DESC NavigationDesc = {};
	NavigationDesc.iIndex = m_iMonsterNavigationIndex;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Navigation_Stage_1", L"Com_Navigation", reinterpret_cast<CComponent**>(&m_pNavigation), &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSlime_Large_Water::Bind_ResourceData()
{
	if (FAILED(__super::Bind_ResourceData()))
		return E_FAIL;

	return S_OK;
}

CSlime_Large_Water* CSlime_Large_Water::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSlime_Large_Water* pInstance = new CSlime_Large_Water(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CSlime_Large_Water::Clone(void* pArg)
{
	CSlime_Large_Water* pInstance = new CSlime_Large_Water(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CSlime_Large_Water::Free()
{
	__super::Free();
}
