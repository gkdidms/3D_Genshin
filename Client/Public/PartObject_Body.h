#pragma once
#include "PartObject.h"

BEGIN(Client)

class CPartObject_Body abstract:
    public CPartObject
{
public:
    typedef struct tBodyDesc : public CPartObject::PART_DESC
    {
        const _uint* pDirState;
        const _bool* pFly;
        const CPlayer::HILL_TYPE* pHill;
    }BODY_DESC; 

    typedef struct tBodyInfo
    {
        char szPlayerbleName[MAX_PATH];
        _float m_fMaxHp;
        _float m_fHp;
        _float m_fAtk;
        _float m_fDef;
        ELEMENTAL_TYPE eElementalType;
    }BODY_INFO;

protected:
    CPartObject_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CPartObject_Body(const CPartObject_Body& rhs);
    virtual ~CPartObject_Body() = default;

public:
    ELEMENTAL_TYPE Get_PlayerbleElemental() { return m_Info.eElementalType; }

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Tick(const _float& fTimeDelta);
    virtual void Tick(const _float& fTimeDelta);
    virtual void Late_Tick(const _float& fTimeDelta);
    virtual HRESULT Render();

protected:
    const _uint* m_pDirState = { nullptr };
    const _bool* m_pFly = { nullptr };
    const CPlayer::HILL_TYPE* m_pHill = { nullptr };

    BODY_INFO m_Info;

protected:
    _bool m_IsFinished = { false };


protected:
    _float m_fRunSpeed = { 5.f };
    _float m_fSprintSpeed = { 80.f };

    _float m_fFlySpeed = { 2.f };
    _float m_fFallDropSpeed = { 10.f };

protected:
    virtual HRESULT Add_Components() = 0;
    virtual HRESULT Bind_ResourceData() = 0;
    virtual void Change_Animation(const _float& fTimeDelta) {};

protected:
    void Move_Pos(const _float& fTimeDelta, _matrix* MoveMatrix);


protected:
    virtual void Free() override;
};
END
