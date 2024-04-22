#pragma once
#include "Base.h"

/* 1. 네비게이션을 구성하는 하나의 삼가형. */
/* 2. 삼각형을 구성하는 세점의 정보를 보관한다. */
/* 3. 세 변을 구성하여 객체의 결과위치가 안에 있는지? 없는지? 조사.  */

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
    _float3 m_vPoints[POINT_END] = {}; // 삼각형의 좌표를 저장
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