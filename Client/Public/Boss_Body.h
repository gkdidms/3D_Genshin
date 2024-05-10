#pragma once
#include "PartObject.h"

BEGIN(Client)
class CBoss_Body :
    public CPartObject
{
public:
    typedef struct tTartagliaDesc : public CPartObject::PART_DESC {
        const _float4x4* pTargetMatrix;
    }TARTAGLIA_DESC;

    typedef struct tTartagliaInfo {
        _float fMaxHp = { 146125.f };
        _float fHp = { 146125.f };
        _float fMaxDefendHp;
        _float fDefendHp;
        _bool isDefend;
    }TARTAGLIA_INFO;

public:
    TARTAGLIA_INFO* Get_Info() { return &m_Info; }

protected:
    CBoss_Body(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CBoss_Body(const CBoss_Body& rhs);
    virtual ~CBoss_Body() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

protected:
    const _float4x4* m_pTargetMatrix = { nullptr };
    TARTAGLIA_INFO m_Info;

protected:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();
    virtual void Change_Animation();


public:
    virtual void Free() override;
};

END