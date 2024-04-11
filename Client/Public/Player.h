#pragma once
#include "GameObject.h"

#include "Client_Defines.h"

BEGIN(Client)
class CPlayer :
    public CGameObject
{
public:
    enum PLAYER_PART { PART_BODY, PART_WEAPON, PART_END };
    enum PLAYER_STATE { PLAYER_IDLE, PLAYER_ATTACK_IDLE, PLAYER_RUN, PLAYER_STOP, PLAYER_SPRINT, PLAYER_JUMP, PLAYER_FLY, PLAYER_ATTACK_1, PLAYER_ATTACK_2, PLAYER_ATTACK_3, PLAYER_ATTACK_4, PLAYER_ELEMENTAL_1, PLAYER_ELEMENTAL_END, PLAYER_ELENENTAL_BURST, PLAYER_ELENENTAL_BURST_END, PLAYER_END};

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
    //class CPartObject* m_PartObjects[PART_END];
    vector<class CPartObject*> m_PartObject;

    _uint m_iState = { 0 };
    _uint m_iNumMouseClick = { 0 };
    _uint m_iMaxMouseClick = { 4 };

private:
    HRESULT Ready_PartObjects();
    void Input_Key(const _float& fTimeDelta);

public:
    static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
