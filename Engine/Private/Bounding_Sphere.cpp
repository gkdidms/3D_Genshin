#include "Bounding_Sphere.h"

#include "DebugDraw.h"

CBounding_Sphere::CBounding_Sphere()
	: CBounding{}
{
}

HRESULT CBounding_Sphere::Initialize(const void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	const BOUNDING_SPHERE_DESC* pDesc = static_cast<const BOUNDING_SPHERE_DESC*>(pArg);

	m_fRadius = pDesc->fRadius;

	m_pOriginalBox = new BoundingSphere(m_vCenter, m_fRadius);
	m_pBoundingBox = new BoundingSphere(*m_pOriginalBox);
	if (m_pBoundingBox == nullptr)
		return E_FAIL;

	return S_OK;
}

void CBounding_Sphere::Tick(_fmatrix WorldMatrix)
{
	m_pOriginalBox->Transform(*m_pBoundingBox, WorldMatrix);
}

_bool CBounding_Sphere::Intersect(CCollider::TYPE eTargetType, CBounding* pTargetBounding)
{
	_bool isColl = { false };
	switch (eTargetType)
	{
	case Engine::CCollider::COLLIDER_AABB:
	{
		BoundingBox* pDesc = static_cast<BoundingBox*>(pTargetBounding->Get_Desc());
		isColl = m_pBoundingBox->Intersects(*pDesc);

		break;
	}
	case Engine::CCollider::COLLIDER_OBB:
	{
		BoundingOrientedBox* pDesc = static_cast<BoundingOrientedBox*>(pTargetBounding->Get_Desc());
		isColl = m_pBoundingBox->Intersects(*pDesc);
		break;
	}
	case Engine::CCollider::COLLIDER_SPHERE:
	{
		BoundingSphere* pDesc = static_cast<BoundingSphere*>(pTargetBounding->Get_Desc());
		isColl = m_pBoundingBox->Intersects(*pDesc);
		break;
	}
	case Engine::CCollider::COLLIDER_END:
		break;
	default:
		break;
	}

	return isColl;
}

#ifdef _DEBUG
HRESULT CBounding_Sphere::Render(PrimitiveBatch<VertexPositionColor>* pBatch)
{
	DX::Draw(pBatch, *m_pBoundingBox, XMVectorSet(1.f, 0.f, 0.f, 1.f));

	return S_OK;
}
#endif // _DEBUG

CBounding_Sphere* CBounding_Sphere::Create(const void* pArg)
{
	CBounding_Sphere* pInstance = new CBounding_Sphere();

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CBounding_Sphere::Free()
{
	__super::Free();

	Safe_Delete(m_pOriginalBox);
	Safe_Delete(m_pBoundingBox);
}
