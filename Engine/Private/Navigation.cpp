#include "Navigation.h"

#include "Cell.h"
#include "Shader.h"

CNavigation::CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CComponent{ pDevice, pContext }
{
}

CNavigation::CNavigation(const CNavigation& rhs)
    : CComponent{ rhs }
{
}

void CNavigation::Set_Points(const _float3* vPoints)
{
    CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoints, m_iIndexCount);
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

    return S_OK;
}

HRESULT CNavigation::Initialize(void* pArg)
{
    return S_OK;
}

void CNavigation::Tick()
{
}

#ifdef _DEBUG
HRESULT CNavigation::Render()
{
    m_pShaderCom->Begin(0);

    for (auto& pCell : m_Cells)
        pCell->Render();

    return S_OK;
}
#endif // _DEBUG



HRESULT CNavigation::Load_File(const wstring strFilePath)
{

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

void CNavigation::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);

    for (auto& pCell : m_Cells)
        Safe_Release(pCell);
    m_Cells.clear();
}

