#pragma once
#include "Effect.h"

BEGIN(Client)
class CHarbinger_AlertCircle :
    public CEffect
{
private:
    CHarbinger_AlertCircle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CHarbinger_AlertCircle(const CHarbinger_AlertCircle& rhs);
    virtual ~CHarbinger_AlertCircle() = default;

public:
    virtual HRESULT Initialize_Prototype(const _char* szFileName);
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    CCollider* m_pColliderCom = { nullptr };

    _float m_fTurnDuration = { 0.6f };
    _float m_fTurnCurrentTime = { 0.f };
    _bool m_isCreateBullet = { true };

    _float m_fCreateNum = { 0 };

private:
    random_device				m_RandomDevice;
    mt19937_64					m_RandomNumber;

private:
    HRESULT Add_Components();
    HRESULT File_Open(const _char* szFileName);

public:
    static CHarbinger_AlertCircle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* szFilePath);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free();
};
END
