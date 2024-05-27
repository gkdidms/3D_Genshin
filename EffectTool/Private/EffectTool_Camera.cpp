#include "EffectTool_Camera.h"

#include "GameInstance.h"

CEffectTool_Camera::CEffectTool_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CCamera{ pDevice, pContext }
{
}

CEffectTool_Camera::CEffectTool_Camera(const CEffectTool_Camera& rhs)
    : CCamera { rhs }
{
}

HRESULT CEffectTool_Camera::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CEffectTool_Camera::Initialize(void* pArg)
{
    if (nullptr == pArg)
        return E_FAIL;

    EFFECT_TOOL_CAMERA_DESC* pDesc = static_cast<EFFECT_TOOL_CAMERA_DESC*>(pArg);
    m_fSensor = pDesc->fSensor;

    XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CEffectTool_Camera::Priority_Tick(const _float& fTimeDelta)
{

}

void CEffectTool_Camera::Tick(const _float& fTimeDelta)
{
    if (m_pGameInstance->Get_DIKeyState(DIK_W) & 0x80)
        m_pTransformCom->Go_Straight(fTimeDelta);
    if (m_pGameInstance->Get_DIKeyState(DIK_S) & 0x80)
        m_pTransformCom->Go_Backwork(fTimeDelta);
    if (m_pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
        m_pTransformCom->Go_Left(fTimeDelta);
    if (m_pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
        m_pTransformCom->Go_Right(fTimeDelta);

    if (m_pGameInstance->GetKeyState(DIK_TAB) == CInput_Device::TAP)
        m_isCameraMove = !m_isCameraMove;

    if (m_isCameraMove)
    {
        _float MouseMove = { 0.f };
        if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_X))
            m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * m_fSensor * MouseMove);

        if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_Y))
            m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta * m_fSensor * MouseMove);
    }

    __super::Tick(fTimeDelta);
}

void CEffectTool_Camera::Late_Tick(const _float& fTimeDelta)
{
    XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix());
}

HRESULT CEffectTool_Camera::Render()
{
    return S_OK;
}

CEffectTool_Camera* CEffectTool_Camera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CEffectTool_Camera* pInstance = new CEffectTool_Camera(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
        Safe_Release(pInstance);

    return pInstance;
}

CGameObject* CEffectTool_Camera::Clone(void* pArg)
{
    CEffectTool_Camera* pInstance = new CEffectTool_Camera(*this);

    if (FAILED(pInstance->Initialize(pArg)))
        Safe_Release(pInstance);

    return pInstance;
}

void CEffectTool_Camera::Free()
{
    __super::Free();
}
