#include "Transform.h"

#include "GameInstance.h"

#include "GameObject.h"
#include "Shader.h"
#include "Navigation.h"

CTransform::CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent{ pDevice, pContext }
{
}

CTransform::CTransform(const CTransform& rhs)
	: CComponent{ rhs },
	m_WorldMatrix{ rhs.m_WorldMatrix },
	m_fSpeedPerSec{ rhs.m_fSpeedPerSec },
	m_fRotationPerSec{ rhs.m_fRotationPerSec },
	m_fPower{ rhs.m_fPower },
	m_fAccelTime{ rhs.m_fAccelTime }
{
}

_vector CTransform::Get_State(STATE eState)
{
	return XMLoadFloat4x4(&m_WorldMatrix).r[eState];
}

void CTransform::Set_State(STATE eState, _fvector vState)
{
	_matrix matWorld = XMLoadFloat4x4(&m_WorldMatrix);

	matWorld.r[eState] = vState;

	XMStoreFloat4x4(&m_WorldMatrix, matWorld);
}

void CTransform::Scaling(_float fScaleX, _float fScaleY, _float fScaleZ)
{
	Set_State(STATE_RIGHT, Get_State(STATE_RIGHT) * fScaleX);
	Set_State(STATE_UP, Get_State(STATE_UP) * fScaleY);
	Set_State(STATE_LOOK, Get_State(STATE_LOOK) * fScaleZ);
}

void CTransform::Set_Scale(_float fScaleX, _float fScaleY, _float fScaleZ)
{
	Set_State(STATE_RIGHT, XMVector3Normalize(Get_State(STATE_RIGHT)) * fScaleX);
	Set_State(STATE_UP, XMVector3Normalize(Get_State(STATE_UP)) * fScaleY);
	Set_State(STATE_LOOK, XMVector3Normalize(Get_State(STATE_LOOK)) * fScaleZ);
}

HRESULT CTransform::Initialize_Prototype()
{
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	return S_OK;
}

HRESULT CTransform::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		TRANSFORM_DESC* pTransformDesc = (TRANSFORM_DESC*)pArg;

		m_fSpeedPerSec = pTransformDesc->fSpeedPecSec;
		m_fRotationPerSec = pTransformDesc->fRotatePecSec;
		m_fPower = pTransformDesc->fPower;
	}

	return S_OK;
}

HRESULT CTransform::Bind_ShaderMatrix(CShader* pShader, const _char* pConstantName)
{
	return pShader->Bind_Matrix(pConstantName, &m_WorldMatrix);
}

_bool CTransform::Go_Run(const _matrix vMoveMatrix, CNavigation* pNavigationCom, _bool isFlyMove)
{
	_vector vPos = XMVectorSetW(XMVector3TransformCoord(vMoveMatrix.r[3], Get_WorldMatrix()), 1.f);

	if (nullptr == pNavigationCom ? false : !pNavigationCom->isMove(vPos))
		return false;

	if (pNavigationCom->Get_OptionType() == CCell::OPTION_FLY && pNavigationCom->isFlyCell(XMVector3Normalize(Get_State(STATE_LOOK))) && !isFlyMove)
		return false;
		
	Set_State(CTransform::STATE_POSITION, vPos);
	return true;
}

_bool CTransform::Check_Radius_AfterRun(const _matrix vMoveMatrix, CNavigation* pNavigationCom, _vector vRadius)
{
	_vector vPos = XMVector3TransformCoord(vMoveMatrix.r[3], Get_WorldMatrix());

	_vector vPosToRadiuse = XMVectorSetW(vPos + vRadius, 1.f);
	if (nullptr == pNavigationCom ? false : !pNavigationCom->isMove(vPosToRadiuse))
		return false;

	return true;
}

_bool CTransform::Go_Coll(const _fvector vMovePos, class CNavigation* pNavigationCom)
{
	if (nullptr == pNavigationCom ? false : !pNavigationCom->isMove(vMovePos))
		return false;

	if (pNavigationCom->Get_OptionType() == CCell::OPTION_FLY && pNavigationCom->isFlyCell(XMVector3Normalize(Get_State(STATE_LOOK))))
		return false;


	Set_State(STATE_POSITION, vMovePos);
}

_bool CTransform::Jump(const _float& fTimeDelta, _float fHeight, _float fAccelTime)
{
	m_fAccelTime += fAccelTime;

	_float fTemp = (m_fPower * m_fAccelTime) - (9.8f * m_fAccelTime * m_fAccelTime * 0.5f);

	_vector vPosition = Get_State(STATE_POSITION);
	_vector vLook = Get_State(STATE_LOOK);

	_float fY = XMVectorGetY(vPosition);
	vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;
	vPosition = XMVectorSetY(vPosition, fY + fTemp);

	Set_State(CTransform::STATE_POSITION, vPosition);

	if (fHeight >= XMVectorGetY(vPosition))
	{
		vPosition = XMVectorSetY(vPosition, fHeight);
		Set_State(CTransform::STATE_POSITION, vPosition);

		return false;
	}

	return true;
}

void CTransform::Go_Straight(const _float& fTimeDelta)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vLook = Get_State(STATE_LOOK);

	vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;


	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Backwork(const _float& fTimeDelta)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vLook = Get_State(STATE_LOOK);

	vPosition -= XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;


	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Left(const _float& fTimeDelta)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vRight = Get_State(STATE_RIGHT);

	vPosition -= XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Right(const _float& fTimeDelta)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vRight = Get_State(STATE_RIGHT);

	vPosition += XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

void CTransform::LookAt(_fvector vTargetPosition)
{
	_float3 m_vScale = Get_Scaled();

	_vector vLook = vTargetPosition - Get_State(STATE_POSITION);
	_vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	_vector vUp = XMVector3Cross(vLook, vRight);

	Set_State(STATE_RIGHT, XMVector4Normalize(vRight) * m_vScale.x);
	Set_State(STATE_UP, XMVector4Normalize(vUp) * m_vScale.y);
	Set_State(STATE_LOOK, XMVector4Normalize(vLook) * m_vScale.z);
}

void CTransform::LookForCamera(_fvector vCamLook, _float fRadian)
{
	_float3 m_vScale = Get_Scaled();

	_vector vChangeCamLook = XMVectorSetY(vCamLook, 0.f);
	_vector vLook = XMVector4Normalize(vChangeCamLook);
	_vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	_vector vUp = XMVector3Cross(vLook, vRight);

	_matrix		RotationMatrix = XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), fRadian);

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(STATE_RIGHT, XMVector4Normalize(vRight) * m_vScale.x);
	Set_State(STATE_UP, XMVector4Normalize(vUp) * m_vScale.y);
	Set_State(STATE_LOOK, XMVector4Normalize(vLook) * m_vScale.z);
}

void CTransform::Turn(_fvector vAxis, _float fTimeDelta)
{
	_vector		vRight = Get_State(STATE_RIGHT);
	_vector		vUp = Get_State(STATE_UP);
	_vector		vLook = Get_State(STATE_LOOK);

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, m_fRotationPerSec * fTimeDelta);

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::Rotation(_fvector vAxis, _float fRadian)
{
	_float3		vScaled = Get_Scaled();

	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScaled.x;
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScaled.y;
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScaled.z;

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::WorldRotation(_fvector vAxis, _float fRadian)
{
	_vector		vRight = Get_State(STATE_RIGHT);
	_vector		vUp = Get_State(STATE_UP);
	_vector		vLook = Get_State(STATE_LOOK);

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::Rotation(_float fRotationX, _float fRotationY, _float fRotationZ)
{
	_float3		vScaled = Get_Scaled();

	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScaled.x;
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScaled.y;
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScaled.z;

	_matrix RotationXMatrix = XMMatrixRotationX(fRotationX);
	_matrix RotationYMatrix = XMMatrixRotationX(fRotationY);
	_matrix RotationZMatrix = XMMatrixRotationX(fRotationZ);

	_matrix RotationMatrix = RotationXMatrix * RotationYMatrix * RotationZMatrix;

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::Get_Rotation(_fvector vAxis, _float fRadian, _float4x4* TargetMatrix)
{
	_vector		vRight = XMLoadFloat4x4(TargetMatrix).r[0];
	_vector		vUp = XMLoadFloat4x4(TargetMatrix).r[1];
	_vector		vLook = XMLoadFloat4x4(TargetMatrix).r[2];

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	_matrix ChangeMatrix = XMLoadFloat4x4(TargetMatrix);
	ChangeMatrix.r[0] = vRight;
	ChangeMatrix.r[1] = vUp;
	ChangeMatrix.r[2] = vLook;

	XMStoreFloat4x4(TargetMatrix, ChangeMatrix);
}

void CTransform::Billboard(_float4 CamPosition)
{

}

void CTransform::Get_RootTransformationMatrix(_fmatrix RootMatrix)
{
	_vector vScale, vRotation, vTranslation;

	// 1. 루트 애니메이션 (현재 프레임 - 이전 프레임 행렬) 행렬의 
	XMMatrixDecompose(&vScale, &vRotation, &vTranslation, RootMatrix);
}

CTransform* CTransform::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTransform* pInstance = new CTransform(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* CTransform::Clone(void* pArg)
{
	CTransform* pInstance = new CTransform(*this);

	if (FAILED(pInstance->Initialize(pArg)))
		Safe_Release(pInstance);

	return pInstance;
}

void CTransform::Free()
{
	__super::Free();
}
