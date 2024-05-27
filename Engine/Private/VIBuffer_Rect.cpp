#include "VIBuffer_Rect.h"

CVIBuffer_Rect::CVIBuffer_Rect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer{ pDevice, pContext }
{
}

CVIBuffer_Rect::CVIBuffer_Rect(const CVIBuffer_Rect& rhs)
	: CVIBuffer{rhs}
{
}

HRESULT CVIBuffer_Rect::Initialize_Prototype()
{
	// Á¤Á¡ 
//	UINT ByteWidth;
//	D3D11_USAGE Usage;
//	UINT BindFlags;
//	UINT CPUAccessFlags;
//	UINT MiscFlags;
//	UINT StructureByteStride;
//} 	D3D11_BUFFER_DESC;
	m_GIFormat = DXGI_FORMAT_R16_UINT;
	m_Primitive_Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_iNumVertexBuffers = 1;
	m_iNumVertices = 4;
	m_iVertexStride = sizeof(VXTPOSTEX);

	m_Buffer_Desc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_Buffer_Desc.Usage = D3D11_USAGE_DEFAULT;
	m_Buffer_Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_Buffer_Desc.CPUAccessFlags = 0;
	m_Buffer_Desc.MiscFlags = 0;
	m_Buffer_Desc.StructureByteStride = m_iVertexStride;

	VXTPOSTEX* pVertexts = new VXTPOSTEX[m_iNumVertices];

	pVertexts[0].vPosition = _float3{ -0.5f, 0.5f, 0.f };
	pVertexts[0].vTexcoord = _float2{ 0.f, 0.f };

	pVertexts[1].vPosition = _float3{ 0.5f, 0.5f, 0.f };
	pVertexts[1].vTexcoord = _float2{ 1.f, 0.f };

	pVertexts[2].vPosition = _float3{ 0.5f, -0.5f, 0.f };
	pVertexts[2].vTexcoord = _float2{ 1.f, 1.f };

	pVertexts[3].vPosition = _float3{ -0.5f, -0.5f, 0.f };
	pVertexts[3].vTexcoord = _float2{ 0.f, 1.f };

	m_ResourceData.pSysMem = pVertexts;

	__super::Create_Buffer(&m_pVB);

	Safe_Delete_Array(pVertexts);

	m_iNumIndices = 6;
	m_iIndexStride = 2;

	m_Buffer_Desc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_Buffer_Desc.Usage = D3D11_USAGE_DEFAULT;
	m_Buffer_Desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_Buffer_Desc.CPUAccessFlags = 0;
	m_Buffer_Desc.MiscFlags = 0;
	m_Buffer_Desc.StructureByteStride = 0;

	_ushort* pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;

	pIndices[3] = 0;
	pIndices[4] = 2;
	pIndices[5] = 3;

	m_ResourceData.pSysMem = pIndices;

	__super::Create_Buffer(&m_pIB);

	Safe_Delete_Array(pIndices);
	
	return S_OK;
}

HRESULT CVIBuffer_Rect::Initialize(void* pArg)
{
	return S_OK;
}

CVIBuffer_Rect* CVIBuffer_Rect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Rect* pInstance = new CVIBuffer_Rect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* CVIBuffer_Rect::Clone(void* pArg)
{
	CComponent* pInstance = new CVIBuffer_Rect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CVIBuffer_Rect::Free()
{
	__super::Free();
}
