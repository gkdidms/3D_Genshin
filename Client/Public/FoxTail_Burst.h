#pragma once
#include "SkillObj.h"

BEGIN(Client)
class CFoxTail_Burst :
    public CSkillObj
{
private:
    CFoxTail_Burst(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CFoxTail_Burst(const CFoxTail_Burst& rhs);
    virtual ~CFoxTail_Burst() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Tick(const _float& fTimeDelta);
    virtual void Tick(const _float& fTimeDelta);
    virtual void Late_Tick(const _float& fTimeDelta);
    virtual HRESULT Render();

private:
    HRESULT Add_Components() override;
    HRESULT Bind_ResourceData() override;
    void Change_Animation(const _float& fTimeDelta) override;

public:
    static CFoxTail_Burst* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
