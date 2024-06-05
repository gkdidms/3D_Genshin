#pragma once
#include "UI.h"

BEGIN(Client)
class CBossHP :
    public CUI
{
public:
    typedef struct MyStruct : public CGameObject::GAMEOBJECT_DESC
    {
        _float4x4 pParentMatrix;
    } BOSS_HP_DESC;

private:
    CBossHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CBossHP() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    _float4x4 m_pParentMatrix = { };

    _float4x4 m_WorldMatrix;

private:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();

public:
    static CBossHP* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
