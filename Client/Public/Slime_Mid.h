#pragma once
#include "Slime.h"

BEGIN(Client)
class CSlime_Mid:
    public CSlime
{
public:
    enum SLIME_STATE {
        SLIME_IDLE,
        SLIME_JUMP,
        SLIME_ATTACK_1,
        SLIME_ATTACK_2,
        SLIME_HIT,
        SLIME_DIE,
        SLIME_END
    };

protected:
    CSlime_Mid(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CSlime_Mid(const CSlime_Mid& rhs);
    virtual ~CSlime_Mid() = default;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(void* pArg) override;
    void Priority_Tick(const _float& fTimeDelta) override;
    void Tick(const _float& fTimeDelta) override;
    void Late_Tick(const _float& fTimeDelta) override;
    HRESULT Render() override;

protected:
    SLIME_STATE m_eCurrentState = { SLIME_END };

protected:
    virtual HRESULT Add_Components() { return S_OK; }
    virtual HRESULT Bind_ResourceData();
    virtual void Change_Animation(const _float& fTimeDelta);

public:
    virtual void Free();

};
END
