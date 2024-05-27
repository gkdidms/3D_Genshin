#include "Avatar_Face.h"

#include "GameInstance.h"

#include "Player.h"

CAvatar_Face::CAvatar_Face(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUI{ pDevice, pContext }
{
}

CAvatar_Face::CAvatar_Face(const CAvatar_Face& rhs)
    : CUI{ rhs }
{
}

HRESULT CAvatar_Face::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CAvatar_Face::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	AVATAR_FACE_DESC* pDesc = static_cast<AVATAR_FACE_DESC*>(pArg);
	m_iPlayerble = pDesc->iPlayerble;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	//m_fSizeX = 128.f * 0.6f;
	//m_fSizeY = 128.f * 0.6f;
	m_fSizeX = pDesc->vSize.x;
	m_fSizeY = pDesc->vSize.y;

	m_fX = pDesc->vPos.x;
	m_fY = pDesc->vPos.y;

	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX, m_fY, 0.f, 1.f));

	_matrix ParentPosMatrix = XMMatrixIdentity();
	ParentPosMatrix.r[3] = XMLoadFloat4x4(&pDesc->ParentMatrix).r[3];

	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * ParentPosMatrix);

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.0f));

    return S_OK;
}

void CAvatar_Face::Priority_Tick(const _float& fTimeDelta)
{
}

void CAvatar_Face::Tick(const _float& fTimeDelta)
{
}

void CAvatar_Face::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CAvatar_Face::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	m_pShaderCom->Begin(4);
	m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CAvatar_Face::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxPosTex_UI", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	wstring strPrototypeTag = { L"" };
	if (m_iPlayerble == CPlayer::PLAYER_TIGHNARI)
		strPrototypeTag = TEXT("Prototype_Component_Texture_UI_Avatar_Side_Tighnari");
	else if (m_iPlayerble == CPlayer::PLAYER_NILOU)
		strPrototypeTag = TEXT("Prototype_Component_Texture_UI_Avatar_Side_Nilou");
	else if (m_iPlayerble == CPlayer::PLAYER_WANDERER)
		strPrototypeTag = TEXT("Prototype_Component_Texture_UI_Avatar_Side_Wanderer");
	else if (m_iPlayerble == CPlayer::PLAYER_FEIYAN)
		strPrototypeTag = TEXT("Prototype_Component_Texture_UI_Avatar_Side_Feiyan");

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, strPrototypeTag, L"Com_Texture", reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;


	return S_OK;
}

HRESULT CAvatar_Face::Bind_ResourceData()
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

CAvatar_Face* CAvatar_Face::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CAvatar_Face* pInstance = new CAvatar_Face(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
        Safe_Release(pInstance);

    return pInstance;
}

CGameObject* CAvatar_Face::Clone(void* pArg)
{
    CAvatar_Face* pInstance = new CAvatar_Face(*this);

    if (FAILED(pInstance->Initialize(pArg)))
        Safe_Release(pInstance);

    return pInstance;
}

void CAvatar_Face::Free()
{
    __super::Free();
}
