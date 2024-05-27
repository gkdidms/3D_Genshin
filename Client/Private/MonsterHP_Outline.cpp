#include "MonsterHp_Outline.h"

#include "GameInstance.h"

#include "MonsterHP.h"

CMonsterHP_Outline::CMonsterHP_Outline(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI{ pDevice, pContext}
{
}

CMonsterHP_Outline::CMonsterHP_Outline(const CMonsterHP_Outline& rhs)
	: CUI { rhs }
{
}

HRESULT CMonsterHP_Outline::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonsterHP_Outline::Initialize(void* pArg) // 빌보드 적용하기 
{
	if (nullptr != pArg)
	{
		MONSTER_HP_OUTLINE_DESC* pDesc = static_cast<MONSTER_HP_OUTLINE_DESC*>(pArg);
		m_pParentMatrix = pDesc->pParentMatrix;
		m_pPlayerHP = pDesc->pMonsterHP;
		m_fMonsterMaxHP = pDesc->fMonsterMaxHP;
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_fSizeX = 128.f * 0.01f;
	m_fSizeY = 10.f * 0.02f;
	m_fX = 0.f;
	m_fY = 2.f;

	m_pTransformCom->Set_Scale(m_fSizeX, m_fSizeY, 1.f);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(m_fX, m_fY, 0.f, 1.f));
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));

	return S_OK;
}

void CMonsterHP_Outline::Priority_Tick(const _float& fTimeDelta)
{
}

void CMonsterHP_Outline::Tick(const _float& fTimeDelta)
{	
	m_pTransformCom->LookAt(m_pGameInstance->Get_CamLook());
}

void CMonsterHP_Outline::Late_Tick(const _float& fTimeDelta)
{
	_matrix ParentPosMatrix = XMMatrixIdentity();
	ParentPosMatrix.r[3] = XMLoadFloat4x4(m_pParentMatrix).r[3];
	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * ParentPosMatrix);

	m_pGameInstance->Add_Renderer(CRenderer::RENDER_UI, this);

	m_pMonsterHPBar->Late_Tick(fTimeDelta);
}

HRESULT CMonsterHP_Outline::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(m_pMonsterHPBar->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonsterHP_Outline::Add_Components()
{

	if (FAILED(__super::Add_Component(LEVEL_STATIC, L"Prototype_Component_VIBuffer_Rect", L"Com_VIBuffer", reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxPosTex_UI", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Texture_UI_Monster_HP_Outline", L"Com_Texture", reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	CMonsterHP::MONSTER_HP_DESC tMonsterDesc{};
	tMonsterDesc.pParentMatrix = &m_WorldMatrix;
	tMonsterDesc.pMonsterHP = m_pPlayerHP;
	tMonsterDesc.fMonsterMaxHP = m_fMonsterMaxHP;

	m_pMonsterHPBar = dynamic_cast<CMonsterHP*>(m_pGameInstance->Clone_Object(L"Prototype_GameObject_UI_MonsterHP", &tMonsterDesc));
	if (nullptr == m_pMonsterHPBar)
		return E_FAIL;

	return S_OK;
}

HRESULT CMonsterHP_Outline::Bind_ResourceData()
{
	//if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
	//	return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

CMonsterHP_Outline* CMonsterHP_Outline::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonsterHP_Outline* pInstance = new CMonsterHP_Outline(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CMonsterHP_Outline::Clone(void* pArg)
{
	CMonsterHP_Outline* pInstane = new CMonsterHP_Outline(*this);

	if (FAILED(pInstane->Initialize(pArg)))
		Safe_Release(pInstane);

	return pInstane;
}

void CMonsterHP_Outline::Free()
{
	__super::Free();

	Safe_Release(m_pMonsterHPBar);
}
