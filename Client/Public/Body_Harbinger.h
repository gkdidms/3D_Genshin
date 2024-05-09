#pragma once
#include "Boss_Body.h"

BEGIN(Client)
class CBody_Harbinger :
    public CBoss_Body
{
private:
    CBody_Harbinger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CBody_Harbinger(const CBody_Harbinger& rhs);
    virtual ~CBody_Harbinger() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    virtual HRESULT Add_Components() override;
    virtual HRESULT Bind_ResourceData() override;
    virtual void Change_Animation() override;

public:
    static CBody_Harbinger* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
