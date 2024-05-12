#include "Brute.h"

CBrute::CBrute(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CMonster { pDevice, pContext }
{
}

CBrute::CBrute(const CBrute& rhs)
    : CMonster { rhs }
{
}

HRESULT CBrute::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBrute::Initialize(void* pArg)
{
    return S_OK;
}

void CBrute::Priority_Tick(const _float& fTimeDelta)
{
}

void CBrute::Tick(const _float& fTimeDelta)
{
}

void CBrute::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CBrute::Render()
{
    return S_OK;
}

HRESULT CBrute::Add_Components()
{
    return S_OK;
}

HRESULT CBrute::Bind_ResourceData()
{
    return S_OK;
}

void CBrute::Change_Animation(const _float& fTimeDelta)
{
}

void CBrute::Free()
{
}
