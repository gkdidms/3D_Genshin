#pragma once
#include "Monster.h"

BEGIN(Client)
class CBrute :
    public CMonster
{
private:
    CBrute(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CBrute(const CBrute& rhs);
    virtual ~CBrute() = default;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(void* pArg) override;
    void Priority_Tick(const _float& fTimeDelta) override;
    void Tick(const _float& fTimeDelta) override;
    void Late_Tick(const _float& fTimeDelta) override;
    HRESULT Render() override;

protected:
    virtual HRESULT Add_Components() override;
    virtual HRESULT Bind_ResourceData() override;
    virtual void Change_Animation(const _float& fTimeDelta) override;

public:
    virtual void Free() override;
};
END
