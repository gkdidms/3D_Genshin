#include "EffectObject.h"

CEffectObject::CEffectObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBlendObject{ pDevice, pContext}
{
}

CEffectObject::CEffectObject(const CEffectObject& rhs)
	: CBlendObject { rhs },
	m_WorldMatrix{ rhs.m_WorldMatrix },
	m_iShaderPass{ rhs.m_iShaderPass }
{
}

HRESULT CEffectObject::Initialize_Prototype(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	EFFECT_OBJECT_DESC* pDesc = static_cast<EFFECT_OBJECT_DESC*>(pArg);
	m_WorldMatrix = pDesc->WorldMatrix;
	m_iShaderPass = pDesc->iShaderPass;

	
	return S_OK;
}

HRESULT CEffectObject::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	EFFECT_OBJECT_DESC* pDesc = static_cast<EFFECT_OBJECT_DESC*>(pArg);
	m_ParentMatrix = pDesc->pParentMatrix;
	m_isTrailMove = pDesc->isTrailMove;
	m_fSpeed = pDesc->fSpeed;
	m_fDuration = pDesc->fDuration;

	m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&m_WorldMatrix));
	m_pTransformCom->Set_WorldMatrix(XMLoadFloat4x4(&pDesc->RotationMatrix));
	m_pTransformCom->Set_Scale(pDesc->vScale.x, pDesc->vScale.y, pDesc->vScale.z);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&pDesc->vPos));

	return S_OK;
}

void CEffectObject::Priority_Tick(const _float& fTimeDelta)
{
}

void CEffectObject::Tick(const _float& fTimeDelta)
{
}

void CEffectObject::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CEffectObject::Render()
{
	return S_OK;
}

void CEffectObject::Free()
{
	__super::Free();
}
