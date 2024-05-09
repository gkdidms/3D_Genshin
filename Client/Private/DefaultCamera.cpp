#include "DefaultCamera.h"

#include "GameInstance.h"

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
	DEFAULT_CAMERA_DESC* pDesc = static_cast<DEFAULT_CAMERA_DESC*>(pArg);

	if (nullptr != pDesc)
	{
		m_pTargetMatrix = pDesc->pPlayerMatrix;
		m_pCameraLook = pDesc->pCameraLookMatrix;
		m_fSensor = pDesc->fSensor;
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	XMStoreFloat4x4(&m_OrbitMatrix, XMMatrixIdentity());
	Rotation(&m_OrbitMatrix, XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));
	//m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));
	return S_OK;
}

void CDefaultCamera::Priority_Tick(const _float& fTimeDelta)
{
}

void CDefaultCamera::Tick(const _float& fTimeDelta)
{
	SetCursorPos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f); // 마우스 좌표 적용해주기
	ShowCursor(false);

	_float MouseMove = { 0.f };
	if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_X))
		Turn(&m_OrbitMatrix, XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMove * m_fSensor);
	if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_Z))
		Zoom(fTimeDelta * MouseMove * m_fSensor);

	m_pTransformCom->LookAt(XMLoadFloat4x4(m_pCameraLook).r[3]);

	_matrix ParentMatrix = XMMatrixIdentity();
	ParentMatrix.r[3] = XMLoadFloat4x4(m_pTargetMatrix).r[3];

	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(&m_OrbitMatrix) * ParentMatrix);

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
