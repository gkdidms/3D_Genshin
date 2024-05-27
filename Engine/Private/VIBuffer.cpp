#include "VIBuffer.h"

CVIBuffer::CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent{pDevice, pContext}
{
}

CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
	: CComponent{rhs},
	m_pIB { rhs.m_pIB},
	m_pVB{ rhs.m_pVB },
	m_Buffer_Desc {rhs.m_Buffer_Desc},
	m_ResourceData { rhs.m_ResourceData},
	m_iNumVertices {rhs.m_iNumVertices},
	m_iVertexStride { rhs.m_iVertexStride},
	m_iNumIndices { rhs.m_iNumIndices},
	m_iIndexStride { rhs.m_iIndexStride},
	m_GIFormat { rhs.m_GIFormat},
	m_Primitive_Topology{ rhs.m_Primitive_Topology },
	m_iNumVertexBuffers { rhs.m_iNumVertexBuffers }
{
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}

HRESULT CVIBuffer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	ID3D11Buffer* pVertices[] = {
		m_pVB,
	};

	_uint pStrideVertices[] = {
		m_iVertexStride,
	};

	_uint pStartVertices[] = {
		0,
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertices, pStrideVertices, pStartVertices);
	m_pContext->IASetIndexBuffer(m_pIB, m_GIFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_Primitive_Topology);
	
	m_pContext->DrawIndexed(m_iNumIndices, 0, 0);

	return S_OK;
}

HRESULT CVIBuffer::Create_Buffer(ID3D11Buffer** pOut)
{
	if (FAILED(m_pDevice->CreateBuffer(&m_Buffer_Desc, &m_ResourceData, pOut)))
		return E_FAIL;

	return S_OK;
}

void CVIBuffer::Free()
{
	__super::Free();
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}
