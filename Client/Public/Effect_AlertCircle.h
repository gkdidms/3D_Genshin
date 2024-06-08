#pragma once
#include "Effect.h"

BEGIN(Client)
class CEffect_AlertCircle :
    public CEffect
{
private:
    CEffect_AlertCircle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_AlertCircle(const CEffect_AlertCircle& rhs);
    virtual ~CEffect_AlertCircle() = default;

public:
    virtual HRESULT Initialize_Prototype(const _char* szFileName);
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    CCollider* m_pColliderCom = { nullptr };

    _float m_fTurnDuration = { 0.4f };
    _float m_fTurnCurrentTime = { 0.f };
    _bool m_isCreateBullet = { true };

private:
    HRESULT Add_Components();
    HRESULT File_Open(const _char* szFileName);

public:
    static CEffect_AlertCircle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* szFilePath);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free();
};
END
