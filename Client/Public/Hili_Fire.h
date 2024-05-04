#pragma once
#include "Hili.h"

BEGIN(Client)
class CHili_Fire :
    public CHili
{
private:
    CHili_Fire(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CHili_Fire(const CHili_Fire& rhs);
    virtual ~CHili_Fire() = default;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(void* pArg) override;
    void Priority_Tick(const _float& fTimeDelta) override;
    void Tick(const _float& fTimeDelta) override;
    void Late_Tick(const _float& fTimeDelta) override;
    HRESULT Render() override;

private:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();
    virtual void Change_Animation(const _float& fTimeDelta);

private:
    class CBT_Hili* m_pBT = { nullptr };

public:
    static CHili_Fire* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free();
};
END
