#pragma once
#include "PartObject.h"

BEGIN(Client)
class CWeapon :
    public CPartObject
{
public:
    enum WEAPON_TYPE { WEAPON_SWORD, WEAPON_BOW, WEAPON_CATALYSE, WEAPON_END };
    typedef struct tWeaponDesc : public CPartObject::PART_DESC{
        const _float4x4* pHandCombinedTransformationMatrix;
        const _float4x4* pBackCombinedTransformationMatrix;
        WEAPON_TYPE eWeaponType;
    }WEAPON_DESC;


protected:
    CWeapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CWeapon(const CWeapon& rhs);
    virtual ~CWeapon() = default;

public:
    WEAPON_TYPE Get_WeaponType() { return m_WeaponType; }

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Tick(const _float& fTimeDelta);
    virtual void Tick(const _float& fTimeDelta);
    virtual void Late_Tick(const _float& fTimeDelta);
    virtual HRESULT Render();

protected:
    CTexture* m_pTextureCom = { nullptr };
    const _float4x4* m_pSocketMatrix = { nullptr };
    const _float4x4* m_pBackMatrix = { nullptr };

    _bool m_isHide = { false };

    WEAPON_TYPE m_WeaponType = { WEAPON_END };

    _float m_fCurrentTime = { 20.f };
    _float m_fOut = { 1.2f };

protected:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();
    virtual void Change_Animation(const _float& fTimeDelta) = 0;

protected:
    virtual CGameObject* Clone(void* pArg) = 0;
    virtual void Free();
};

END