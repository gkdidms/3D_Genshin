#pragma once
#include "Base.h"

/* 1. �׺���̼��� �����ϴ� �ϳ��� �ﰡ��. */
/* 2. �ﰢ���� �����ϴ� ������ ������ �����Ѵ�. */
/* 3. �� ���� �����Ͽ� ��ü�� �����ġ�� �ȿ� �ִ���? ������? ����.  */

BEGIN(Engine)
class ENGINE_DLL CCell :
    public CBase
{
public:
    enum TYPE { POINT_A, POINT_B, POINT_C, POINT_END };
    enum LINE_TYPE { LINE_AB, LINE_BC, LINE_CA, LINE_END };

private:
    CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CCell() = default;

public:
    HRESULT Initialize(const _float3* pPoints, _int iIndex);

#ifdef _DEBUG
public:
    HRESULT Render();
#endif // _DEBUG

private:
    ID3D11Device* m_pDevice = { nullptr };
    ID3D11DeviceContext* m_pContext = { nullptr };

private:
    _float3 m_vPoints[POINT_END] = {}; // �ﰢ���� ��ǥ�� ����
    _int m_iNeighborIndices[LINE_END] = {};

    _int m_iIndex = { -1 };

#ifdef _DEBUG
private:
    class CVIBuffer_Cell* m_pVIBufferCom = { nullptr };
#endif // _DEBUG

public:
    static CCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _int iIndex);
    virtual void Free() override;
};

END