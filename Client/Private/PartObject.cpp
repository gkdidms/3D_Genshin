#include "PartObject.h"

#include "GameInstance.h"
#include "StateManager.h"

CPartObject::CPartObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext },
	m_pStateManager { CStateManager::GetInstance() }
{
	Safe_AddRef(m_pStateManager);
}

CPartObject::CPartObject(const CPartObject& rhs)
	: CGameObject { rhs },
	m_pStateManager { rhs.m_pStateManager }
{
	Safe_AddRef(m_pStateManager);
}

HRESULT CPartObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPartObject::Initialize(void* pArg)
{
	PART_DESC* pDesc = (PART_DESC*)pArg;

	if (nullptr != pDesc)
	{
		m_pParentMatrix = pDesc->pParentMatrix;
		m_pState = pDesc->pState;
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CPartObject::Priority_Tick(const _float& fTimeDelta)
{
}

void CPartObject::Tick(const _float& fTimeDelta)
{
}

void CPartObject::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CPartObject::Render()
{
	return S_OK;
}

_vector CPartObject::Targeting()
{
	// 플레이어 시야에 있는 몬스터만 공격하도록 수정하기

	_vector TargetPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	_float fPreDistance = { 0.f };

	vector<CGameObject*> pMonsters = m_pGameInstance->Get_GameObjects(LEVEL_GAMEPLAY, TEXT("Layer_Monster"));

	for (auto& pMonster : pMonsters)
	{
		CTransform* pMonsterTransform = dynamic_cast<CTransform*>(pMonster->Get_Component(TEXT("Com_Transform")));

		//거리
		_vector vDistance = pMonsterTransform->Get_State(CTransform::STATE_POSITION) - XMLoadFloat4x4(m_pParentMatrix).r[3];
		_float fDistance = XMVectorGetX(XMVector3Length(vDistance));

		//시야
		_float fAngle = acosf(XMVectorGetX(XMVector3Dot(XMVector3Normalize(vDistance), XMVector3Normalize(XMLoadFloat4x4(m_pParentMatrix).r[2]))));

		float fFov = 100.f;
		if (fDistance <= 20.f && XMConvertToDegrees(fAngle) < fFov)
		{
			if (fPreDistance == 0.f || fPreDistance > fDistance)
			{
				fPreDistance = fDistance;
				TargetPos = pMonsterTransform->Get_State(CTransform::STATE_POSITION);
			}
		}
	}

	return TargetPos;
}

void CPartObject::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pColliderCom);
	Safe_Release(m_pStateManager);
}
