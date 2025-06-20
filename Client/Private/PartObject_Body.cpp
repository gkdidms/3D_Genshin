#include "PartObject_Body.h"

#include "GameInstance.h"
#include "StateManager.h"

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
	m_pHill = pDesc->pHill;

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

		if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture", i, aiTextureType_DIFFUSE)))
			continue;
		m_pShaderCom->Begin(0);
		m_pModelCom->Render(i);
	}

	return S_OK;
}

void CPartObject_Body::Set_HP(_float fDamage)
{
	m_Info.m_fHp -= fDamage;

	if (m_Info.m_fHp <= 0.f)
	{
		m_Info.m_fHp = 0.f;
	}
}

void CPartObject_Body::Move_Pos(const _float& fTimeDelta, _matrix* MoveMatrix)
{

	if (m_pStateManager->isFly())
	{
		if (*m_pState == PLAYER_FLY_START)
		{
			m_fStartSpeed -= 0.02f;
			//스타트일때 
			MoveMatrix->r[3] = XMVectorSet(0.f, m_fStartSpeed * fTimeDelta, 0.f, 1.f);
			return;
		}

		if (m_pStateManager->isCollWindField())
		{
			m_fStartSpeed = { 0.f };

			MoveMatrix->r[3] = XMVectorSet(0.f, m_fWindSpeed * fTimeDelta, 0.f, 1.f);

			if (*m_pDirState == CPlayer::DIR_STRIGHT)
				MoveMatrix->r[3] = XMVectorSetZ(MoveMatrix->r[3], m_fFlySpeed * fTimeDelta * -1.f);
			else if (*m_pDirState == CPlayer::DIR_LEFT_SIDE)
				MoveMatrix->r[3] = XMVectorSetX(MoveMatrix->r[3], m_fFlySpeed * fTimeDelta * 1.f);
			else if (*m_pDirState == CPlayer::DIR_RIGHT_SIDE)
				MoveMatrix->r[3] = XMVectorSetX(MoveMatrix->r[3], m_fFlySpeed * fTimeDelta * -1.f);

			return;
		}

		// 플레이어가 날고 있고, 윈드필더를 타고 있지 않을때

		if (*m_pDirState == CPlayer::DIR_STRIGHT)
			MoveMatrix->r[3] = XMVectorSet(0.f, m_fFlySpeed * fTimeDelta, m_fFlySpeed * fTimeDelta * -1.f, 1.f);
		else if (*m_pDirState == CPlayer::DIR_LEFT_SIDE)
			MoveMatrix->r[3] = XMVectorSet(m_fFlySpeed * fTimeDelta * 1.f, m_fFlySpeed * fTimeDelta, m_fFlySpeed * fTimeDelta * -1.f, 1.f);
		else if (*m_pDirState == CPlayer::DIR_RIGHT_SIDE)
			MoveMatrix->r[3] = XMVectorSet(m_fFlySpeed * fTimeDelta * -1.f, m_fFlySpeed * fTimeDelta, m_fFlySpeed * fTimeDelta * -1.f, 1.f);
		else if (*m_pDirState == CPlayer::DIR_END)
			MoveMatrix->r[3] = XMVectorSet(0.f, m_fFlySpeed * fTimeDelta, 0.f, 1.f);
	}

	// 플레이어가 떨어질때
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

