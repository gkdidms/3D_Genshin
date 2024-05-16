#include "BlackFox.h"

CBlackFox::CBlackFox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSkillObj{ pDevice, pContext }
{
}

CBlackFox::CBlackFox(const CBlackFox& rhs)
	: CSkillObj{ rhs }
{
}

HRESULT CBlackFox::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBlackFox::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	_uint iAnim = { 0 };
	//if (*m_pState == PLAYER_ATTACK_1)
	//	iAnim = 0;
	//else if (*m_pState == PLAYER_ATTACK_2)
	//	iAnim = 1;
	//else if (*m_pState == PLAYER_ATTACK_3)
	//	iAnim = 2;

	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ iAnim, false, false, false });

	m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(m_pParentMatrix));

	return S_OK;
}

void CBlackFox::Priority_Tick(const _float& fTimeDelta)
{
}

void CBlackFox::Tick(const _float& fTimeDelta)
{
	if (m_pModelCom->Get_Animation_Finished())
	{
		m_isDead = true;
		return;
	}

	_float4x4 pMoveMatrix;
	m_pModelCom->Play_Animation(fTimeDelta, &pMoveMatrix);
}

void CBlackFox::Late_Tick(const _float& fTimeDelta)
{

	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CBlackFox::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBlackFox::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_BlackFox", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;


	return S_OK;
}

HRESULT CBlackFox::Bind_ResourceData()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CBlackFox::Change_Animation(const _float& fTimeDelta)
{
	
}

CBlackFox* CBlackFox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBlackFox* pInstance = new CBlackFox(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CBlackFox::Clone(void* pArg)
{
	CBlackFox* pInstance = new CBlackFox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CBlackFox::Free()
{
	__super::Free();
}
