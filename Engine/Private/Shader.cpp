#include "Shader.h"

CShader::CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent{ pDevice, pContext }
{
}

CShader::CShader(const CShader& rhs)
	: CComponent{ rhs },
	m_pEffect{rhs.m_pEffect},
	m_iNumPasses{rhs.m_iNumPasses},
	m_InputLayouts{rhs.m_InputLayouts}
{
	Safe_AddRef(m_pEffect);

	for (size_t i = 0; i < m_iNumPasses; ++i)
		Safe_AddRef(m_InputLayouts[i]);
}

HRESULT CShader::Initialize_Prototype(const wstring& strShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElement, _uint iNumElement)
{
	_uint iHlslFlag = { 0 };

#ifdef _DEBUG
	iHlslFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iHlslFlag = D3DCOMPILE_OPTIMIZATION_LEVEL0;
#endif // DEBUG

	if (FAILED(D3DX11CompileEffectFromFile(strShaderFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iHlslFlag, 0, m_pDevice, &m_pEffect, nullptr)))
		return E_FAIL;

	ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (nullptr == pTechnique)
		return E_FAIL;

	D3DX11_TECHNIQUE_DESC TechniqueDesc = {};

	pTechnique->GetDesc(&TechniqueDesc);

	m_iNumPasses = TechniqueDesc.Passes;

	for (size_t i = 0; i < m_iNumPasses; ++i)
	{
		ID3DX11EffectPass* pPass = pTechnique->GetPassByIndex(i);

		D3DX11_PASS_DESC PassDesc{};

		pPass->GetDesc(&PassDesc);

		ID3D11InputLayout* pInputLayout = { nullptr };

		if (FAILED(m_pDevice->CreateInputLayout(pElement, iNumElement, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pInputLayout)))
			return E_FAIL;

		m_InputLayouts.push_back(pInputLayout);
	}

	return S_OK;
}

HRESULT CShader::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CShader::Begin(_uint iPassIndex)
{
	if (nullptr == m_InputLayouts[iPassIndex])
		return E_FAIL;

	m_pContext->IASetInputLayout(m_InputLayouts[iPassIndex]);

	ID3DX11EffectPass* pPass = m_pEffect->GetTechniqueByIndex(0)->GetPassByIndex(iPassIndex);
	if (nullptr == pPass)
		return E_FAIL;

	pPass->Apply(0, m_pContext);

	return S_OK;
}

HRESULT CShader::Bind_Matrix(const _char* pConstantName, const _float4x4* pMatrix)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
	if (nullptr == pMatrixVariable)
		return E_FAIL;

	return pMatrixVariable->SetMatrix((_float*)pMatrix);
}

HRESULT CShader::Bind_Matrices(const _char* pConstantName, const _float4x4* pMatrix, _uint iNumMatrices)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
	if (nullptr == pMatrixVariable)
		return E_FAIL;

	return pMatrixVariable->SetMatrixArray((_float*)pMatrix, 0, iNumMatrices);
}

HRESULT CShader::Bind_SRV(const _char* pConstantName, ID3D11ShaderResourceView* pSRV)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable* pShaderResourceVariable = pVariable->AsShaderResource();
	if (nullptr == pShaderResourceVariable)
		return E_FAIL;

	return pShaderResourceVariable->SetResource(pSRV);
}

HRESULT CShader::Bind_RawValue(const _char* pConstantName, const void* pValue, _uint iLength)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	return pVariable->SetRawValue(pValue, 0, iLength);
}

CShader* CShader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElement, _uint iNumElements)
{
	CShader* pInstance = new CShader(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(strShaderFilePath, pElement, iNumElements)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* CShader::Clone(void* pArg)
{
	CShader* pInstance = new CShader(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CShader::Free()
{
	__super::Free();

	Safe_Release(m_pEffect);

	for (size_t i = 0; i < m_iNumPasses; ++i)
		Safe_Release(m_InputLayouts[i]);
}
