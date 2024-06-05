#pragma once
#include "SkillObj.h"

BEGIN(Client)
class CFlowerArrow final :
    public CSkillObj
{
private:
    CFlowerArrow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CFlowerArrow(const CFlowerArrow& rhs);
    virtual ~CFlowerArrow() = default;
    
public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Tick(const _float& fTimeDelta);
    virtual void Tick(const _float& fTimeDelta);
    virtual void Late_Tick(const _float& fTimeDelta);
    virtual HRESULT Render();

private:
    CTexture* m_pTextureCom = { nullptr };

private:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();
    virtual void Change_Animation(const _float& fTimeDelta);

public:
    static CFlowerArrow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
