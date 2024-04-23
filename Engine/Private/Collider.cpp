#include "Collider.h"

#include "Bounding_AABB.h"

CCollider::CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent{ pDevice, pContext }
{
}

CCollider::CCollider(const CCollider& rhs)
	: CComponent{ rhs }
{
}

HRESULT CCollider::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCollider::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	COLLIDER_DESC* pDesc = static_cast<COLLIDER_DESC*>(pArg);

	m_ColliderType = pDesc->eType;

	switch (m_ColliderType)
	{
	case COLLIDER_AABB:
		m_pCurrentBounding = CBounding_AABB::Create(pArg);
		break;
	case COLLIDER_OBB:
		break;
	case COLLIDER_SPHERE:
		break;
	case COLLIDER_END:
		break;
	default:
		break;
	}

	if (nullptr == m_pCurrentBounding)
		return E_FAIL;

	return S_OK;
}

void CCollider::Tick()
{
}

CCollider* CCollider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCollider* pInstance = new CCollider(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* CCollider::Clone(void* pArg)
{
	CCollider* pInstance = new CCollider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CCollider::Free()
{
	__super::Free();

	Safe_Release(m_pCurrentBounding);
}
