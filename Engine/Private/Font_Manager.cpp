#include "Font_Manager.h"

#include "CustomFont.h"
CFont_Manager::CFont_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : m_pDevice{ pDevice}, m_pContext{ pContext}
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CFont_Manager::Add_Font(const wstring& strFontTag, const wstring& strFontFilePath)
{
    if (nullptr != Find_Font(strFontTag))
        return E_FAIL;

    CCustomFont*  pFont = CCustomFont::Create(m_pDevice, m_pContext, strFontFilePath);
    if (nullptr == pFont)
        return E_FAIL;
    m_Fonts.emplace(strFontTag, pFont);

    return S_OK;
}

HRESULT CFont_Manager::Render_Font(const wstring& strFontTag, const wstring& strText, const _float2& vPosition, _fvector vColor)
{
    CCustomFont* pFont = Find_Font(strFontTag);
    if (nullptr == pFont)
        return E_FAIL;

    return pFont->Render(strText, vPosition, vColor);
}

CCustomFont* CFont_Manager::Find_Font(const wstring& strFontTag)
{
    auto pFont = m_Fonts.find(strFontTag);

    if (pFont == m_Fonts.end())
        return nullptr;

    return pFont->second;
}

CFont_Manager* CFont_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    return new CFont_Manager(pDevice, pContext);
}

void CFont_Manager::Free()
{
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);

    for (auto& Pair : m_Fonts)
        Safe_Release(Pair.second);
    m_Fonts.clear();
}
