#include "DefaultCamera.h"

CDefaultCamera::CDefaultCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera{ pDevice, pContext }
{
}

CDefaultCamera::CDefaultCamera(const CDefaultCamera& rhs)
	: CCamera{ rhs }
{
}

HRESULT CDefaultCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDefaultCamera::Initialize(void* pArg)
{
	DEFAULT_CAMERA_DESC* pDesc = (DEFAULT_CAMERA_DESC*)pArg;

	if (nullptr != pDesc)
	{
		m_pTargetMatrix = pDesc->pPlayerMatrix;
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CDefaultCamera::Priority_Tick(const _float& fTimeDelta)
{
}

void CDefaultCamera::Tick(const _float& fTimeDelta)
{
	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pTargetMatrix));
	/*if (GetAsyncKeyState('W') & 0x8000)
		m_pTransformCom->Go_Straight(fTimeDelta);
	if (GetAsyncKeyState('S') & 0x8000)
		m_pTransformCom->Go_Backwork(fTimeDelta);
	if (GetAsyncKeyState('A') & 0x8000)
		m_pTransformCom->Go_Left(fTimeDelta);
	if (GetAsyncKeyState('D') & 0x8000)
		m_pTransformCom->Go_Right(fTimeDelta);*/

	__super::Tick(fTimeDelta);
}

void CDefaultCamera::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CDefaultCamera::Render()
{
	return S_OK;
}

CDefaultCamera* CDefaultCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDefaultCamera* pInstance = new CDefaultCamera(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CDefaultCamera::Clone(void* pArg)
{
	CDefaultCamera* pInstance = new CDefaultCamera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CDefaultCamera::Free()
{
	__super::Free();
}
