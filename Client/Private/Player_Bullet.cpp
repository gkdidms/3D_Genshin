#include "Player_Bullet.h"

#include "GameInstance.h"

CPlayer_Bullet::CPlayer_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBullet{ pDevice, pContext }
{
}

CPlayer_Bullet::CPlayer_Bullet(const CPlayer_Bullet& rhs)
	: CBullet{ rhs }
{
}

HRESULT CPlayer_Bullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer_Bullet::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CPlayer_Bullet::Priority_Tick(const _float& fTimeDelta)
{
}

void CPlayer_Bullet::Tick(const _float& fTimeDelta)
{
}

void CPlayer_Bullet::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CPlayer_Bullet::Render()
{
	return S_OK;
}

void CPlayer_Bullet::Check_Coll()
{
	vector<CGameObject*> Monsters = m_pGameInstance->Get_GameObjects(LEVEL_GAMEPLAY, TEXT("Layer_Monster"));

	for (auto& pMonster : Monsters)
	{
		CCollider* pMonsterColl = dynamic_cast<CCollider*>(pMonster->Get_Component(TEXT("Com_Collider")));
		if (m_pColliderCom->Intersect(pMonsterColl))
		{
			//체력 줄이기
			m_isDead = true;
		}
	}
}

void CPlayer_Bullet::Free()
{
	__super::Free();
}
