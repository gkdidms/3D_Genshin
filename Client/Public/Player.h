#pragma once
#include "GameObject.h"

#include "Client_Defines.h"

BEGIN(Client)
class CPlayer :
    public CGameObject
{
public:
    enum PLAYERBLE_TYPE { PLAYER_TIGHNARI, PLAYER_NILOU, PLAYER_WANDERER, PLAYER_YAE, PLAYER_TYPE_END };
    enum PLAYER_PART { PART_BODY, PART_WEAPON, PART_END };
   

private:
    CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CPlayer(const CPlayer& rhs);
    virtual ~CPlayer() = default;

public:
    HRESULT Initialize_Prototype();
    HRESULT Initialize(void* pArg);
    void Priority_Tick(const _float& fTimeDelta);
    void Tick(const _float& fTimeDelta);
    void Late_Tick(const _float& fTimeDelta);
    HRESULT Render();

private:
    vector<class CPartObject*> m_PartObject[PLAYER_TYPE_END];

    _uint m_iState = { 0 };
    _uint m_iNumMouseClick = { 0 };
    _uint m_iMaxMouseClick = { 4 };

    PLAYERBLE_TYPE m_CurrentPlayerble = { PLAYER_WANDERER };

    class CState_Manager* m_pState_Manager = { nullptr };

private:
    void Change_State(PLAYER_STATE NextState) { m_iState = NextState; }

private:
    HRESULT Ready_PartObjects();
    void Input_Key(const _float& fTimeDelta);

public:
    static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
