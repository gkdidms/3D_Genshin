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

private:
	CBT_Hili();
	virtual ~CBT_Hili() = default;

public:
	HRESULT Initialize(void* pArg);
	void Tick();
	virtual CNode::NODE_STATE Evaluate() override;
	
private:
	CNode* m_pRootNode = { nullptr };
	CGameInstance* m_pGameInstance = { nullptr };
	_float4x4 m_PreMatrix = {};

private:
	const CModel* m_pModelCom;
	const _float4x4* m_pTargetMatrix;
	CTransform* m_pTransformCom;
	CCollider* m_pColliderCom;
	CHili::HILI_STATE* m_pState;
	const CHili::HILI_WEAPON_TYPE* m_pWeaponType;
	CHili::HILI_INFO* m_pInfo = { nullptr };


private:
	_bool m_isDiscovered = { false }; // 인지를 했는가?

	_float m_iAttackRange = { 2.f };
	_float m_iDetectRange = { 10.f };

	_float m_fAttackDelay = { 2.f }; // 2초
	_float m_fAttackTime = { 0.f }; // 공격 후 시간 

private:
	CNode::NODE_STATE CheckDeath();
	CNode::NODE_STATE Death();

	CNode::NODE_STATE CheckHit();
	CNode::NODE_STATE Hit();
	
	CNode::NODE_STATE CheckAttack();
	CNode::NODE_STATE CheckDiscoverToPlayer();
	CNode::NODE_STATE CheckRangePlayer();
	CNode::NODE_STATE Attack();

	CNode::NODE_STATE CheckDetect();
	CNode::NODE_STATE CheckLookPlayer();
	CNode::NODE_STATE MoveCloserToPlayer();

	CNode::NODE_STATE MoveToPrePlace();
	CNode::NODE_STATE StandBy();
	


private:
	void Ready_Node();
	
public:
	static CBT_Hili* Create(void* pArg);
	virtual void Free();
};
END
