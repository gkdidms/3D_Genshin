#pragma once
#include "Slime.h"

BEGIN(Client)
class CSlime_Large:
    public CSlime
{
public:
    enum SLIME_LARGE_STATE {
        SLIME_LARGE_IDLE,
        SLIME_LARGE_JUMP,
        SLIME_LARGE_JUMP_ATK,
        SLIME_LARGE_ATTACK_1,
        SLIME_LARGE_ATTACK_2,
        SLIME_LARGE_TURN,
        SLIME_LARGE_HIT_H,
        SLIME_LARGE_HIT_L,
        SLIME_LARGE_DIE,
        SLIME_END
    };

protected:
    CSlime_Large(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CSlime_Large(const CSlime_Large& rhs);
    virtual ~CSlime_Large() = default;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(void* pArg) override;
    void Priority_Tick(const _float& fTimeDelta) override;
    void Tick(const _float& fTimeDelta) override;
    void Late_Tick(const _float& fTimeDelta) override;
    HRESULT Render() override;

protected:
    SLIME_LARGE_STATE m_eCurrentState = { SLIME_END };
 
    class CSlime_Root* m_pBT = { nullptr };

protected:
    virtual HRESULT Add_Components() { return S_OK; }
    virtual HRESULT Bind_ResourceData();
    virtual void Change_Animation(const _float& fTimeDelta);

public:
    virtual void Free();

};
END
