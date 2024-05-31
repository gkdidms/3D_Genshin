#include "Hili_Fire.h"

#include "GameInstance.h"
#include "BT_Hili.h"



CHili_Fire::CHili_Fire(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CHili{ pDevice, pContext }
{
}

CHili_Fire::CHili_Fire(const CHili_Fire& rhs)
	: CHili{ rhs }
{
}

HRESULT CHili_Fire::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHili_Fire::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (FAILED(__super::Ready_Object()))
		return E_FAIL;

	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ 0, true, true, true });

	return S_OK;
}

void CHili_Fire::Priority_Tick(const _float& fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CHili_Fire::Tick(const _float& fTimeDelta)
{
	m_pBT->Tick(fTimeDelta);
	Change_Animation(fTimeDelta);

	_float4x4 MoveMatrix;
	XMStoreFloat4x4(&MoveMatrix, XMMatrixIdentity());
	m_pModelCom->Play_Animation(fTimeDelta, &MoveMatrix);

	if (!m_isColl)
	{
		XMStoreFloat4x4(&MoveMatrix, XMLoadFloat4x4(&MoveMatrix) * -1.f);
		m_pTransformCom->Go_Run(XMLoadFloat4x4(&MoveMatrix), m_pNavigation);
	}
	m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());

	__super::Tick(fTimeDelta);
}

void CHili_Fire::Late_Tick(const _float& fTimeDelta)
{
	SetUp_OnTerrain(fTimeDelta);

	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);

	__super::Late_Tick(fTimeDelta);
}

HRESULT CHili_Fire::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

#ifdef _DEBUG
	m_pColliderCom->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CHili_Fire::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Hili_Fire", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	CBounding_AABB::BOUNDING_AABB_DESC Desc{};
	Desc.eType = CCollider::COLLIDER_AABB;
	Desc.vExtents = _float3(0.5f, 0.7f, 0.5f);
	Desc.vCenter = _float3(0.f, Desc.vExtents.y, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider", L"Com_Collider", reinterpret_cast<CComponent**>(&m_pColliderCom), &Desc)))
		return E_FAIL;

	CNavigation::NAVIGATION_DESC NavigationDesc = {};
	NavigationDesc.iIndex = m_iMonsterNavigationIndex;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Navigation", L"Com_Navigation", reinterpret_cast<CComponent**>(&m_pNavigation), &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHili_Fire::Bind_ResourceData()
{
	if (FAILED(__super::Bind_ResourceData()))
		return E_FAIL;

	return S_OK;
}

void CHili_Fire::Change_Animation(const _float& fTimeDelta)
{
	__super::Change_Animation(fTimeDelta);
}

CHili_Fire* CHili_Fire::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHili_Fire* pInstance = new CHili_Fire(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CHili_Fire::Clone(void* pArg)
{
	CHili_Fire* pInstance = new CHili_Fire(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CHili_Fire::Free()
{
	__super::Free();
}
