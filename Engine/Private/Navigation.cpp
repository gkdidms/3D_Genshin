#include "Navigation.h"

#include "Cell.h"
#include "Shader.h"
#include "GameInstance.h"

CNavigation::CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CComponent{ pDevice, pContext }
{
}

CNavigation::CNavigation(const CNavigation& rhs)
    : CComponent{ rhs },
    m_pShaderCom{ rhs.m_pShaderCom },
    m_Cells{ rhs.m_Cells }
{
    Safe_AddRef(m_pShaderCom);

    for (auto& pCell : m_Cells)
        Safe_AddRef(pCell);
}

void CNavigation::Set_Points(const _float3* vPoints, _int OptionType)
{
    CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoints, m_iIndexCount++, CCell::OPTION(OptionType));
    if (nullptr == pCell)
        return;

    m_Cells.emplace_back(pCell);
}

HRESULT CNavigation::Initialize_Prototype()
{
    m_pShaderCom = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"), VXTPOS::Elements, VXTPOS::iNumElements);
    if (nullptr == m_pShaderCom)
        return E_FAIL;

    return S_OK;
}

HRESULT CNavigation::Initialize_Prototype(const wstring strFilePath)
{
    m_pShaderCom = CShader::Create(m_pDevice, m_pContext, L"../Bin/ShaderFiles/Shader_Cell.hlsl", VXTPOS::Elements, VXTPOS::iNumElements);
    if (nullptr == m_pShaderCom)
        return E_FAIL;

    // 파일 열어서 가져오기 
    if (FAILED(Load_File(strFilePath)))
        return E_FAIL;

    if (FAILED(SetUp_Neighbors()))
        return E_FAIL;

    return S_OK;
}

HRESULT CNavigation::Initialize(void* pArg)
{
    if (nullptr != pArg)
    {
        NAVIGATION_DESC* pDesc = static_cast<NAVIGATION_DESC*>(pArg);
        m_iCurrentIndex = pDesc->iIndex;
    }

    return S_OK;
}

void CNavigation::Tick()
{
}

_int CNavigation::Find_Index(_fvector vTargetPos, _fvector vTargetRayDir, _fmatrix WorldMatirx)
{
	_ulong	dwVtxIdx[3]{};
	_float	fDist(0.f);
	for (auto& pCell : m_Cells)
	{
		if (Intersects(vTargetPos,
            XMVectorSet(0.f, 1.f, 0.f, 0.f),
			pCell->Get_Point(CCell::POINT_A),
			pCell->Get_Point(CCell::POINT_B),
			pCell->Get_Point(CCell::POINT_C),
			fDist))
		{
			return pCell->Get_Index();
		}
	}

	return -1;
}

#ifdef _DEBUG
HRESULT CNavigation::Render()
{
    XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
    m_WorldMatrix.m[3][1] += 0.1f;
    m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);

    m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW));
    m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ));

    for (auto& pCell : m_Cells)
    {
        _vector vColor{};
        if (pCell->Get_Option() == CCell::OPTION_NONE)
        {
            vColor = XMVectorSet(0.f, 0.f, 1.f, 1.f);
            m_pShaderCom->Bind_RawValue("g_vColor", &vColor, sizeof(_vector));
        }
            
        else if (pCell->Get_Option() == CCell::OPTION_STAIRS)
        {
            vColor = XMVectorSet(1.f, 0.f, 0.f, 1.f);
            m_pShaderCom->Bind_RawValue("g_vColor", &vColor, sizeof(_vector));
        }
        else if (pCell->Get_Option() == CCell::OPTION_FLY)
        {
            vColor = XMVectorSet(0.f, 1.f, 0.f, 1.f);
            m_pShaderCom->Bind_RawValue("g_vColor", &vColor, sizeof(_vector));
        }

        m_pShaderCom->Begin(0);

        pCell->Render();
    }

    return S_OK;
}
#endif // _DEBUG



HRESULT CNavigation::Load_File(const wstring strFilePath)
{
    ifstream ifs(strFilePath, ios::binary | ios::in);

    if (ifs.fail())
        return E_FAIL;

    _uint NumCells = { 0 };
    ifs.read((_char*)&NumCells, sizeof(_uint));

    for (size_t i = 0; i < NumCells; ++i)
    {
        _float3* vPoint = new _float3[3];
        ifs.read((_char*)vPoint, sizeof(_float3) * 3);

        _int iOption = { 0 };
        ifs.read((_char*)&iOption, sizeof(_int));

        CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoint, i, CCell::OPTION(iOption));

        m_Cells.emplace_back(pCell);
        Safe_Delete(vPoint);
    }

    ifs.close();

    return S_OK;
}

HRESULT CNavigation::SetUp_Neighbors()
{
    for (auto& pSourCell : m_Cells)
    {
        for (auto& pDescCell : m_Cells)
        {
            if (pSourCell == pDescCell)
                continue;

            if (pDescCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_A), pSourCell->Get_Point(CCell::POINT_B)))
                pSourCell->SetUp_Neighbor(CCell::LINE_AB, pDescCell);

            if (pDescCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_B), pSourCell->Get_Point(CCell::POINT_C)))
                pSourCell->SetUp_Neighbor(CCell::LINE_BC, pDescCell);

            if (pDescCell->Compare_Points(pSourCell->Get_Point(CCell::POINT_C), pSourCell->Get_Point(CCell::POINT_A)))
                pSourCell->SetUp_Neighbor(CCell::LINE_CA, pDescCell);
        }
    }

    return S_OK;
}

CNavigation* CNavigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring strFilePath)
{
    CNavigation* pInstance = new CNavigation(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(strFilePath)))
        Safe_Release(pInstance);

    return pInstance;
}

CNavigation* CNavigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CNavigation* pInstance = new CNavigation(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
        Safe_Release(pInstance);

    return pInstance;
}

CComponent* CNavigation::Clone(void* pArg)
{
    CNavigation* pInstance = new CNavigation(*this);

    if (FAILED(pInstance->Initialize(pArg)))
        Safe_Release(pInstance);

    return pInstance;
}

_bool CNavigation::isMove(_fvector vMovePos)
{
    _int iNeighborsIndex = { -1 };

    // 셀 안에 있는지?
    if (m_Cells[m_iCurrentIndex]->isIn(vMovePos, &iNeighborsIndex))
        return true;
    else
    {
        if (-1 != iNeighborsIndex)
        {
            // 셀 밖에 있는지?
            while (true)
            {
                if (-1 == iNeighborsIndex)
                    return false;

                if (m_Cells[iNeighborsIndex]->isIn(vMovePos, &iNeighborsIndex))
                {
                    m_iCurrentIndex = iNeighborsIndex;
                    cout << m_iCurrentIndex << endl;
                    return true;
                }
                    
            }
        }
        else
            return false;
    }
}

_float CNavigation::Compute_Height(_fvector vPosition)
{
    return m_Cells[m_iCurrentIndex]->Compute_Height(vPosition);
}


void CNavigation::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);

    for (auto& pCell : m_Cells)
        Safe_Release(pCell);
    m_Cells.clear();
}


