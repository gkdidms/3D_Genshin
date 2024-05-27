#include "Effect.h"

CEffect::CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CGameObject{ pDevice, pContext }
{
}

CEffect::CEffect(const CEffect& rhs)
    : CGameObject{ rhs }
{
}

HRESULT CEffect::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CEffect::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CEffect::Priority_Tick(const _float& fTimeDelta)
{
}

void CEffect::Tick(const _float& fTimeDelta)
{
}

void CEffect::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CEffect::Render()
{
    return S_OK;
}

void CEffect::Free()
{
}
