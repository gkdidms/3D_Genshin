#include "Bullet.h"

#include "GameInstance.h"

CBullet::CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CBullet::CBullet(const CBullet& rhs)
	:CGameObject{ rhs }
{
}

HRESULT CBullet::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBullet::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	BULLET_DESC* pDesc = static_cast<BULLET_DESC*>(pArg);
	//m_pTargetMatrix = pDesc->pTargetMatrix;
	XMStoreFloat4(&m_vTargetPos ,pDesc->pTargetPos);
	m_fAtk = pDesc->fAtk;
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CBullet::Priority_Tick(const _float& fTimeDelta)
{
}

void CBullet::Tick(const _float& fTimeDelta)
{
}

void CBullet::Late_Tick(const _float& fTimeDelta)
{
}

HRESULT CBullet::Render()
{
	return S_OK;
}



void CBullet::Free()
{
	__super::Free();

	Safe_Release(m_pColliderCom);
}
