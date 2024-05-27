#include "Hili_Weapon_Crossbow.h"

#include "GameInstance.h"

#include "Hili.h"
#include "Hili_Arrow.h"
#include "Player.h"

CHili_Weapon_Crossbow::CHili_Weapon_Crossbow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CHili_Weapon{ pDevice, pContext}
{
}

CHili_Weapon_Crossbow::CHili_Weapon_Crossbow(const CHili_Weapon_Crossbow& rhs)
	: CHili_Weapon{ rhs}
{
}

HRESULT CHili_Weapon_Crossbow::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHili_Weapon_Crossbow::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(-90.f));
	//m_pTransformCom->Turn(XMVectorSet(0.f, 0.f, 0.f, 0.f), 2.f);

	return S_OK;
}

void CHili_Weapon_Crossbow::Priority_Tick(const _float& fTimeDelta)
{
}

void CHili_Weapon_Crossbow::Tick(const _float& fTimeDelta)
{
	_matrix SocketMatrix = XMLoadFloat4x4(m_SocketMatrix);

	SocketMatrix.r[0] = XMVector3Normalize(SocketMatrix.r[0]);
	SocketMatrix.r[1] = XMVector3Normalize(SocketMatrix.r[1]);
	SocketMatrix.r[2] = XMVector3Normalize(SocketMatrix.r[2]);

	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_TargetMatrix));
	m_pColliderCom->Tick(XMLoadFloat4x4(&m_WorldMatrix));

	Create_Arrow();
}

void CHili_Weapon_Crossbow::Late_Tick(const _float& fTimeDelta)
{
	if (m_PreState != *m_pState)
		m_PreState = *m_pState;

	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CHili_Weapon_Crossbow::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

#ifdef _DEBUG
	m_pColliderCom->Render();
#endif // _DEBUG

	return S_OK;
}

HRESULT CHili_Weapon_Crossbow::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Hili_Weapon_CrossBow", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
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

HRESULT CHili_Weapon_Crossbow::Create_Arrow()
{
	if (m_PreState == *m_pState)
		return S_OK;

	if (*m_pState == CHili::HILI_NORMAL_ATK)
	{
		CHili_Arrow::BULLET_DESC ArrowDesc{};
		ArrowDesc.fSpeedPecSec = 20.f;
		ArrowDesc.HandCombinedTransformationMatrix = *m_SocketMatrix;
		CTransform* pPlayerTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_GameObject_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform"), 0));
		ArrowDesc.ParentMatrix = *m_TargetMatrix;
		ArrowDesc.pTargetPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);

		if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Skill_Hili_Arrow"), TEXT("Layer_MonBullet"), &ArrowDesc)))
			return E_FAIL;
	}

	return S_OK;
}

CHili_Weapon_Crossbow* CHili_Weapon_Crossbow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHili_Weapon_Crossbow* pInstance = new CHili_Weapon_Crossbow(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CHili_Weapon_Crossbow::Clone(void* pArg)
{
	CHili_Weapon_Crossbow* pInstance = new CHili_Weapon_Crossbow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CHili_Weapon_Crossbow::Free()
{
	__super::Free();
}
