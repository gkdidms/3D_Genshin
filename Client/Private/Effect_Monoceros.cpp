#include "Effect_Monoceros.h"

#include "GameInstance.h"

CEffect_Monoceros::CEffect_Monoceros(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEffect{ pDevice, pContext }
{
}

CEffect_Monoceros::CEffect_Monoceros(const CEffect_Monoceros& rhs)
	: CEffect{ rhs }
{
}

HRESULT CEffect_Monoceros::Initialize_Prototype(const _char* szFileName)
{
	if (FAILED(__super::Initialize_Prototype(szFileName)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Monoceros::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_fHeight = XMVectorGetY(XMLoadFloat4x4(m_pParentMatrix).r[3]);

	return S_OK;
}

void CEffect_Monoceros::Priority_Tick(const _float& fTimeDelta)
{
	__super::Priority_Tick(fTimeDelta);
}

void CEffect_Monoceros::Tick(const _float& fTimeDelta)
{
	__super::Tick(fTimeDelta);

	m_fAccelTime += fTimeDelta * 0.1f;

	_float fTemp = (m_fPower * m_fAccelTime) - (9.8f * m_fAccelTime * m_fAccelTime * 0.5f);

	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float fY = XMVectorGetY(vPosition);
	vPosition += m_vTargetDir * m_fSpeed * fTimeDelta;
	vPosition = XMVectorSetY(vPosition, fY - fTemp);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

	if (m_fHeight >= XMVectorGetY(vPosition))
	{
		// 충돌했을때 이펙트 출력
	}
}

void CEffect_Monoceros::Late_Tick(const _float& fTimeDelta)
{
	__super::Late_Tick(fTimeDelta);
}

HRESULT CEffect_Monoceros::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Monoceros::Add_Components()
{
	if (FAILED(__super::Add_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Monoceros::File_Open(const _char* szFileName)
{
	if (FAILED(__super::File_Open(szFileName)))
		return E_FAIL;

	return S_OK;
}

CEffect_Monoceros* CEffect_Monoceros::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* szFilePath)
{
	CEffect_Monoceros* pInstance = new CEffect_Monoceros(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(szFilePath)))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CEffect_Monoceros::Clone(void* pArg)
{
	CEffect_Monoceros* pInstance = new CEffect_Monoceros(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CEffect_Monoceros::Free()
{
	__super::Free();
}
