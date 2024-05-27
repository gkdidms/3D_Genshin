#pragma once
#include "UI.h"

BEGIN(Client)
class CSkillBtn_Q :
    public CUI
{
private:
    CSkillBtn_Q(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CSkillBtn_Q(const CSkillBtn_Q& rhs);
    virtual ~CSkillBtn_Q() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    class CSkillBtn_Icon* m_pBtnIcon[4] = { nullptr };
    CGameObject* m_pBtnFrame = { nullptr };
    _uint m_iCurrentPlayerble = { 0 };

    class CStateManager* m_pStateManager = { nullptr };

private:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();
    HRESULT Ready_Icon();

public:
    static CSkillBtn_Q* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};

END
