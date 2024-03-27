#include "Tool_Camera.h"

#include "GameInstance.h"
CTool_Camera::CTool_Camera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera { pDevice, pContext }
{
}

CTool_Camera::CTool_Camera(const CTool_Camera& rhs)
	: CCamera { rhs }
{
}

HRESULT CTool_Camera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTool_Camera::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		TOOLCAMERA_DESC* tDesc = (TOOLCAMERA_DESC*)pArg;

		m_fSensor = tDesc->fSensor;
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CTool_Camera::Priority_Tick(const _float& fTimeDelta)
{
}

void CTool_Camera::Tick(const _float& fTimeDelta)
{
	if (m_pGameInstance->Get_DIKeyState(DIK_W) & 0x80)
		m_pTransformCom->Go_Straight(fTimeDelta);
	if (m_pGameInstance->Get_DIKeyState(DIK_S) & 0x80)
		m_pTransformCom->Go_Backwork(fTimeDelta);
	if (m_pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
		m_pTransformCom->Go_Left(fTimeDelta);
	if (m_pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
		m_pTransformCom->Go_Right(fTimeDelta);

	if (m_pGameInstance->Get_DIKeyState(DIK_TAB) & 0x80)
		m_bCameraMove = !m_bCameraMove;

	if (m_bCameraMove)
	{
		_float MouseMove = { 0.f };
		if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_X))
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * m_fSensor * MouseMove);

		if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_Y))
			m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta * m_fSensor * MouseMove);
	}

	__super::Tick(fTimeDelta);
}

void CTool_Camera::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CTool_Camera::Render()
{
	return S_OK;
}

CTool_Camera* CTool_Camera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTool_Camera* pInstance = new CTool_Camera(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CTool_Camera::Clone(void* pArg)
{
	CTool_Camera* pInstance = new CTool_Camera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CTool_Camera::Free()
{
	__super::Free();
}
