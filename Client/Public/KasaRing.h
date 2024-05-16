#pragma once
#include "SkillObj.h"

BEGIN(Client)
class CKasaRing :
    public CSkillObj
{
public:
    typedef struct tKasaRing : public CSkillObj::SKILLOBJ_DESC
    {
        const _bool* isElementalArt;
    }KASARING_DESC;
private:
    CKasaRing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CKasaRing(const CKasaRing& rhs);
    virtual ~CKasaRing() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Tick(const _float& fTimeDelta);
    virtual void Tick(const _float& fTimeDelta);
    virtual void Late_Tick(const _float& fTimeDelta);
    virtual HRESULT Render();

private:
    const _bool* m_isElementalArt = { nullptr };

private:
    HRESULT Add_Components() override;
    HRESULT Bind_ResourceData() override;
    void Change_Animation(const _float& fTimeDelta) override;

public:
    static CKasaRing* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
