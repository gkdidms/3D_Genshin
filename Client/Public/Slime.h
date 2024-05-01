#pragma once
#include "Monster.h"

BEGIN(Client)
class CSlime abstract:
    public CMonster
{
protected:
    CSlime(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CSlime(const CSlime& rhs);
    virtual ~CSlime() = default;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(void* pArg) override;
    void Priority_Tick(const _float& fTimeDelta) override;
    void Tick(const _float& fTimeDelta) override;
    void Late_Tick(const _float& fTimeDelta) override;
    HRESULT Render() override;

private:
    virtual HRESULT Add_Components() = 0;
    virtual HRESULT Bind_ResourceData() = 0;
    virtual void Change_Animation(const _float& fTimeDelta) = 0;

public:
    virtual void Free();
};

END