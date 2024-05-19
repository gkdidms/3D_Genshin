#include "BT_Hili.h"

#include "GameInstance.h"

#include "Weapon.h"

#include "Selector.h"
#include "Sequence.h"
#include "Action.h"

CBT_Hili::CBT_Hili()
	: CNode{},
	m_pGameInstance { CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CBT_Hili::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	BT_HILI_DESC* pDesc = static_cast<BT_HILI_DESC*>(pArg);

	m_pModelCom = pDesc->pModel;
	m_pTargetMatrix = pDesc->pTargetMatrix;
	m_pTransformCom = pDesc->pTransformCom;
	m_pState = pDesc->pState;
	m_pWeaponType = pDesc->pWeaponType;
	m_pColliderCom = pDesc->pCollider;
	m_pInfo = pDesc->pInfo;

	m_PreMatrix = *m_pTransformCom->Get_WorldFloat4x4();

	Ready_Node();

	return S_OK;
}

void CBT_Hili::Tick(const _float& fTimeDelta)
{
	this->Evaluate();
}

CNode::NODE_STATE CBT_Hili::Evaluate()
{
	return m_pRootNode->Evaluate();
}

void CBT_Hili::Ready_Node()
{
}

void CBT_Hili::Free()
{
	__super::Free();

	Safe_Release(m_pRootNode);
	Safe_Release(m_pGameInstance);
}

CNode::NODE_STATE CBT_Hili::CheckDeath()
{
	if (*m_pState != CHili::HILI_DIE)
		return CNode::SUCCESS;

	if (m_pModelCom->Get_Animation_Finished())
		return CNode::SUCCESS;

	return CNode::RUNNING;
}

CNode::NODE_STATE CBT_Hili::Death()
{
	// 피가 다 떨어지면 죽음
	if (m_pInfo->fHP < 0.f)
	{
		*m_pState = CHili::HILI_DIE;
		return CNode::SUCCESS;
	}
		
	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Hili::CheckHit()
{
	if (*m_pState != CHili::HILI_HIT)
		return CNode::SUCCESS;

	if (m_pModelCom->Get_Animation_Finished(8) || m_pModelCom->Get_Animation_Finished(9))
		return CNode::SUCCESS;

	return CNode::RUNNING;
}

CNode::NODE_STATE CBT_Hili::Hit()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"), 0));

	// 플레이어가 공격중인가?
	if (false == pPlayer->isAttack())
		return CNode::FAILURE;

	// 플레이어가 근거리인가? 원거리인가?
	// 플레이어가 근거리라면 무기 콜라이더 사용
	// 플레이어가 원거리라면 이펙트 사용
	if (pPlayer->Get_CurrentWeapon() == CWeapon::WEAPON_SWORD)
	{
		//근거리 (소드)
		if (m_pColliderCom->Intersect(pPlayer->Get_SwordCollider()))
		{
			*m_pState = CHili::HILI_HIT;
			return CNode::SUCCESS;
		}
	}
	else
	{
		//원거리
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Hili::CheckAttack()
{
	if (*m_pState != CHili::HILI_NORMAL_ATK)
		return CNode::SUCCESS;

	if (m_pModelCom->Get_Animation_Finished())
		return CNode::SUCCESS;

	return CNode::RUNNING;
}

CNode::NODE_STATE CBT_Hili::CheckDiscoverToPlayer()
{
	if (m_isDiscovered)
	{
		m_pTransformCom->LookAt(XMLoadFloat4x4(m_pTargetMatrix).r[3]);

		return CNode::SUCCESS;
	}

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Hili::CheckRangePlayer()
{
	_vector vTargetPos = XMLoadFloat4x4(m_pTargetMatrix).r[3];
	_vector vHiliPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_vector vDistance = vTargetPos - vHiliPos;
	_float fDistance = sqrtf(XMVectorGetX(vDistance) * XMVectorGetX(vDistance) + XMVectorGetY(vDistance) * XMVectorGetY(vDistance) + XMVectorGetZ(vDistance) * XMVectorGetZ(vDistance));

	if (fDistance <= m_iAttackRange)
		return CNode::SUCCESS;

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Hili::CheckAttackTime()
{
	if (m_isAttack)
	{
		if (m_fAttackDelay < m_fDuration)
		{
			m_fDuration = 0.f;
			m_isAttack = false;
			return CNode::SUCCESS;
		}

		return CNode::FAILURE;
	}

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_Hili::Attack()
{
	*m_pState = CHili::HILI_NORMAL_ATK;

	m_isAttack = true;

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_Hili::CheckDetect()
{
	_vector vTargetPos = XMLoadFloat4x4(m_pTargetMatrix).r[3];
	_vector vSlimePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_vector vDistance = vTargetPos - vSlimePos;
	_float fDistance = sqrtf(XMVectorGetX(vDistance) * XMVectorGetX(vDistance) + XMVectorGetY(vDistance) * XMVectorGetY(vDistance) + XMVectorGetZ(vDistance) * XMVectorGetZ(vDistance));

	if (fDistance <= m_iDetectRange)
		return CNode::SUCCESS;
	else if (fDistance > m_iDetectRange)
	{
		m_isDiscovered = false;
		return CNode::FAILURE;
	}

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_Hili::CheckLookPlayer()
{
	if (!m_isDiscovered)
	{
		_float vLookAngle = { 100.f };

		_vector vTargetPos = XMLoadFloat4x4(m_pTargetMatrix).r[3];
		_vector vHiliPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		_vector vHiliLook = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

		if (AngleOfView(vLookAngle, vTargetPos, vHiliPos, vHiliLook))
		{
			m_isDiscovered = true;
			return CNode::SUCCESS;
		}

		return CNode::FAILURE;
	}
	else m_pTransformCom->LookAt(XMLoadFloat4x4(m_pTargetMatrix).r[3]);

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_Hili::MoveToPlayer() // 플레이어 가까이 다가감
{
	*m_pState = CHili::HILI_RUN;

	return CNode::SUCCESS;
}

CNode::NODE_STATE CBT_Hili::MoveToPrePlace()
{
	// 원래 자리로 돌아감
	_vector vPrePos = XMLoadFloat4x4(&m_PreMatrix).r[3];
	_vector vCurrentPos = m_pTransformCom->Get_WorldMatrix().r[3];

	_vector vDistance = vPrePos - vCurrentPos;
	_float fDistance = sqrtf(XMVectorGetX(vDistance) * XMVectorGetX(vDistance) + XMVectorGetY(vDistance) * XMVectorGetY(vDistance) + XMVectorGetZ(vDistance) * XMVectorGetZ(vDistance));

	if (fDistance <= 0.3f)
		return CNode::SUCCESS;

	m_pTransformCom->LookAt(vPrePos);
	*m_pState = CHili::HILI_WALK_FORWARD;

	return CNode::FAILURE;
}

CNode::NODE_STATE CBT_Hili::StandBy()
{
	*m_pState = CHili::HILI_IDLE;

	return CNode::SUCCESS;
}
