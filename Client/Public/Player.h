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
    enum PLAYER_DIR { DIR_RIGHT_SIDE, DIR_LEFT_SIDE, DIR_STRIGHT, DIR_BACKWORK, DIR_END };

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
    class CState_Manager* m_pState_Manager = { nullptr };
    vector<class CGameObject*> m_PartObject[PLAYER_TYPE_END];
    PLAYERBLE_TYPE m_CurrentPlayerble = { PLAYER_WANDERER };

private:
    _uint m_iState = { 0 };
    _uint m_iDirState = { 0 };
    _bool m_IsElementalAir = { false };

private:
    HRESULT Ready_PartObjects();
    void Input_Key(const _float& fTimeDelta);
    void Change_Playerble();

public:
    static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
