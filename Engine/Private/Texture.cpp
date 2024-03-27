#include "Texture.h"

#include "Shader.h"

CTexture::CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent{ pDevice, pContext }
{
}

CTexture::CTexture(const CTexture& rhs)
	: CComponent{ rhs },
	m_iNumTextures{ rhs.m_iNumTextures },
	m_pTextures{ rhs.m_pTextures }
{
	for (auto& iter : m_pTextures)
		Safe_AddRef(iter);
}

HRESULT CTexture::Initialize_Prototype(const wstring& strTextureFilePath, _uint iNumTextures)
{
	m_pTextures.reserve(iNumTextures);

	m_iNumTextures = iNumTextures;
	_tchar szTextureFile[MAX_PATH] = L"";
	_tchar szExt[MAX_PATH] = L"";

	for (size_t i = 0; i < iNumTextures; ++i)
	{
		wsprintf(szTextureFile, strTextureFilePath.c_str(), iNumTextures);
		_wsplitpath_s(szTextureFile, nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH); // 파일 경로에서 확장자 저장

		ID3D11ShaderResourceView* pSRV = { nullptr };
		HRESULT hr = {};

		if (!lstrcmp(szExt, L".dds"))
		{
			hr = CreateDDSTextureFromFile(m_pDevice, szTextureFile, nullptr, &pSRV);
		}
		else if (!lstrcmp(szExt, L".tgr"))
		{
			MSG_BOX("Error : TextureFilePath Ext : .tgr");
			hr = E_FAIL;
		}
		else
			hr = CreateWICTextureFromFile(m_pDevice, szTextureFile, nullptr, &pSRV);

		if (FAILED(hr))
			return E_FAIL;

		m_pTextures.push_back(pSRV);
	}

	return S_OK;
}

HRESULT CTexture::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CTexture::Bind_ShaderResource(CShader* pShader, const _char* pConstantName, _uint iNumTextures)
{
	if (m_iNumTextures < iNumTextures)
		return E_FAIL;

	return pShader->Bind_SRV(pConstantName, m_pTextures[iNumTextures]);
}

CTexture* CTexture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strTextureFilePath, _uint iNumTextures)
{
	CTexture* pInstance = new CTexture(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(strTextureFilePath, iNumTextures)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* CTexture::Clone(void* pArg)
{
	CTexture* pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CTexture::Free()
{
	__super::Free();

	for (auto& iter : m_pTextures)
		Safe_Release(iter);
}
