#include "Calculator.h"

#include "GameInstance.h"

CCalculator::CCalculator(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent{ pDevice, pContext },
	m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

CCalculator::CCalculator(const CCalculator& rhs)
	: CComponent { rhs },
	m_pGameInstance{ rhs.m_pGameInstance }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CCalculator::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCalculator::Initialize(void* pArg)
{
	return S_OK;
}

_bool CCalculator::Picking_OnTerrain(_fvector vMouseCurserPos, CTransform* pTransformCom, CVIBuffer_Terrain* pTerrainVIBuffer, const _ulong& dwCntX, const _ulong& dwCntZ, _float3* vWorldMouse)
{
	// 스케일 값
	_float3 vTerrainScale = pTransformCom->Get_Scaled();

	// 뷰포트 -> 투영
	D3D11_VIEWPORT ViewPort;
	_uint iNumViewPorts = 1;
	m_pContext->RSGetViewports(&iNumViewPorts, &ViewPort);

	_float3 vMousePos;

	vMousePos.x = vMouseCurserPos.m128_f32[0] / (ViewPort.Width * 0.5f) - 1.f;
	vMousePos.y = vMouseCurserPos.m128_f32[1] / -(ViewPort.Height * 0.5f) + 1.f;

	vMousePos.z = 0.f;

	// 투영 -> 뷰
	_matrix	matProj;
	matProj = m_pGameInstance->Get_Transform_Inverse_Matrix(CPipeLine::D3DTS_PROJ);
	XMStoreFloat3(&vMousePos, XMVector3TransformCoord(XMLoadFloat3(&vMousePos), matProj));

	//뷰 -> 월드
	_matrix	matView = m_pGameInstance->Get_Transform_Inverse_Matrix(CPipeLine::D3DTS_VIEW);

	_vector	vRayDir, vRayPos; 

	vRayPos = { 0.f, 0.f, 0.f };
	vRayDir = XMLoadFloat3(&vMousePos) - vRayPos;

	vRayPos = XMVector3TransformCoord(vRayPos, matView);
	vRayDir = XMVector3TransformNormal(vRayDir, matView);

	//월드 -> 로컬
	_matrix		matWorld = pTransformCom->Get_WorldMatrix();
	matWorld = XMMatrixInverse(nullptr, matWorld);
	vRayPos = XMVector3TransformCoord(vRayPos, matWorld);
	vRayDir = XMVector3Normalize(XMVector3TransformCoord(vRayDir, matWorld));

	const _float3* pTerrainVtxPos = pTerrainVIBuffer->Get_VtxPos();

	_ulong	dwVtxIdx[3]{};
	_float	fDist(0.f);


	for (_ulong i = 0; i < dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwCntX - 1; ++j)
		{
			_ulong	dwIndex = i * dwCntX + j;
			// 오른쪽 위
			dwVtxIdx[0] = dwIndex + dwCntX;
			dwVtxIdx[1] = dwIndex + dwCntX + 1;
			dwVtxIdx[2] = dwIndex + 1;

			//t = (p - v0) · n / (d · n)
			if (Intersects(vRayPos,
				vRayDir,
				XMLoadFloat3(&pTerrainVtxPos[dwVtxIdx[1]]),
				XMLoadFloat3(&pTerrainVtxPos[dwVtxIdx[0]]),
				XMLoadFloat3(&pTerrainVtxPos[dwVtxIdx[2]]),
				fDist))
			{
				XMStoreFloat3(vWorldMouse, (vRayPos + vRayDir * fDist) * XMLoadFloat3(&vTerrainScale));
				return true;
			}

			// V1 + U(V2 - V1) + V(V3 - V1)

			// 왼쪽 아래
			dwVtxIdx[0] = dwIndex + dwCntX;
			dwVtxIdx[1] = dwIndex + 1;
			dwVtxIdx[2] = dwIndex;

			if (Intersects(vRayPos,
				vRayDir,
				XMLoadFloat3(&pTerrainVtxPos[dwVtxIdx[2]]),
				XMLoadFloat3(&pTerrainVtxPos[dwVtxIdx[1]]),
				XMLoadFloat3(&pTerrainVtxPos[dwVtxIdx[0]]),
				fDist))
			{
				XMStoreFloat3(vWorldMouse, (vRayPos + vRayDir * fDist) * XMLoadFloat3(&vTerrainScale));
				return true;
			}
		}
	}

	*vWorldMouse = _float3(0.f, 0.f, 0.f);
	return false;
}

//_bool CCalculator::Picking_OnMesh(_fvector vMouseCurserPos, CTransform* pTransformCom, CMesh* pMeshBuffer, const _ulong& dwCntVertices, _float3* vWorldMouse)
//{
//	// 스케일 값
//	_float3 vTerrainScale = pTransformCom->Get_Scaled();
//
//	// 뷰포트 -> 투영
//	D3D11_VIEWPORT ViewPort;
//	_uint iNumViewPorts = 1;
//	m_pContext->RSGetViewports(&iNumViewPorts, &ViewPort);
//
//	_float3 vMousePos;
//
//	vMousePos.x = vMouseCurserPos.m128_f32[0] / (ViewPort.Width * 0.5f) - 1.f;
//	vMousePos.y = vMouseCurserPos.m128_f32[1] / -(ViewPort.Height * 0.5f) + 1.f;
//
//	vMousePos.z = 0.f;
//
//	// 투영 -> 뷰
//	_matrix	matProj;
//	matProj = m_pGameInstance->Get_Transform_Inverse_Matrix(CPipeLine::D3DTS_PROJ);
//	XMStoreFloat3(&vMousePos, XMVector3TransformCoord(XMLoadFloat3(&vMousePos), matProj));
//
//	//뷰 -> 월드
//	_matrix	matView = m_pGameInstance->Get_Transform_Inverse_Matrix(CPipeLine::D3DTS_VIEW);
//
//	_vector	vRayDir, vRayPos;
//
//	vRayPos = { 0.f, 0.f, 0.f };
//	vRayDir = XMLoadFloat3(&vMousePos) - vRayPos;
//
//	vRayPos = XMVector3TransformCoord(vRayPos, matView);
//	vRayDir = XMVector3TransformNormal(vRayDir, matView);
//
//	//월드 -> 로컬
//	_matrix		matWorld = pTransformCom->Get_WorldMatrix();
//	matWorld = XMMatrixInverse(nullptr, matWorld);
//	vRayPos = XMVector3TransformCoord(vRayPos, matWorld);
//	vRayDir = XMVector3Normalize(XMVector3TransformCoord(vRayDir, matWorld));
//
//	const _float3* pTerrainVtxPos = pMeshBuffer->Get_VtxPos();
//	const _uint* pVtxIndices = pMeshBuffer->Get_VtxIndices();
//
//	_ulong	dwVtxIdx[3]{};
//	_float	fDist(0.f);
//	_ulong	dwIndex = { 0 };
//	for (_ulong i = 0; i < pMeshBuffer->Get_NumFaces(); ++i)
//	{
//		// 오른쪽 위
//		dwVtxIdx[0] = pVtxIndices[dwIndex++];
//		dwVtxIdx[1] = pVtxIndices[dwIndex++];
//		dwVtxIdx[2] = pVtxIndices[dwIndex++];
//
//		//t = (p - v0) · n / (d · n)
//		if (Intersects(vRayPos,
//			vRayDir,
//			XMLoadFloat3(&pTerrainVtxPos[dwVtxIdx[1]]),
//			XMLoadFloat3(&pTerrainVtxPos[dwVtxIdx[0]]),
//			XMLoadFloat3(&pTerrainVtxPos[dwVtxIdx[2]]),
//			fDist))
//		{
//			XMStoreFloat3(vWorldMouse, (vRayPos + vRayDir * fDist) * XMLoadFloat3(&vTerrainScale));
//			return true;
//		}
//	}
//
//	*vWorldMouse = _float3(0.f, 0.f, 0.f);
//	return false;
//}

CCalculator* CCalculator::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCalculator* pInstance = new CCalculator(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* CCalculator::Clone(void* pArg)
{
	CCalculator* pInstance = new CCalculator(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CCalculator::Free()
{
	__super::Free();
	Safe_Release(m_pGameInstance);
}
