#include "Cell.h"

#include "VIBuffer_Cell.h"

CCell::CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : m_pDevice{ pDevice },
    m_pContext{ pContext }
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CCell::Initialize(const _float3* pPoints, _int iIndex)
{
    memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);

    m_iIndex = iIndex;

#ifdef _DEBUG
    m_pVIBufferCom = CVIBuffer_Cell::Create(m_pDevice, m_pContext, pPoints);
    if (nullptr == m_pVIBufferCom)
        return E_FAIL;
#endif // _DEBUG

    return S_OK;
}

#ifdef _DEBUG
HRESULT CCell::Render()
{
    m_pVIBufferCom->Render();

    return S_OK;
}
#endif // _DEBUG

CCell* CCell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _int iIndex)
{
    CCell* pInstance = new CCell(pDevice, pContext);

    if (FAILED(pInstance->Initialize(pPoints, iIndex)))
        Safe_Release(pInstance);

    return pInstance;
}

void CCell::Free()
{
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);

#ifdef _DEBUG
    Safe_Release(m_pVIBufferCom);
#endif // _DEBUG
}
