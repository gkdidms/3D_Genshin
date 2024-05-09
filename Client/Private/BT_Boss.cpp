#include "BT_Boss.h"

#include "GameInstance.h"


CBT_Boss::CBT_Boss()
	: CNode{},
	m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CBT_Boss::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	BT_BOSS_DESC* pDesc = static_cast<BT_BOSS_DESC*>(pArg);
	m_pInfo = pDesc->pInfo;
	m_pState = pDesc->pState;
	m_pModelCom = pDesc->pModel;
	m_pColliderCom = pDesc->pCollider;
	m_pTransformCom = pDesc->pTransform;
	m_pTargetMatrix = pDesc->pTargetMatrix;

	return S_OK;
}

void CBT_Boss::Tick(const _float& fTimeDelta)
{
}

CNode::NODE_STATE CBT_Boss::Evaluate()
{
    return CNode::NODE_STATE();
}

void CBT_Boss::Free()
{
	__super::Free();
	Safe_Release(m_pRootNode);
	Safe_Release(m_pGameInstance);
}
