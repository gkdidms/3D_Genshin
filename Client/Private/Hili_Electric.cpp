#include "Hili_Electric.h"

#include "GameInstance.h"
#include "BT_Hili.h"



CHili_Electric::CHili_Electric(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CHili{ pDevice, pContext }
{
}

CHili_Electric::CHili_Electric(const CHili_Electric& rhs)
	: CHili{ rhs }
{
}

HRESULT CHili_Electric::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHili_Electric::Initialize(void* pArg)
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

void CHili_Electric::Priority_Tick(const _float& fTimeDelta)
{
}

void CHili_Electric::Tick(const _float& fTimeDelta)
{
	m_pBT->Tick(fTimeDelta);
	Change_Animation(fTimeDelta);

	_float4x4 MoveMatrix;
	XMStoreFloat4x4(&MoveMatrix, XMMatrixIdentity());
	m_pModelCom->Play_Animation(fTimeDelta, &MoveMatrix);

	XMStoreFloat4x4(&MoveMatrix, XMLoadFloat4x4(&MoveMatrix) * -1.f);
	m_pTransformCom->Go_Run(XMLoadFloat4x4(&MoveMatrix), m_pNavigation);

	m_pColliderCom->Tick(m_pTransformCom->Get_WorldMatrix());

	__super::Tick(fTimeDelta);
}

void CHili_Electric::Late_Tick(const _float& fTimeDelta)
{
	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	vPos = XMVectorSetY(vPos, m_pNavigation->Compute_Height(vPos));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);

	__super::Late_Tick(fTimeDelta);
}

HRESULT CHili_Electric::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

#ifdef _DEBUG
	m_pColliderCom->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CHili_Electric::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Hili_Electric", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	CBounding_AABB::BOUNDING_AABB_DESC Desc{};
	Desc.eType = CCollider::COLLIDER_AABB;
	Desc.vExtents = _float3(0.5f, 0.5f, 0.5f);
	Desc.vCenter = _float3(0.f, Desc.vExtents.y, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider", L"Com_Collider", reinterpret_cast<CComponent**>(&m_pColliderCom), &Desc)))
		return E_FAIL;

	CNavigation::NAVIGATION_DESC NavigationDesc = {};
	NavigationDesc.iIndex = m_iMonsterNavigationIndex;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Navigation", L"Com_Navigation", reinterpret_cast<CComponent**>(&m_pNavigation), &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CHili_Electric::Bind_ResourceData()
{
	if (FAILED(__super::Bind_ResourceData()))
		return E_FAIL;

	return S_OK;
}

void CHili_Electric::Change_Animation(const _float& fTimeDelta)
{
	__super::Change_Animation(fTimeDelta);
}

CHili_Electric* CHili_Electric::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHili_Electric* pInstance = new CHili_Electric(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CHili_Electric::Clone(void* pArg)
{
	CHili_Electric* pInstance = new CHili_Electric(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CHili_Electric::Free()
{
	__super::Free();
}
