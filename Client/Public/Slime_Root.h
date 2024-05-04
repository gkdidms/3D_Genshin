#pragma once
#include "Node.h"

#include "Client_Defines.h"
#include "Slime_Large.h"

BEGIN(Engine)
class CModel;
class CTransform;
END

BEGIN(Client)
class CSlime_Root :
    public CNode
{
public:
    typedef struct tSlimeLargeRootDesc
    {
        const CModel* pModel;
        const _float4x4* pTargetMatrix;
        CSlime_Large::SLIME_LARGE_STATE* pState;
        CTransform* pTransformCom;

    }SLIME_LARGE_ROOT_DESC;

private:
    CSlime_Root();
    virtual ~CSlime_Root() = default;

public:
    HRESULT Initialize(void* pArg);
    void Tick();
    NODE_STATE Evaluate();

private:
    const CModel* m_pModelCom;
    CTransform* m_pTransformCom;

    const _float4x4* m_pTargetMatrix;
    CSlime_Large::SLIME_LARGE_STATE* m_pState;

private: // Root Node�� ������ �ִ� ù��° Node
    CNode* m_pNode = { nullptr };

private:
    _bool m_IsJumpAtk = { false }; // ���� ������ ��������?

    _float m_iAttackRange = { 3.f }; // ���� ���� ����
    _float m_iDetectRange = { 5.f }; // �÷��̾� ���� ���� ����

private:
    void Ready_Node();

private:
    CNode::NODE_STATE CheckAttack();
    CNode::NODE_STATE CheckLookPlayer();
    CNode::NODE_STATE CheckAttackRange();

    CNode::NODE_STATE AttackJump();
    CNode::NODE_STATE AttackNormal();

    CNode::NODE_STATE CheckDetectPlayer();
    CNode::NODE_STATE CheckMovePlayer();
    CNode::NODE_STATE MoveCloserPlayer();

    CNode::NODE_STATE CheckHit();
    CNode::NODE_STATE Hit();

    CNode::NODE_STATE Undo();

public:
    static CSlime_Root* Create(void* pArg);
    virtual void Free();
};
END