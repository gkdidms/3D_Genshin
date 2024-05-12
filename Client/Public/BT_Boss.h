#pragma once
#include "Node.h"

#include "Boss_Body.h"

BEGIN(Engine)
class CModel;
class CCollider;
class CTransform;
class CGameInstance;
END

BEGIN(Client)
class CBT_Boss abstract:
    public CNode
{
public:
	typedef struct tBTEvilEyeDesc {
		CModel* pModel;
		CCollider* pCollider;
		CTransform* pTransform;
		_uint* pState;
		CBoss_Body::TARTAGLIA_INFO* pInfo;
		const _float4x4* pTargetMatrix;
	}BT_BOSS_DESC;

protected:
	CBT_Boss();
	virtual ~CBT_Boss() = default;

public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(const _float& fTimeDelta);
	virtual virtual CNode::NODE_STATE Evaluate() override;

protected:
	CGameInstance* m_pGameInstance = { nullptr };
	CNode* m_pRootNode = { nullptr };
	_uint* m_pState = { nullptr };
	CBoss_Body::TARTAGLIA_INFO* m_pInfo = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	const _float4x4* m_pTargetMatrix = { nullptr };

protected:
	_bool m_isAttack = { false };
	_bool m_isFirst = { true };
	_bool m_isWalkStop = { false };

protected:
	_uint m_iBladeNormalAttackMax = { 0 };
	_uint m_iBladeNormalAttackMaxCount = { 0 };

protected:
	// 연속 공격이 가능한 모션의 경우 랜덤으로 카운트 가능 
	_uint m_iAttackCount = { 0 };
	_uint m_iCurrentAttackCount = { 0 };

protected:
	// 플레이어가 피해을 입힌 숫자 카운팅
	_uint m_iPlayerDealing = { 0 };

protected:
	// 공격 딜레이 저장
	_float m_fDelayTime = { 7.f };
	_float m_fCurrentTime = { 0.f };

public:
	virtual void Free() override;
};

END