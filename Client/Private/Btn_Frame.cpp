#include "Btn_Frame.h"

#include "MainApp.h"
#include "GameInstance.h"

CBtn_Frame::CBtn_Frame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUI { pDevice, pContext }
{
}

CBtn_Frame::CBtn_Frame(const CBtn_Frame& rhs)
    : CUI { rhs }
{
}

HRESULT CBtn_Frame::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBtn_Frame::Initialize(void* pArg)
{
    if (nullptr != pArg)
    {
        BTN_FRAME_DESC* pDesc = static_cast<BTN_FRAME_DESC*>(pArg);
        m_strText = pDesc->strText;
        m_ParentMatrix = pDesc->ParentMatrix;
        m_fX = pDesc->vPos.x;
        m_fY = pDesc->vPos.y;
    }

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Add_Components()))
        return E_FAIL;

    m_fSizeX = 16.f;
    m_fSizeY = 16.f;

    m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX, m_fY, 0.f, 1.f));

    _matrix ParentPosMatrix = XMMatrixIdentity();
    ParentPosMatrix.r[3] = XMLoadFloat4x4(&m_ParentMatrix).r[3];

    XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * ParentPosMatrix);

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.0f));

    return S_OK;
}

void CBtn_Frame::Priority_Tick(const _float& fTimeDelta)
{
}

void CBtn_Frame::Tick(const _float& fTimeDelta)
{
}

void CBtn_Frame::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CBtn_Frame::Render()
{
    if (FAILED(Bind_ResourceData()))
        return E_FAIL;

    m_pShaderCom->Begin(6);

    m_pVIBufferCom->Render();

    _vector vPos = XMLoadFloat4x4(&m_WorldMatrix).r[3];
    _float2 vFontPos = _float2(XMVectorGetX(vPos) + g_iWinSizeX * 0.5f - 6.f, -XMVectorGetY(vPos) + g_iWinSizeY * 0.5f - 7.5f);
    m_pGameInstance->Render_Font(L"Font_Nanum_20", m_strText, vFontPos, XMVectorSet(0.f, 0.f, 0.f, 1.f));

    return S_OK;
}

HRESULT CBtn_Frame::Add_Components()
{
    if (FAILED(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(CMainApp::g_iCurrentLevel, L"Prototype_Component_Shader_VtxPosTex_UI", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(CMainApp::g_iCurrentLevel, L"Prototype_Component_Texture_UI_Btn_Frame", L"Com_Texture", reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CBtn_Frame::Bind_ResourceData()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
        return E_FAIL;
}

CBtn_Frame* CBtn_Frame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBtn_Frame* pInstance = new CBtn_Frame(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
        Safe_Release(pInstance);

    return pInstance;
}

CGameObject* CBtn_Frame::Clone(void* pArg)
{
    CBtn_Frame* pInstance = new CBtn_Frame(*this);

    if (FAILED(pInstance->Initialize(pArg)))
        Safe_Release(pInstance);

    return pInstance;
}

void CBtn_Frame::Free()
{
    __super::Free();
}
