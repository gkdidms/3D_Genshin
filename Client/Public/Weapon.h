#pragma once
#include "PartObject.h"

BEGIN(Client)
class CWeapon :
    public CPartObject
{
public:
    typedef struct tWeaponDesc : public CPartObject::PART_DESC{
        const _float4x4* pHandCombinedTransformationMatrix;
        const _float4x4* pBackCombinedTransformationMatrix;
    
    }WEAPON_DESC;

protected:
    CWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CWeapon(const CWeapon& rhs);
    virtual ~CWeapon() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Tick(const _float& fTimeDelta);
    virtual void Tick(const _float& fTimeDelta);
    virtual void Late_Tick(const _float& fTimeDelta);
    virtual HRESULT Render();

protected:
    const _float4x4* m_pSocketMatrix = { nullptr };
    const _float4x4* m_pBackMatrix = { nullptr };

    _bool m_isHide = { false };

protected:
    virtual HRESULT Add_Components() = 0;
    virtual HRESULT Bind_ResourceData() = 0;
    virtual void Change_Animation(const _float& fTimeDelta) = 0;

protected:
    virtual CGameObject* Clone(void* pArg) = 0;
    virtual void Free();
};

END