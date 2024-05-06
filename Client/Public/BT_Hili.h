#pragma once

#include "Node.h"
#include "Hili.h"

BEGIN(Engine)
class CModel;
class CTransform;
class CGameInstance;
class CCollider;
END

BEGIN(Client)
class CBT_Hili : public CNode
{
public:
	typedef struct CBTHiliDesc {
		const CModel* pModel;
		const _float4x4* pTargetMatrix;
		const CHili::HILI_WEAPON_TYPE* pWeaponType;
		CHili::HILI_STATE* pState;
		CTransform* pTransformCom;
		CCollider* pCollider;
		CHili::HILI_INFO* pInfo;
	} BT_HILI_DESC;

protected:
	CBT_Hili();
	virtual ~CBT_Hili() = default;

public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(const _float& fTimeDelta);
	virtual CNode::NODE_STATE Evaluate() override;
	
protected:
	CNode* m_pRootNode = { nullptr };
	CGameInstance* m_pGameInstance = { nullptr };
	_float4x4 m_PreMatrix = {};

protected:
	const CModel* m_pModelCom;
	const _float4x4* m_pTargetMatrix;
	CTransform* m_pTransformCom;
	CCollider* m_pColliderCom;
	CHili::HILI_STATE* m_pState;
	const CHili::HILI_WEAPON_TYPE* m_pWeaponType;
	CHili::HILI_INFO* m_pInfo = { nullptr };


protected:
	_bool m_isDiscovered = { false }; // 인지를 했는가?
	_bool m_isAttack = { false };

	_float m_iAttackRange = { 0.f };
	_float m_iDetectRange = { 0.f };
	_float m_iDiscoverRange = { 20.f };

	_float m_fAttackDelay = { 2.f }; // 2초
	_float m_fAttackTime = { 0.f }; // 공격 후 시간 

protected:
	virtual CNode::NODE_STATE CheckDeath();
	virtual CNode::NODE_STATE Death();

	virtual CNode::NODE_STATE CheckHit();
	virtual CNode::NODE_STATE Hit();
	
	virtual CNode::NODE_STATE CheckAttack();
	virtual CNode::NODE_STATE CheckDiscoverToPlayer();
	virtual CNode::NODE_STATE CheckRangePlayer();
	virtual CNode::NODE_STATE CheckAttackTime();
	virtual CNode::NODE_STATE Attack();

	virtual CNode::NODE_STATE CheckDetect();
	virtual CNode::NODE_STATE CheckLookPlayer();
	virtual CNode::NODE_STATE MoveToPlayer();

	virtual CNode::NODE_STATE MoveToPrePlace();
	virtual CNode::NODE_STATE StandBy();


private:
	virtual void Ready_Node();
	
public:
	virtual void Free() override;
};
END
