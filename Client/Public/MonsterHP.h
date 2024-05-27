#pragma once
#include "UI.h"

BEGIN(Client)
class CMonsterHP :
    public CUI
{
public:
    typedef struct MyStruct : public CGameObject::GAMEOBJECT_DESC
    {
        const _float4x4* pParentMatrix;
        _float fMonsterMaxHP;
        const _float* pMonsterHP;
    } MONSTER_HP_DESC;

private:
    CMonsterHP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual ~CMonsterHP() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    const _float4x4* m_pParentMatrix = { nullptr };
    _float m_fMonsterMaxHP = { 0.f };
    const _float* m_pPlayerHP = { nullptr };

    _float4x4 m_WorldMatrix;

private:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();

public:
    static CMonsterHP* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
