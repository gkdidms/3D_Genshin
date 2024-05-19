#pragma once
#include "UI.h"

BEGIN(Client)
class CPlayerHP_Outline :
    public CUI
{
private:
    CPlayerHP_Outline(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CPlayerHP_Outline(const CPlayerHP_Outline& rhs);
    virtual ~CPlayerHP_Outline() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    class CPlayerHP* m_pMonsterHP = { nullptr };

private:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();

public:
    static CPlayerHP_Outline* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free();
};
END
