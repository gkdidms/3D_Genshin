#pragma once
#include "GameObject.h"

#include "Client_Defines.h"

BEGIN(Engine)
class CNavigation;
class CCollider;
END

BEGIN(Client)
class CPlayer :
    public CGameObject
{
public:
    typedef struct tPlayerDesc : public CGameObject::GAMEOBJECT_DESC {
        _float3 vPlayerPos;
        _int iPlayerNavigationIndex;
    } PLAYER_DESC;

public:
    enum PLAYERBLE_TYPE { PLAYER_TIGHNARI, PLAYER_NILOU, PLAYER_WANDERER, PLAYER_YAE, PLAYER_TYPE_END };
    enum PLAYER_PART { PART_BODY, PART_WEAPON, PART_SKILOBJ, PART_END };
    enum PLAYER_DIR { DIR_RIGHT_SIDE, DIR_LEFT_SIDE, DIR_STRIGHT, DIR_BACKWORK, DIR_END };

private:
    CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CPlayer(const CPlayer& rhs);
    virtual ~CPlayer() = default;

public:
    const _float4x4* Get_PlayerCameraLook() const { return m_pCameraLook; }

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
    PLAYERBLE_TYPE m_CurrentPlayerble = { PLAYER_NILOU };

    CNavigation* m_pNavigationCom = { nullptr };
    CCollider* m_pColliderCom = { nullptr };

private:
    _uint m_iState = { 0 };
    _uint m_iDirState = { 0 };
    _bool m_IsElementalAir = { false };
    _bool m_IsFly = { false };

private:
    _int m_iPlayerNavigationIndex = { 0 };
    _float3 m_vPlayerPos = {};

private:
    const _float4x4* m_pCameraLook = {};

private:
    HRESULT Ready_Bodys();
    HRESULT Ready_Weapons();
    HRESULT Ready_SkillObjs();
    void Input_Key(const _float& fTimeDelta);
    void Change_Playerble();

private:
    HRESULT Add_Components();
    void SetUp_OnTerrain(); // 높이 조정 

public:
    static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
