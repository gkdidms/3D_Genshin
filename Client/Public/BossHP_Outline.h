#pragma once
#include "UI.h"

BEGIN(Client)
class CBossHP_Outline :
    public CUI
{
private:
    CBossHP_Outline(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CBossHP_Outline(const CBossHP_Outline& rhs);
    virtual ~CBossHP_Outline() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    class CBossHP* m_pBossHP = { nullptr };

private:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();

public:
    static CBossHP_Outline* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free();
};
END
