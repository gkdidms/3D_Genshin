#include "Tool_Effect.h"

#include "GameInstance.h"

CTool_Effect::CTool_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CBlendObject{ pDevice, pContext  }
{
}

CTool_Effect::CTool_Effect(const CTool_Effect& rhs)
    : CBlendObject{ rhs }
{
}

HRESULT CTool_Effect::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTool_Effect::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (nullptr == pArg)
        return E_FAIL;

    TOOL_EFFECT_DESC* pDesc = static_cast<TOOL_EFFECT_DESC*>(pArg);
    
    //텍스쳐 경로 저장
    string strTextureFilePath(pDesc->strTextureFilePath);
    m_strTextureFilePath.assign(strTextureFilePath.begin(), strTextureFilePath.end());

    //마스크 텍스쳐를 쓰는지 안쓰는지
    m_isMask = pDesc->isMask;
    if (m_isMask)
        m_strMaskPath.assign(pDesc->strMaskFilePath.begin(), pDesc->strMaskFilePath.end());

    m_isNoise = pDesc->isNoise;
    if (m_isNoise)
        m_strNoisePath.assign(pDesc->strNoiseFilePath.begin(), pDesc->strNoiseFilePath.end());

    // 이펙트 타입
    m_iEffectType = pDesc->iEffectType;

    // 텍스쳐 갯수
    m_iNumTexture = pDesc->iTextureNum == 0 ? 1 : pDesc->iTextureNum;

    if (m_iNumTexture > 1)
    {
        //포맷 지정자 추가하기
        string strFilePath = pDesc->strTextureFilePath;
        _int iIndex = 6;
        strFilePath.replace(strFilePath.length() - iIndex, 2, "%02d");

        m_strTextureFilePath.assign(strFilePath.begin(), strFilePath.end());
    }

    m_iRendererType = pDesc->iRendererType;
    m_fStartTime = pDesc->fStartTime;
    m_fDurationTime = pDesc->fDurationTime;
    m_isFrameLoop = pDesc->isFrameLoop;
    m_vColor = pDesc->vColor;
    m_pTransformCom->Set_WorldMatrix(pDesc->WorldMatrix);



    return S_OK;
}

void CTool_Effect::Priority_Tick(const _float& fTimeDelta)
{
}

void CTool_Effect::Tick(const _float& fTimeDelta)
{
}

void CTool_Effect::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CTool_Effect::Render()
{
    return S_OK;
}

HRESULT CTool_Effect::Add_Components()
{
    m_pTextureCom = CTexture::Create(m_pDevice, m_pContext, m_strTextureFilePath, m_iNumTexture);
    if (nullptr == m_pTextureCom)
        return E_FAIL;

    if (m_strMaskPath.length() > 0 && m_isMask == true)
    {
        m_pMaskTextureCom = CTexture::Create(m_pDevice, m_pContext, m_strMaskPath, 1);
        if (nullptr == m_pMaskTextureCom)
            return E_FAIL;
    }

    if (m_strNoisePath.length() > 0 && m_isNoise == true)
    {
        
        m_pNoiseTexturCom = CTexture::Create(m_pDevice, m_pContext, m_strNoisePath, 1);
        if (nullptr == m_pNoiseTexturCom)
            return E_FAIL;
    }

    return S_OK;
}

HRESULT CTool_Effect::Bind_ResourceData()
{
    if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", (_uint)m_fFrame)))
        return E_FAIL;

    if (m_isMask)
    {
        if (FAILED(m_pMaskTextureCom->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture", 0)))
            return E_FAIL;
    }

    if (m_isNoise)
    {
        if (FAILED(m_pNoiseTexturCom->Bind_ShaderResource(m_pShaderCom, "g_NoiseTexture", 0)))
            return E_FAIL;
    }

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vColor", &m_vColor, sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fTimeDelta", &m_fCurrentTime, sizeof(_float))))
        return E_FAIL;

    return S_OK;
}

void CTool_Effect::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pMaskTextureCom);
    Safe_Release(m_pNoiseTexturCom);
}
