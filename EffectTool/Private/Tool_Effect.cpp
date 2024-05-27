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
    if (nullptr != pArg)
    {
        TOOL_EFFECT_DESC* pDesc = static_cast<TOOL_EFFECT_DESC*>(pArg);
        m_iEffectType = pDesc->iEffectType;
        m_strTextureFileName = pDesc->strTextureFileName;
    }

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

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
    return S_OK;
}

HRESULT CTool_Effect::Bind_ResourceData()
{
    return S_OK;
}

void CTool_Effect::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
}
