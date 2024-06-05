#pragma once
#include "GameObject.h"

#include "Client_Defines.h"


BEGIN(Engine)
class CCollider;
END

BEGIN(Client)
class CBullet abstract:
    public CGameObject
{
public:
    typedef struct tBulletDesc : public CGameObject::GAMEOBJECT_DESC
    {
        const _float4x4* ParentMatrix;
        _vector pTargetPos;
        _float4x4 HandCombinedTransformationMatrix;
        _float fAtk;
    } BULLET_DESC;
protected:
    CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CBullet(const CBullet& rhs);
    virtual ~CBullet() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Tick(const _float& fTimeDelta);
    virtual void Tick(const _float& fTimeDelta);
    virtual void Late_Tick(const _float& fTimeDelta);
    virtual HRESULT Render();

protected:
    CCollider* m_pColliderCom = { nullptr };

    _float4 m_vTargetPos = {};
    const _float4x4* m_pTargetMatrix = { nullptr };
    _float4 m_vTargetLook = {};
    _float m_fAtk = { 0.f };
protected:
    //지속시간
    _float m_fDuration = { 5.f };
    _float m_fCurrentTime = { 0.f };

    _float m_fSpeed = { 0.f };

public:
    virtual CGameObject* Clone(void* pArg) = 0;
    virtual void Free() override;
};
END
