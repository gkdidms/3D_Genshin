#pragma once
#include "SkillObj.h"

/*
지속시간 : 14초
재사용시간 : 4초

랜덤 타겟팅으로 주변에 있는 적들을 자동으로 사냥한다. 
*/

BEGIN(Client)
class CFoxTower :
    public CSkillObj
{
private:
    CFoxTower(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CFoxTower(const CFoxTower& rhs);
    virtual ~CFoxTower() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Tick(const _float& fTimeDelta);
    virtual void Tick(const _float& fTimeDelta);
    virtual void Late_Tick(const _float& fTimeDelta);
    virtual HRESULT Render();

private:
    _float m_fDuration = { 14.f };
    _float m_fCurrentTime = { 0.f };

private:
    HRESULT Add_Components() override;
    HRESULT Bind_ResourceData() override;
    void Change_Animation(const _float& fTimeDelta) override;

public:
    static CFoxTower* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
