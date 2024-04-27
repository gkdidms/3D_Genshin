#include "Bounding_OBB.h"

#include "DebugDraw.h"

CBounding_OBB::CBounding_OBB()
	: CBounding{}
{
}

HRESULT CBounding_OBB::Initialize(const void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	const BOUNDING_OBB_DESC* pDesc = static_cast<const BOUNDING_OBB_DESC*>(pArg);

	_float4		vQuaternion;
	XMStoreFloat4(&vQuaternion, XMQuaternionRotationRollPitchYaw(pDesc->vRotation.x, pDesc->vRotation.y, pDesc->vRotation.z));

	m_vCenter = pDesc->vCenter;
	m_vExtents = pDesc->vExtents;

	m_pOriginalBox = new BoundingOrientedBox(m_vCenter, m_vExtents, vQuaternion);
	m_pBoundingBox = new BoundingOrientedBox(*m_pOriginalBox);
	if (m_pBoundingBox == nullptr)
		return E_FAIL;

	return S_OK;
}

void CBounding_OBB::Tick(_fmatrix WorldMatrix)
{
	m_pOriginalBox->Transform(*m_pBoundingBox, WorldMatrix);
}

#ifdef _DEBUG
HRESULT CBounding_OBB::Render(PrimitiveBatch<VertexPositionColor>* pBatch)
{
	DX::Draw(pBatch, *m_pBoundingBox);

	return S_OK;
}
#endif // _DEBUG

CBounding_OBB* CBounding_OBB::Create(const void* pArg)
{
	CBounding_OBB* pInstance = new CBounding_OBB();

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CBounding_OBB::Free()
{
	__super::Free();
}
