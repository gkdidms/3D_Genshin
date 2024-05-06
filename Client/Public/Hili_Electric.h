#pragma once
#include "Hili.h"

BEGIN(Client)
class CHili_Electric :
    public CHili
{
private:
    CHili_Electric(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CHili_Electric(const CHili_Electric& rhs);
    virtual ~CHili_Electric() = default;

public:
    HRESULT Initialize_Prototype() override;
    HRESULT Initialize(void* pArg) override;
    void Priority_Tick(const _float& fTimeDelta) override;
    void Tick(const _float& fTimeDelta) override;
    void Late_Tick(const _float& fTimeDelta) override;
    HRESULT Render() override;

protected:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();
    virtual void Change_Animation(const _float& fTimeDelta);

public:
    static CHili_Electric* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free();
};
END
