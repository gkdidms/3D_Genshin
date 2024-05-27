#pragma once
#include "UI.h"

BEGIN(Client)
class CAvatar_Side :
    public CUI
{
public:
    typedef struct tAvatarSideDesc: public CGameObject::GAMEOBJECT_DESC {
        _uint iPlayerble;
        _float2 vPos;
    } AVATAR_SIDE_DESC;
private:
    CAvatar_Side(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CAvatar_Side(const CAvatar_Side& rhs);
    virtual ~CAvatar_Side() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    CGameObject* m_pFace = { nullptr };
    CGameObject* m_pNumBtn = { nullptr };
    _uint m_iPlayerble = { 0 };

private:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();
    virtual HRESULT Ready_Btn();

public:
    static CAvatar_Side* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
