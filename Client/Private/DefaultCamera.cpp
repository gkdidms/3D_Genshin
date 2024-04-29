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

void CDefaultCamera::isEB(_bool isEB, CPlayer::PLAYERBLE_TYPE ePlayerble)
{
	if (m_ePlayerble == ePlayerble)
		return;

	m_ePlayerble = ePlayerble;
	m_isEB = isEB;
	fEBTime = 0.f;

	//초기화
	switch (m_ePlayerble)
	{
	case Client::CPlayer::PLAYER_TIGHNARI:
	{
		Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-35.f));
		Turn(XMVectorSet(1.f, 0.f, 0.f, 0.f), -1.f);

		_vector CamZoom = XMVectorSet(0.25f, 1.3f, 0.f, 1.f);
		m_pTransformCom->LookAt(CamZoom);

		m_fFovY = 0.15;
		break;
	}
	case Client::CPlayer::PLAYER_NILOU:
	{
		Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.f));

		_vector CamZoom = XMVectorSet(0.f, 1.7f, 0.f, 1.f);
		m_pTransformCom->LookAt(CamZoom);
		break;
	}
	case Client::CPlayer::PLAYER_WANDERER:
	{
		Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.f));
		Turn(XMVectorSet(0.f, 0.f, 1.f, 0.f), -2.3f);

		_vector CamZoom = XMVectorSet(0.1f, 0.7f, 0.f, 1.f);
		m_pTransformCom->LookAt(CamZoom);

		m_fFovY = 0.2;

		break;
	}
	case Client::CPlayer::PLAYER_YAE:
	{
		Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-85.f));
		Turn(XMVectorSet(0.f, 0.f, 0.1f, 0.f), -0.7f);

		_vector CamZoom = XMVectorSet(0.0f, 1.4f, 0.f, 1.f);
		m_pTransformCom->LookAt(CamZoom);

		m_fFovY = 0.3;

		break;
	}
	case Client::CPlayer::PLAYER_TYPE_END:
		XMStoreFloat4x4(&m_OrbitMatrix, XMMatrixIdentity());
		
		Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));
		m_fFovY = XMConvertToRadians(90.f);
		break;
	default:
		break;
	}
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
	Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));
	//m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));
	return S_OK;
}

void CDefaultCamera::Priority_Tick(const _float& fTimeDelta)
{
}

void CDefaultCamera::Tick(const _float& fTimeDelta)
{
	//SetCursorPos(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f); // 마우스 좌표 적용해주기
	//ShowCursor(false);

	if (m_isEB)
	{
		switch (m_ePlayerble)
		{
		case CPlayer::PLAYER_NILOU:
			Nilou_EB(fTimeDelta);
			break;
		case CPlayer::PLAYER_TIGHNARI:
			Tighnari_EB(fTimeDelta);
			break;
		case CPlayer::PLAYER_WANDERER:
			Wanderer_EB(fTimeDelta);
			break;
		case CPlayer::PLAYER_YAE:
			Yae_EB(fTimeDelta);
			break;
		default:
			break;
		}
	}
	else
	{
		_float MouseMove = { 0.f };
		if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_X))
			Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMove * m_fSensor);
		if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_Z))
			Zoom(fTimeDelta * MouseMove * m_fSensor);

		m_pTransformCom->LookAt(XMLoadFloat4x4(m_pCameraLook).r[3]);
	}

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

void CDefaultCamera::Nilou_EB(const _float& fTimeDelta)
{
	fEBTime += fTimeDelta;
	_float fZoomSpeed = -6.f;

	if (fEBTime > 0.8f)
		return;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 15.f, -1.f, 1.f));
	Turn(XMVectorSet(0.f, 0.f, 1.f, 0.f), fTimeDelta * -2.f);

	Zoom(fTimeDelta * fZoomSpeed);
}

void CDefaultCamera::Wanderer_EB(const _float& fTimeDelta)
{
	_float fZoomSpeed = 0.01f;

	Zoom(fTimeDelta * fZoomSpeed);
}

void CDefaultCamera::Tighnari_EB(const _float& fTimeDelta)
{
	_float fZoomSpeed = 0.01f;
	Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * -1.f * m_fSensor);

	Zoom(fTimeDelta * fZoomSpeed);
}

void CDefaultCamera::Yae_EB(const _float& fTimeDelta)
{
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

void CDefaultCamera::Rotation(_fvector vAxis, _float fRadian)
{
	_matrix OrbitMatrix = XMLoadFloat4x4(&m_OrbitMatrix);
	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f);
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f);

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

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
	m_fFovY += fTimeDelta;

	if (m_isEB) return;

	if (m_fFovY > 2.f)
		m_fFovY = 2.f;
	else if (m_fFovY < 0.3f)
		m_fFovY = 0.3f;
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
