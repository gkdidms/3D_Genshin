#include "Skill_Power.h"

#include "GameInstance.h"

CSkill_Power::CSkill_Power(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CSkill_Power::CSkill_Power(const CSkill_Power& rhs)
	: CGameObject{rhs }
{
}

HRESULT CSkill_Power::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSkill_Power::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Add_Components()))
		return E_FAIL;

	if (nullptr == pArg)
		return E_FAIL;

	SKILL_POWER_DESC* pDesc = static_cast<SKILL_POWER_DESC*>(pArg);
	_matrix InitMatrix = XMMatrixIdentity();
	InitMatrix.r[3] = XMVectorSet(0.f, 0.f, 5.f, 1.f);

	m_pTransformCom->Set_WorldMatrix(InitMatrix * pDesc->ParentMatrix);

	m_fHeight = XMVectorGetY(pDesc->ParentMatrix.r[3]);

	return S_OK;
}

void CSkill_Power::Priority_Tick(const _float& fTimeDelta)
{
}

void CSkill_Power::Tick(const _float& fTimeDelta)
{
	m_fAccelTime += fTimeDelta * 0.005f;

	_float fTemp = (m_fPower * m_fAccelTime) - (9.8f * m_fAccelTime * m_fAccelTime * 0.5f);

	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float fY = XMVectorGetY(vPosition);
	vPosition += XMLoadFloat4(&m_vTargetLook) * m_fSpeed * fTimeDelta;
	vPosition = XMVectorSetY(vPosition, fY - fTemp);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

	if (m_fHeight >= XMVectorGetY(vPosition))
	{
		// 충돌했을때 이펙트 출력
	}
}

void CSkill_Power::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CSkill_Power::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (int i = 0; i < iNumMeshes; ++i)
	{
		m_pShaderCom->Begin(1);
		m_pModelCom->Render(i);
	}

#ifdef _DEBUG
#endif // _DEBUG
}

HRESULT CSkill_Power::Add_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Shader_VtxMesh_Effect", L"Com_Shader", reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, L"Prototype_Component_Model_Monster_Arrow", L"Com_Model", reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkill_Power::Bind_ResourceData()
{
	return S_OK;
}

CSkill_Power* CSkill_Power::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CSkill_Power* pInstance = new CSkill_Power(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CSkill_Power::Clone(void* pArg)
{
	CSkill_Power* pInstance = new CSkill_Power(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CSkill_Power::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
}
