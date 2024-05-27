#include "Camera.h"

#include "GameInstance.h"

CCamera::CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CCamera::CCamera(const CCamera& rhs)
	: CGameObject{ rhs }
{
}

HRESULT CCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera::Initialize(void* pArg)
{
	CAMERA_DESC* pCameraDesc = (CAMERA_DESC*)pArg;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&pCameraDesc->vEye));
	m_pTransformCom->LookAt(XMLoadFloat4(&pCameraDesc->vFocus));

	m_fFovY = pCameraDesc->fFovY;
	m_fAspect = pCameraDesc->fAspect;
	m_fNear = pCameraDesc->fNear;
	m_fFar = pCameraDesc->fFar;


	return S_OK;
}

void CCamera::Priority_Tick(const _float& fTimeDelta)
{
}

void CCamera::Tick(const _float& fTimeDelta)
{
	m_pGameInstance->Set_Transform(CPipeLine::D3DTS_VIEW, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));
	m_pGameInstance->Set_Transform(CPipeLine::D3DTS_PROJ, XMMatrixPerspectiveFovLH(m_fFovY, m_fAspect, m_fNear, m_fFar));
	m_pGameInstance->Set_CamFar(m_fFar);
}

void CCamera::Late_Tick(const _float& fTimeDelta)
{
}


void CCamera::Turn(_float4x4* OrbitMatrix, _fvector vAxis, const _float& fTimeDelta)
{
	_matrix SubMatrix = XMLoadFloat4x4(OrbitMatrix);
	_vector		vRight = SubMatrix.r[0];
	_vector		vUp = SubMatrix.r[1];
	_vector		vLook = SubMatrix.r[2];

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, XMConvertToRadians(45.f) * fTimeDelta);

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	SubMatrix.r[0] = vRight;
	SubMatrix.r[1] = vUp;
	SubMatrix.r[2] = vLook;

	XMStoreFloat4x4(OrbitMatrix, SubMatrix);
}

void CCamera::Rotation(_float4x4* OrbitMatrix, _fvector vAxis, _float fRadian)
{
	_matrix SubMatrix = XMLoadFloat4x4(OrbitMatrix);
	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f);
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f);

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	SubMatrix.r[0] = vRight;
	SubMatrix.r[1] = vUp;
	SubMatrix.r[2] = vLook;

	XMStoreFloat4x4(OrbitMatrix, SubMatrix);
}

void CCamera::Zoom(const _float& fTimeDelta)
{
	//_vector vDirect = XMVector3Normalize(m_pGameInstance->Get_CamLook());
	//_vector vCamPos = m_pGameInstance->Get_CamPosition();
	//if (fTimeDelta < 0)
	//	vCamPos -= vDirect;
	//else if (fTimeDelta > 0)
	//	vCamPos += vDirect;

	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCamPos);

	m_fFovY += fTimeDelta;

	if (m_fFovY > 2.f)
		m_fFovY = 2.f;
	else if (m_fFovY < 0.03f)
		m_fFovY = 0.03f;
}

HRESULT CCamera::Render()
{
	return S_OK;
}

void CCamera::Free()
{
	__super::Free();
}
