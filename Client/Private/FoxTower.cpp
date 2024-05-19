#include "FoxTower.h"

CFoxTower::CFoxTower(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSkillObj{ pDevice, pContext }
{
}

CFoxTower::CFoxTower(const CFoxTower& rhs)
	: CSkillObj { rhs }
{
}

HRESULT CFoxTower::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CFoxTower::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(m_pParentMatrix));

	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ 0, true, false, false });

	return S_OK;
}

void CFoxTower::Priority_Tick(const _float& fTimeDelta)
{
}

void CFoxTower::Tick(const _float& fTimeDelta)
{
	if (m_fDuration <= m_fCurrentTime)
	{
		m_isDead = true;
		return;
	}
	else m_fCurrentTime += fTimeDelta;

	Change_Animation(fTimeDelta);

	_float4x4 pMoveMatrix;
	m_pModelCom->Play_Animation(fTimeDelta, &pMoveMatrix);
}

void CFoxTower::Late_Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
}

HRESULT CFoxTower::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFoxTower::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_FoxTower_1", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CFoxTower::Bind_ResourceData()
{
	if (FAILED(m_pTransformCom->Bind_ShaderMatrix(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CFoxTower::Change_Animation(const _float& fTimeDelta)
{
}

CFoxTower* CFoxTower::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFoxTower* pInstance = new CFoxTower(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CFoxTower::Clone(void* pArg)
{
	CFoxTower* pInstance = new CFoxTower(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CFoxTower::Free()
{
	__super::Free();
}
