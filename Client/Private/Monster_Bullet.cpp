#include "Monster_Bullet.h"

CMonster_Bullet::CMonster_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBullet{ pDevice, pContext }
{
}

CMonster_Bullet::CMonster_Bullet(const CMonster_Bullet& rhs)
	: CBullet{ rhs }
{
}

HRESULT CMonster_Bullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster_Bullet::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CMonster_Bullet::Priority_Tick(const _float& fTimeDelta)
{
}

void CMonster_Bullet::Tick(const _float& fTimeDelta)
{
}

void CMonster_Bullet::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CMonster_Bullet::Render()
{
	return S_OK;
}

void CMonster_Bullet::Check_Coll()
{
}

void CMonster_Bullet::Free()
{
	__super::Free();
}
