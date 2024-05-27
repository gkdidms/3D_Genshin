#pragma once
#include "UI.h"

#include "Player.h"

BEGIN(Client)
class CAvatar_UI :
    public CUI
{
private:
    CAvatar_UI(ID3D11Device* pDevice, ID3D11DeviceContext* pConext);
    CAvatar_UI(const CAvatar_UI& rhs);
    virtual ~CAvatar_UI() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    CGameObject* m_AvatarSides[CPlayer::PLAYER_TYPE_END] = { nullptr };
    CGameObject* m_AvatarSelect[CPlayer::PLAYER_TYPE_END] = { nullptr };

    class CStateManager* m_pStateManager = { nullptr };

private:
    virtual HRESULT Add_Components() { return S_OK; }
    virtual HRESULT Bind_ResourceData() { return S_OK; }
    virtual HRESULT Ready_Btn();

public:
    static CAvatar_UI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pConext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
