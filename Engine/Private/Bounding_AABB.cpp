#include "Bounding_AABB.h"

CBounding_AABB::CBounding_AABB()
	: CBounding{}
{
}

HRESULT CBounding_AABB::Initialize(const void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	const BOUNDING_AABB_DESC* pDesc = static_cast<const BOUNDING_AABB_DESC*>(pArg);
	m_vCenter = pDesc->vCenter;
	m_vExtents = pDesc->vExtents;

	m_pBoundingBox = new BoundingBox(m_vCenter, m_vExtents);
	if (nullptr == m_pBoundingBox)
		return E_FAIL;

	return S_OK;
}

void CBounding_AABB::Tick()
{
}

HRESULT CBounding_AABB::Render()
{
	return S_OK;
}

CBounding_AABB* CBounding_AABB::Create(const void* pArg)
{
	CBounding_AABB* pInstance = new CBounding_AABB();

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CBounding_AABB::Free()
{
	__super::Free();
}
