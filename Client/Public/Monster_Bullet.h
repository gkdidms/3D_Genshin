#pragma once
#include "Bullet.h"

BEGIN(Client)
class CMonster_Bullet :
	public CBullet
{
protected:
	CMonster_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_Bullet(const CMonster_Bullet& rhs);
	virtual ~CMonster_Bullet() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Tick(const _float& fTimeDelta);
    virtual void Tick(const _float& fTimeDelta);
    virtual void Late_Tick(const _float& fTimeDelta);
    virtual HRESULT Render();

protected:
    void Check_Coll();

public:
	virtual void Free() override;
};
END
