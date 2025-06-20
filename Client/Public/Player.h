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
    enum PLAYERBLE_TYPE { PLAYER_TIGHNARI, PLAYER_NILOU, PLAYER_WANDERER, PLAYER_FEIYAN, PLAYER_TYPE_END };
    enum PLAYER_PART { PART_BODY, PART_WEAPON, PART_FLYCLOAK, PART_SKILLOBJ_1, PART_SKILLOJB_2, PART_END };
    enum PLAYER_DIR { DIR_RIGHT_SIDE, DIR_LEFT_SIDE, DIR_STRIGHT, DIR_BACKWORK, DIR_END };
    enum HILL_TYPE { HILL_UP, HILL_DOWN, HILL_END }; // 계단

private:
    CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CPlayer(const CPlayer& rhs);
    virtual ~CPlayer() = default;

public:
    const _float4x4* Get_PlayerCameraLook() const { return m_pCameraLook; }
    _uint Get_PlayerState() { return m_iState; }
    _bool Get_BossSign() { return m_isBossSign; }
    _bool isAttack();
    
public:
    void Set_Hit(_float fDamage);
    
public:// 플레이어 정보 가져오기
    _float Get_PlayerbleHP();
    _float Get_PlayerbleMaxHP();
    _float Get_PlayerbleAtk();

public:
    void Set_PlayerMove(_vector vMoveSpeed);
    void Set_Coll(_bool isColl) { m_isColl = isColl; }
    void Set_Height(_float fHeight);

public:
    _uint Get_CurrentWeapon();
    CCollider* Get_SwordCollider();
    ELEMENTAL_TYPE Get_CurrentPlayerbleElemental();


public:
    HRESULT Initialize_Prototype();
    HRESULT Initialize(void* pArg);
    void Priority_Tick(const _float& fTimeDelta);
    void Tick(const _float& fTimeDelta);
    void Late_Tick(const _float& fTimeDelta);
    HRESULT Render();

private:
    CNavigation* m_pNavigationCom = { nullptr };
    CCollider* m_pColliderCom = { nullptr };

    class CStateManager* m_pStateManager = { nullptr };
    vector<class CGameObject*> m_PartObject[PLAYER_TYPE_END];
    _uint m_CurrentPlayerble = { PLAYER_FEIYAN };

private:
    _int m_iPlayerNavigationIndex = { 0 };

private:
    _uint m_iState = { 0 }; // 현재 플레이어 상태
    _uint m_iDirState = { 0 }; // 플레이어 방향
    HILL_TYPE m_eHill = { HILL_END }; // 계단인지

private: 
    _bool m_isBossSign = { false }; // 보스 (타탈) 단류가 부착되었는지 확인.
    _bool m_isColl = { false };
    _bool m_isPlanHeight = { false }; // 발판과 충돌하여 위에 올라갓는가?

    _float m_fHeight = { 0.f };
    _float m_fJumpDurationTime = { 0.2f };
    _float m_fJumpCurrentTime = { 0.f };

    //hit 시간
    _float m_fHitDuration = { 0.3f };
    _float m_fHitCurrentTime = { 0.f };
    _bool m_isAttackPossible = { true };

private:
    const _float4x4* m_pCameraLook = {};

private:
    HRESULT Add_Components();
    void SetUp_OnTerrain(const _float& fTimeDelta); // 높이 조정 

private:
    HRESULT Ready_Bodys();
    HRESULT Ready_Weapons();
    HRESULT Ready_SkillObjs();
    HRESULT Ready_Flycloak();
    void Ready_PlayerStateRank();

private:
    void Change_Playerble();
    void Check_State(const _float& fTimeDelta);
    void Input_Key(const _float& fTimeDelta);
    void SetUp_CellType(_bool isMove);


public:
    static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
