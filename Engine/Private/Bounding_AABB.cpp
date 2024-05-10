#include "Bounding_AABB.h"

#include "DebugDraw.h"

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

	m_pOriginalBox = new BoundingBox(m_vCenter, m_vExtents);
	m_pBoundingBox = new BoundingBox(*m_pOriginalBox);
	if (nullptr == m_pBoundingBox)
		return E_FAIL;

	return S_OK;
}

void CBounding_AABB::Tick(_fmatrix WorldMatrix)
{
	_matrix		TransformMatrix = WorldMatrix;


	TransformMatrix.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVector3Length(WorldMatrix.r[0]);
	TransformMatrix.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVector3Length(WorldMatrix.r[1]);
	TransformMatrix.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVector3Length(WorldMatrix.r[2]);

	m_pOriginalBox->Transform(*m_pBoundingBox, TransformMatrix);
}

_bool CBounding_AABB::Intersect(CCollider::TYPE eTargetType, CBounding* pTargetBounding)
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
		break;
	case Engine::CCollider::COLLIDER_END:
		break;
	default:
		break;
	}

	return isColl;
}
#ifdef _DEBUG
HRESULT CBounding_AABB::Render(PrimitiveBatch<VertexPositionColor>* pBatch)
{
	DX::Draw(pBatch, *m_pBoundingBox, XMVectorSet(0.f, 1.f, 0.f, 1.f));

	return S_OK;
}
#endif // _DEBUG

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

	Safe_Delete(m_pOriginalBox);
	Safe_Delete(m_pBoundingBox);
}
