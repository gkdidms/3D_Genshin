#pragma once
#include "PartObject.h"

BEGIN(Client)
class CFlycloak :
    public CPartObject
{
public:
    typedef struct tFlycloakDesc : public CPartObject::PART_DESC {
        const _float4x4* pSocketCombinedTransformationMatrix;
    }FLYCLOAK_DESC;

private:
    CFlycloak(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CFlycloak(const CFlycloak& rhs);
    virtual ~CFlycloak() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Tick(const _float& fTimeDelta);
    virtual void Tick(const _float& fTimeDelta);
    virtual void Late_Tick(const _float& fTimeDelta);
    virtual HRESULT Render();

private:
    const _float4x4* m_pSocketMatrix = { nullptr };

private:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();
    virtual void Change_Animation(const _float& fTimeDelta);

public:
    static CFlycloak* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
