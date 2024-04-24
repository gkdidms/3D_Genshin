#include "SkillObj_Gohei.h"


CSkillObj_Gohei::CSkillObj_Gohei(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CSkillObj{ pDevice, pContext }
{
}

CSkillObj_Gohei::CSkillObj_Gohei(const CSkillObj_Gohei& rhs)
	: CSkillObj{ rhs }
{
}

HRESULT CSkillObj_Gohei::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkillObj_Gohei::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;


	return S_OK;
}

void CSkillObj_Gohei::Priority_Tick(const _float& fTimeDelta)
{
}

void CSkillObj_Gohei::Tick(const _float& fTimeDelta)
{
	Change_Animation(fTimeDelta);

	_float4x4 pMoveMatrix;
	m_pModelCom->Play_Animation(fTimeDelta, &pMoveMatrix);
}

void CSkillObj_Gohei::Late_Tick(const _float& fTimeDelta)
{
	_matrix		SocketMatrix = XMMatrixIdentity();

	if (*m_pState == PLAYER_ATTACK_1
		|| *m_pState == PLAYER_ATTACK_2
		|| *m_pState == PLAYER_ATTACK_3
		|| *m_pState == PLAYER_ATTACK_4)
	{
		SocketMatrix = XMLoadFloat4x4(m_pSocketMatrix);

		SocketMatrix.r[0] = XMVector3Normalize(SocketMatrix.r[0]);
		SocketMatrix.r[1] = XMVector3Normalize(SocketMatrix.r[1]);
		SocketMatrix.r[2] = XMVector3Normalize(SocketMatrix.r[2]);

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-109.f, 0.f, 237.f, 1.f));
		XMStoreFloat4x4(&m_pWorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketMatrix * XMLoadFloat4x4(m_pParentMatrix));

		m_pGameInstance->Add_Renderer(CRenderer::RENDER_NONBLENDER, this);
	}
}

HRESULT CSkillObj_Gohei::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkillObj_Gohei::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxAnimMesh", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Gohei", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkillObj_Gohei::Bind_ResourceData()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

void CSkillObj_Gohei::Change_Animation(const _float& fTimeDelta)
{

	switch (*m_pState)
	{
	case PLAYER_ATTACK_1:
		m_iAnim = 0;
		m_IsLoop = false;
		break;
	case PLAYER_ATTACK_2:
		m_iAnim = 1;
		m_IsLoop = false;
		break;
	case PLAYER_ATTACK_3:
		m_iAnim = 2;
		m_IsLoop = false;
		break;
	case PLAYER_ATTACK_4:
		m_iAnim = 3;
		m_IsLoop = false;
		break;
	default:
		break;
	}

	m_pModelCom->Set_Animation(CModel::ANIM_DESC{ m_iAnim, m_IsLoop, m_IsLinear, m_IsLinearSpeed });

}

CSkillObj_Gohei* CSkillObj_Gohei::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSkillObj_Gohei* pInstance = new CSkillObj_Gohei(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CSkillObj_Gohei::Clone(void* pArg)
{
	CSkillObj_Gohei* pInstance = new CSkillObj_Gohei(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CSkillObj_Gohei::Free()
{
	__super::Free();
}

