#pragma once
#include "PartObject.h"

BEGIN(Client)
class CPartObject_Body :
    public CPartObject
{
public:
    typedef struct tBodyDesc: public CPartObject::PART_DESC
    {
        wstring strPrototypeModelTag;
    }BODY_DESC;

private:
    CPartObject_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CPartObject_Body(const CPartObject_Body& rhs);
    virtual ~CPartObject_Body() = default;

public:
    HRESULT Initialize_Prototype();
    HRESULT Initialize(void* pArg);
    void Priority_Tick(const _float& fTimeDelta);
    void Tick(const _float& fTimeDelta);
    void Late_Tick(const _float& fTimeDelta);
    HRESULT Render();

private:
    wstring m_strPrototypeModelTag = { L"" };

private:
    HRESULT Add_Components();
    HRESULT Bind_ResourceData();
    virtual void Change_Animation(const _float& fTimeDelta) override;

public:
    static CPartObject_Body* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END
