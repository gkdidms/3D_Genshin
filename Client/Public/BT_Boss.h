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
	// ���� ������ ������ ����� ��� �������� ī��Ʈ ���� 
	_uint m_iAttackCount = { 0 };
	_uint m_iCurrentAttackCount = { 0 };

protected:
	// �÷��̾ ������ ���� ���� ī����
	_uint m_iPlayerDealing = { 0 };

protected:
	// ���� ������ ����
	_float m_fDelayTime = { 7.f };
	_float m_fCurrentTime = { 0.f };

public:
	virtual void Free() override;
};

END