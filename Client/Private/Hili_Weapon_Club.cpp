#include "Hili_Weapon_Club.h"

#include "GameInstance.h"

CHili_Weapon_Club::CHili_Weapon_Club(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CHili_Weapon{ pDevice, pContext}
{
}

CHili_Weapon_Club::CHili_Weapon_Club(const CHili_Weapon_Club& rhs)
	: CHili_Weapon{ rhs}
{
}

HRESULT CHili_Weapon_Club::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHili_Weapon_Club::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	return S_OK;
}

void CHili_Weapon_Club::Priority_Tick(const _float& fTimeDelta)
{
}

void CHili_Weapon_Club::Tick(const _float& fTimeDelta)
{
	_matrix SocketMatrix = XMLoadFloat4x4(m_SocketMatrix);

	SocketMatrix.r[0] = XMVector3Normalize(SocketMatrix.r[0]);
	SocketMatrix.r[1] = XMVector3Normalize(SocketMatrix.r[1]);
	SocketMatrix.r[2] = XMVector3Normalize(SocketMatrix.r[2]);

	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_TargetMatrix));
	
	_matrix CollMatrix = m_pTransformCom->Get_WorldMatrix() * XMMatrixRotationX(XMConvertToRadians(-90.f)) * SocketMatrix * XMLoadFloat4x4(m_TargetMatrix);
	m_pColliderCom->Tick(CollMatrix);
}

void CHili_Weapon_Club::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CHili_Weapon_Club::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

#ifdef _DEBUG
	m_pColliderCom->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CHili_Weapon_Club::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Hili_Weapon_Club", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	CBounding_OBB::BOUNDING_OBB_DESC Desc{};
	Desc.eType = CCollider::COLLIDER_OBB;
	Desc.vExtents = _float3(0.2f, 0.4f, 0.2f);
	Desc.vCenter = _float3(0.f, Desc.vExtents.y, 0.f);
	Desc.vRotation = _float3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Collider", L"Com_Collider", reinterpret_cast<CComponent**>(&m_pColliderCom), &Desc)))
		return E_FAIL;

	return S_OK;
}

CHili_Weapon_Club* CHili_Weapon_Club::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHili_Weapon_Club* pInstance = new CHili_Weapon_Club(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CHili_Weapon_Club::Clone(void* pArg)
{
	CHili_Weapon_Club* pInstance = new CHili_Weapon_Club(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CHili_Weapon_Club::Free()
{
	__super::Free();
}
