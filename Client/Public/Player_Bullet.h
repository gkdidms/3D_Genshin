#pragma once
#include "Bullet.h"

BEGIN(Client)
class CPlayer_Bullet abstract :
    public CBullet
{
protected:
    CPlayer_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CPlayer_Bullet(const CPlayer_Bullet& rhs);
    virtual ~CPlayer_Bullet() = default;

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
