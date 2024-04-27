#include "PartObject_Body.h"

#include "GameInstance.h"

CPartObject_Body::CPartObject_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPartObject{ pDevice, pContext }
{
}

CPartObject_Body::CPartObject_Body(const CPartObject_Body& rhs)
	: CPartObject{ rhs }
{
}

HRESULT CPartObject_Body::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPartObject_Body::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	BODY_DESC* pDesc = static_cast<BODY_DESC*>(pArg);
	m_pDirState = pDesc->pDirState;
	m_pFly = pDesc->pFly;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CPartObject_Body::Priority_Tick(const _float& fTimeDelta)
{
}

void CPartObject_Body::Tick(const _float& fTimeDelta)
{
}

void CPartObject_Body::Late_Tick(const _float& fTimeDelta)
{

}

HRESULT CPartObject_Body::Render()
{
	if (FAILED(Bind_ResourceData()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (int i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_Texture", i, aiTextureType_DIFFUSE)))
			continue;
		m_pShaderCom->Begin(0);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CPartObject_Body::Move_Pos(const _float& fTimeDelta, _matrix* MoveMatrix)
{
	if (*m_pFly && *m_pState != PLAYER_FLY_START)
	{
		if (*m_pDirState == CPlayer::DIR_STRIGHT)
			MoveMatrix->r[3] = XMVectorSet(0.f, m_fFlySpeed * fTimeDelta, m_fFlySpeed * fTimeDelta * -1.f, 1.f);
		else if (*m_pDirState == CPlayer::DIR_LEFT_SIDE)
			MoveMatrix->r[3] = XMVectorSet(m_fFlySpeed * fTimeDelta * 1.f, m_fFlySpeed * fTimeDelta, m_fFlySpeed * fTimeDelta * -1.f, 1.f);
		else if (*m_pDirState == CPlayer::DIR_RIGHT_SIDE)
			MoveMatrix->r[3] = XMVectorSet(m_fFlySpeed * fTimeDelta * -1.f, m_fFlySpeed * fTimeDelta, m_fFlySpeed * fTimeDelta * -1.f, 1.f);
		else if (*m_pDirState == CPlayer::DIR_END)
			MoveMatrix->r[3] = XMVectorSet(0.f, m_fFlySpeed * fTimeDelta, 0.f, 1.f);
	}

	if (*m_pState == PLAYER_FALL_ATTACK_LOOP)
	{
		if (*m_pDirState == CPlayer::DIR_STRIGHT)
			MoveMatrix->r[3] = XMVectorSet(0.f, m_fFallDropSpeed * fTimeDelta, m_fFlySpeed * fTimeDelta * -1.f, 1.f);
		else if (*m_pDirState == CPlayer::DIR_LEFT_SIDE)
			MoveMatrix->r[3] = XMVectorSet(m_fFlySpeed * fTimeDelta * 1.f, m_fFallDropSpeed * fTimeDelta, m_fFlySpeed * fTimeDelta * -1.f, 1.f);
		else if (*m_pDirState == CPlayer::DIR_RIGHT_SIDE)
			MoveMatrix->r[3] = XMVectorSet(m_fFlySpeed * fTimeDelta * -1.f, m_fFallDropSpeed * fTimeDelta, m_fFlySpeed * fTimeDelta * -1.f, 1.f);
		else if (*m_pDirState == CPlayer::DIR_END)
			MoveMatrix->r[3] = XMVectorSet(0.f, m_fFallDropSpeed * fTimeDelta, 0.f, 1.f);
	}

}

void CPartObject_Body::Free()
{
	__super::Free();
}
