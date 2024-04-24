#include "Cell.h"

#include "VIBuffer_Cell.h"

CCell::CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : m_pDevice{ pDevice },
    m_pContext{ pContext }
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CCell::Initialize(const _float3* pPoints, _int iIndex, OPTION OptionType)
{
    memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);

    m_iIndex = iIndex;

    m_OptionType = OptionType;

#ifdef _DEBUG
    m_pVIBufferCom = CVIBuffer_Cell::Create(m_pDevice, m_pContext, pPoints);
    if (nullptr == m_pVIBufferCom)
        return E_FAIL;
#endif // _DEBUG

    return S_OK;
}

_bool CCell::Compare_Points(_fvector vSourPoint, _fvector vDestPoint)
{
    if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vSourPoint))
    {
        if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint)
            || XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
            return true;
    }
    else if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vSourPoint))
    {
        if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint)
            || XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vDestPoint))
            return true;
    }
    else if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), vSourPoint))
    {
        if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), vDestPoint)
            || XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), vDestPoint))
            return true;
    }

    return false;
}

_float CCell::Compute_Height(_fvector vPosition)
{
    _float4 vLocalPos;// 임시로 월드 좌표 넣어주기

    XMStoreFloat4(&vLocalPos, vPosition);

    _vector		vPlane = XMVectorZero();

    vPlane = XMPlaneFromPoints(XMLoadFloat3(&m_vPoints[POINT_A]),
        XMLoadFloat3(&m_vPoints[POINT_B]),
        XMLoadFloat3(&m_vPoints[POINT_C]));

    /*
    ax + by + cz + d = 0
    y = (-ax - cz - d) / b
    */
    return (-XMVectorGetX(vPlane) * vLocalPos.x - XMVectorGetZ(vPlane) * vLocalPos.z - XMVectorGetW(vPlane)) / XMVectorGetY(vPlane);
}

_bool CCell::isIn(_fvector vPosition, _int* pNeighborsIndex)
{
    _vector vSour, vDesc;
    
    for (int i = 0; i < LINE_END; ++i)
    {
        vSour = vPosition - XMLoadFloat3(&m_vPoints[i]);
        _vector vLine = XMLoadFloat3(&m_vPoints[(i + 1) % POINT_END]) - XMLoadFloat3(&m_vPoints[i]);
        vDesc = XMVectorSet(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine), 0.f);

        if (XMVectorGetX(XMVector3Dot(XMVector3Normalize(vSour), XMVector3Normalize(vDesc))) > 0)
        {
            // 선을 나갔다면 그 선에 인접한 인덱스를 넣어주고 false을 리턴.
            *pNeighborsIndex = m_iNeighborIndices[i];
            return false;
        }
    }
    return true;
}

#ifdef _DEBUG
HRESULT CCell::Render()
{
    m_pVIBufferCom->Render();

    return S_OK;
}
#endif // _DEBUG

CCell* CCell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _int iIndex, OPTION OptionType)
{
    CCell* pInstance = new CCell(pDevice, pContext);

    if (FAILED(pInstance->Initialize(pPoints, iIndex, OptionType)))
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
