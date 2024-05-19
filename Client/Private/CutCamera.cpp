#include "CutCamera.h"

#include "GameInstance.h"
#include "Player.h"

CCutCamera::CCutCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera{ pDevice, pContext}
{
}

CCutCamera::CCutCamera(const CCutCamera& rhs)
	: CCamera{ rhs }
{
}

HRESULT CCutCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCutCamera::Initialize(void* pArg)
{
	Cut_CAMERA_DESC* pDesc = static_cast<Cut_CAMERA_DESC*>(pArg);

	if (nullptr != pDesc)
	{
		m_pTargetMatrix = pDesc->pPlayerMatrix;
		m_pCameraLook = pDesc->pCameraLookMatrix;
		m_fSensor = pDesc->fSensor;

		m_vEye = pDesc->vEye;
		m_vFocus = pDesc->vFocus;
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	XMStoreFloat4x4(&m_OrbitMatrix, XMMatrixIdentity());
	Rotation(&m_OrbitMatrix, XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));
	//m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));

	return S_OK;
}

void CCutCamera::Priority_Tick(const _float& fTimeDelta)
{
}

void CCutCamera::Tick(const _float& fTimeDelta)
{
	if (m_isCutScene)
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
		case CPlayer::PLAYER_FEIYAN:
			Yae_EB(fTimeDelta);
			break;
		default:
			break;
		}
	}

	_matrix ParentMatrix = XMMatrixIdentity();
	ParentMatrix.r[3] = XMLoadFloat4x4(m_pTargetMatrix).r[3];

	XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(&m_OrbitMatrix) * ParentMatrix);

	if (m_isCutScene)
		__super::Tick(fTimeDelta);
}

void CCutCamera::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CCutCamera::Render()
{
	return S_OK;
}

void CCutCamera::isCutScene(_bool isCutScene, _uint ePlayerble)
{
	if (m_ePlayerble == ePlayerble)
		return;

	m_isCutScene = isCutScene;
	m_ePlayerble = ePlayerble;
	fEBTime = 0.f;

	//초기화
	switch (m_ePlayerble)
	{
	case Client::CPlayer::PLAYER_TIGHNARI:
	{
		Rotation(&m_OrbitMatrix, XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-30.f));
		Turn(&m_OrbitMatrix, XMVectorSet(1.f, 0.f, 0.0f, 0.f), -1.f);

		_vector CamZoom = XMVectorSet(0.25f, 1.3f, 0.f, 1.f);
		m_pTransformCom->LookAt(CamZoom);

		m_fFovY = 0.18;
		break;
	}
	case Client::CPlayer::PLAYER_NILOU:
	{
		Rotation(&m_OrbitMatrix, XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.f));
		m_pTransformCom->Turn(XMVectorSet(1.f, 0.f, 0.f, 0.f), 20.f);
		break;
	}
	case Client::CPlayer::PLAYER_WANDERER:
	{
		Rotation(&m_OrbitMatrix, XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.f));
		Turn(&m_OrbitMatrix, XMVectorSet(0.f, 0.f, 1.f, 0.f), -2.3f);

		_vector CamZoom = XMVectorSet(0.1f, 0.7f, 0.f, 1.f);
		m_pTransformCom->LookAt(CamZoom);

		m_fFovY = 0.2;

		break;
	}
	case Client::CPlayer::PLAYER_FEIYAN:
	{
		//연비용 컷씬 만들기
		Rotation(&m_OrbitMatrix, XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-85.f));
		Turn(&m_OrbitMatrix, XMVectorSet(0.f, 0.f, 1.0f, 0.f), -0.7f);

		_vector CamZoom = XMVectorSet(0.0f, 1.4f, 0.f, 1.f);
		m_pTransformCom->LookAt(CamZoom);

		m_fFovY = 0.3;

		break;
	}
	case Client::CPlayer::PLAYER_TYPE_END:
		Reset();
		break;
	default:
		break;
	}
}

void CCutCamera::Zoom(const _float& fTimeDelta)
{
	m_fFovY += fTimeDelta;

	if (m_fFovY > 2.f)
		m_fFovY = 2.f;
	else if (m_fFovY < 0.03f)
		m_fFovY = 0.03f;
}

void CCutCamera::Reset()
{
	XMStoreFloat4x4(&m_OrbitMatrix, XMMatrixIdentity());

	Rotation(&m_OrbitMatrix, XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));
	m_fFovY = XMConvertToRadians(90.f);

	m_pTransformCom->Set_WorldMatrix(XMMatrixIdentity());
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&m_vEye));
	m_pTransformCom->LookAt(XMLoadFloat4(&m_vFocus));
}

void CCutCamera::Nilou_EB(const _float& fTimeDelta)
{
	_vector CamZoom = XMVectorSet(0.0f, 1.4f, 1.1f, 1.f);
	m_pTransformCom->LookAt(CamZoom);

	fEBTime += fTimeDelta;
	_float fZoomSpeed = -2.f;

	if (fEBTime > 1.4f)
		m_isCutScene = false;
	if (fEBTime > 0.8f)
		return;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.f, 10.f, -0.1f, 1.f));
	Turn(&m_OrbitMatrix, XMVectorSet(0.f, 0.f, 1.f, 0.f), fTimeDelta * -1.8f);
	Zoom(fTimeDelta * fZoomSpeed);
}

void CCutCamera::Wanderer_EB(const _float& fTimeDelta)
{
	_float fZoomSpeed = 0.01f;

	Zoom(fTimeDelta * fZoomSpeed);
}

void CCutCamera::Tighnari_EB(const _float& fTimeDelta)
{
	_float fZoomSpeed = 0.01f;
	Turn(&m_OrbitMatrix, XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * -1.f * m_fSensor);

	Zoom(fTimeDelta * fZoomSpeed);
}

void CCutCamera::Yae_EB(const _float& fTimeDelta)
{
}

CCutCamera* CCutCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCutCamera* pInstance = new CCutCamera(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CGameObject* CCutCamera::Clone(void* pArg)
{
	CCutCamera* pInstance = new CCutCamera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CCutCamera::Free()
{
	__super::Free();
}
