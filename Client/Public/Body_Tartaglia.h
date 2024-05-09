#pragma once

#include "Boss_Body.h"

BEGIN(Engine)
class CShader;
class CModel;
END

BEGIN(Client)
class CBody_Tartaglia : // ¹°¿ø¼Ò
    public CBoss_Body
{
private:
    CBody_Tartaglia(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CBody_Tartaglia(const CBody_Tartaglia& rhs);
    virtual ~CBody_Tartaglia() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    _float m_fTime = { 1.5f };
    _float m_fCurrentTime = { 0.f };

private:
    _float m_fLongDistanceSpeed = { 50.f };

private:
    virtual HRESULT Add_Components() override;
    virtual HRESULT Bind_ResourceData() override;
    virtual void Change_Animation() override;
    
public:
    static CBody_Tartaglia* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};

END