#include "SkillBtn_E.h"

#include "MainApp.h"
#include "GameInstance.h"

#include "SkillBtn_Icon.h"
#include "Player.h"
#include "StateManager.h"
#include "Btn_Frame.h"

CSkillBtn_E::CSkillBtn_E(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI{ pDevice, pContext }
{
}

CSkillBtn_E::CSkillBtn_E(const CSkillBtn_E& rhs)
	: CUI{ rhs },
	m_pStateManager{ CStateManager::GetInstance()}
{
	Safe_AddRef(m_pStateManager);
}

HRESULT CSkillBtn_E::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkillBtn_E::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_fSizeX = 78.f;
	m_fSizeY = 78.f;

	m_fX = g_iWinSizeX >> 1;
	m_fY = g_iWinSizeY >> 1;

	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-m_fX + g_iWinSizeX - 220.f, m_fY - g_iWinSizeY + m_fSizeY * 0.5f + 30.f, 0.f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.0f));

	if (FAILED(Ready_Icon()))
		return E_FAIL;

	return S_OK;
}

void CSkillBtn_E::Priority_Tick(const _float& fTimeDelta)
{
}

void CSkillBtn_E::Tick(const _float& fTimeDelta)
{
}

void CSkillBtn_E::Late_Tick(const _float& fTimeDelta)
{
	m_iCurrentPlayerble = m_pStateManager->Get_Playerble();

	m_pGameInstance->Add_Renderer(CRenderer::RENDER_UI, this);
}

HRESULT CSkillBtn_E::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	m_pShaderCom->Begin(3);
	m_pVIBufferCom->Render();

	m_pBtnIcon[m_iCurrentPlayerble]->Render();

	m_pBtnFrame->Render();
	return S_OK;
}

HRESULT CSkillBtn_E::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(CMainApp::g_iCurrentLevel, L"Prototype_Component_Shader_VtxPosTex_UI", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(CMainApp::g_iCurrentLevel, L"Prototype_Component_Texture_UI_Btn_Frame_26", L"Com_Texture", reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkillBtn_E::Bind_ResourceData()
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

HRESULT CSkillBtn_E::Ready_Icon()
{
	//플레이어 별 아이콘 가지고 있음. (플레이어가 변경되면 해당 캐릭터의 스킬 이미지로 변경하기)
	CSkillBtn_Icon::SKILLBTN_ICON_DESC Desc{};
	Desc.ParentMatrix = *m_pTransformCom->Get_WorldFloat4x4();
	Desc.strComponentPrototypeTag = L"Prototype_Component_Texture_UI_SkillBtn_Icon_E_Tighnari";
	Desc.fSizeX = 64.f;
	Desc.fSizeY = 64.f;
	m_pBtnIcon[CPlayer::PLAYER_TIGHNARI] = dynamic_cast<CSkillBtn_Icon*>(m_pGameInstance->Clone_Object(L"Prototype_GameObject_UI_SkillBtn_Icon", &Desc));

	Desc.strComponentPrototypeTag = L"Prototype_Component_Texture_UI_SkillBtn_Icon_E_Nilou";
	m_pBtnIcon[CPlayer::PLAYER_NILOU] = dynamic_cast<CSkillBtn_Icon*>(m_pGameInstance->Clone_Object(L"Prototype_GameObject_UI_SkillBtn_Icon", &Desc));

	Desc.strComponentPrototypeTag = L"Prototype_Component_Texture_UI_SkillBtn_Icon_E_Wanderer";
	m_pBtnIcon[CPlayer::PLAYER_WANDERER] = dynamic_cast<CSkillBtn_Icon*>(m_pGameInstance->Clone_Object(L"Prototype_GameObject_UI_SkillBtn_Icon", &Desc));

	Desc.strComponentPrototypeTag = L"Prototype_Component_Texture_UI_SkillBtn_Icon_E_Feiyan";
	m_pBtnIcon[CPlayer::PLAYER_FEIYAN] = dynamic_cast<CSkillBtn_Icon*>(m_pGameInstance->Clone_Object(L"Prototype_GameObject_UI_SkillBtn_Icon", &Desc));


	CBtn_Frame::BTN_FRAME_DESC BtnDesc{};
	BtnDesc.vPos = _float2{ 0.f, -38.f };
	BtnDesc.ParentMatrix = *m_pTransformCom->Get_WorldFloat4x4();
	BtnDesc.strText = L"E";

	m_pBtnFrame = m_pGameInstance->Clone_Object(L"Prototype_GameObject_UI_Btn_Frame", &BtnDesc);
	if (nullptr == m_pBtnFrame)
		return E_FAIL;

	return S_OK;
}

CSkillBtn_E* CSkillBtn_E::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSkillBtn_E* pInstance = new CSkillBtn_E(pDevice, pContext);
	
	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CSkillBtn_E::Clone(void* pArg)
{
	CSkillBtn_E* pInstance = new CSkillBtn_E(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CSkillBtn_E::Free()
{
	__super::Free();

	for (auto& pIcon : m_pBtnIcon)
		Safe_Release(pIcon);

	Safe_Release(m_pBtnFrame);
	Safe_Release(m_pStateManager);
}
