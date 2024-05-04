#pragma once
#include "Slime_Large.h"

BEGIN(Client)
class CSlime_Large_Water final:
    public CSlime_Large
{
private:
    CSlime_Large_Water(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CSlime_Large_Water(const CSlime_Large_Water& rhs);
    virtual ~CSlime_Large_Water() = default;

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

public:
    static CSlime_Large_Water* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free();
};
END
