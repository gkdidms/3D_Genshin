#pragma once
#include "Slime_Large.h"

BEGIN(Client)
class CSlime_Mid_FIre final:
    public CSlime_Large
{
private:
    CSlime_Mid_FIre(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CSlime_Mid_FIre(const CSlime_Mid_FIre& rhs);
    virtual ~CSlime_Mid_FIre() = default;

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

public:
    static CSlime_Mid_FIre* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free();
};
END
