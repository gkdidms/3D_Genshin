#include "Avatar_Side_Select.h"

#include "MainApp.h"
#include "GameInstance.h"

#include "Player.h"

#include "Avatar_Face.h"
#include "Btn_Frame.h"

CAvatar_Side_Select::CAvatar_Side_Select(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI { pDevice, pContext}
{
}

CAvatar_Side_Select::CAvatar_Side_Select(const CAvatar_Side_Select& rhs)
	: CUI { rhs }
{
}

HRESULT CAvatar_Side_Select::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAvatar_Side_Select::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	AVATAR_SIDE_SELECT_DESC* pDesc = static_cast<AVATAR_SIDE_SELECT_DESC*>(pArg);
	m_iPlayerble = pDesc->iPlayerble;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_fSizeX = 256.f;
	m_fSizeY = 64.f * 0.7f;

	m_fX = g_iWinSizeX >> 1;
	m_fY = g_iWinSizeY >> 1;

	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(g_iWinSizeX - m_fX - 100.f, pDesc->vPos.y, 0.f, 1.f));
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.0f));

	if (FAILED(Ready_Btn()))
		return E_FAIL;

	return S_OK;
}

void CAvatar_Side_Select::Priority_Tick(const _float& fTimeDelta)
{
}

void CAvatar_Side_Select::Tick(const _float& fTimeDelta)
{
}

void CAvatar_Side_Select::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CAvatar_Side_Select::Render()
{
	m_pNumBtn->Render();

	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	m_pShaderCom->Begin(7);
	m_pVIBufferCom->Render();

	m_pFace->Render();

	wstring strText = L"테스트 중";
	if (m_iPlayerble == CPlayer::PLAYER_TIGHNARI)
		strText = TEXT("타이나리");
	else if (m_iPlayerble == CPlayer::PLAYER_NILOU)
		strText = TEXT("닐루");
	else if (m_iPlayerble == CPlayer::PLAYER_WANDERER)
		strText = TEXT("방랑자");
	else if (m_iPlayerble == CPlayer::PLAYER_FEIYAN)
		strText = TEXT("연비");

	_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float2 vFontPos = _float2(XMVectorGetX(vPos) + g_iWinSizeX * 0.5f - 80.f, -XMVectorGetY(vPos) + g_iWinSizeY * 0.5f - 10.f);
	m_pGameInstance->Render_Font(L"Font_Nanum_28", strText, vFontPos, XMVectorSet(1.f, 1.f, 1.f, 1.f));

	return S_OK;
}

HRESULT CAvatar_Side_Select::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(CMainApp::g_iCurrentLevel, L"Prototype_Component_Shader_VtxPosTex_UI", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(CMainApp::g_iCurrentLevel, L"Prototype_Component_Texture_UI_Avatar_Side_Select_Frame", L"Com_Texture", reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CAvatar_Side_Select::Bind_ResourceData()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;
}

HRESULT CAvatar_Side_Select::Ready_Btn()
{
	CAvatar_Face::AVATAR_FACE_DESC FaceDesc{};
	FaceDesc.ParentMatrix = *m_pTransformCom->Get_WorldFloat4x4();
	FaceDesc.iPlayerble = m_iPlayerble;
	FaceDesc.vSize = _float2(128.f * 0.55f, 128.f * 0.55f);
	FaceDesc.vPos = _float2(30.f, 15.f);

	m_pFace = m_pGameInstance->Clone_Object(L"Prototype_GameObject_UI_Avatar_Face", &FaceDesc);
	if (nullptr == m_pFace)
		return E_FAIL;

	CBtn_Frame::BTN_FRAME_DESC BtnDesc{};
	BtnDesc.vPos = _float2{ 70.f, 0.f };
	BtnDesc.ParentMatrix = *m_pTransformCom->Get_WorldFloat4x4();
	BtnDesc.strText = to_wstring(m_iPlayerble + 1);

	m_pNumBtn = m_pGameInstance->Clone_Object(L"Prototype_GameObject_UI_Btn_Frame", &BtnDesc);
	if (nullptr == m_pNumBtn)
		return E_FAIL;

	return S_OK;
}

CAvatar_Side_Select* CAvatar_Side_Select::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CAvatar_Side_Select* pInstance = new CAvatar_Side_Select(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CAvatar_Side_Select::Clone(void* pArg)
{
	CAvatar_Side_Select* pInstance = new CAvatar_Side_Select(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CAvatar_Side_Select::Free()
{
	__super::Free();

	Safe_Release(m_pFace);
	Safe_Release(m_pNumBtn);
}
