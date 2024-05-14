#pragma once
#include "SkillObj.h"

BEGIN(Client)
class CBlackFox :
    public CSkillObj
{
private:
    CBlackFox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CBlackFox(const CBlackFox& rhs);
    virtual ~CBlackFox() = default;

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
    static CBlackFox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};

END