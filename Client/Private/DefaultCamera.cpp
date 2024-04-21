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
	DEFAULT_CAMERA_DESC* pDesc = (DEFAULT_CAMERA_DESC*)pArg;

	if (nullptr != pDesc)
	{
		m_pTargetMatrix = pDesc->pPlayerMatrix;
		m_pCameraLook = pDesc->pCameraLookMatrix;
		m_fSensor = pDesc->fSensor;
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	XMStoreFloat4x4(&m_OrbitMatrix, XMMatrixIdentity());
	return S_OK;
}

void CDefaultCamera::Priority_Tick(const _float& fTimeDelta)
{
}

void CDefaultCamera::Tick(const _float& fTimeDelta)
{
	_float MouseMove = { 0.f };
	if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_X))
		Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMove * m_fSensor);
	if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_Z))
		Zoom(fTimeDelta * MouseMove * m_fSensor);

	_matrix ParentMatrix = XMMatrixIdentity();
	ParentMatrix.r[3] = XMLoadFloat4x4(m_pTargetMatrix).r[3];

	m_pTransformCom->LookAt(XMLoadFloat4x4(m_pCameraLook).r[3]);

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

void CDefaultCamera::Turn(_fvector vAxis, const _float& fTimeDelta)
{
	_matrix OrbitMatrix = XMLoadFloat4x4(&m_OrbitMatrix);
	_vector		vRight = OrbitMatrix.r[0];
	_vector		vUp = OrbitMatrix.r[1];
	_vector		vLook = OrbitMatrix.r[2];

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, XMConvertToRadians(45.f) * fTimeDelta);

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	OrbitMatrix.r[0] = vRight;
	OrbitMatrix.r[1] = vUp;
	OrbitMatrix.r[2] = vLook;

	XMStoreFloat4x4(&m_OrbitMatrix, OrbitMatrix);
}

void CDefaultCamera::Zoom(const _float& fTimeDelta)
{
	_vector m_CamPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector m_CamLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);

	_vector m_CamZoom = fTimeDelta < 0 ? m_CamPos + m_CamLook : m_CamPos - m_CamLook;

	m_CamZoom = XMVectorSetW(m_CamZoom, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_CamZoom);
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
