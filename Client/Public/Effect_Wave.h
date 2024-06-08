#pragma once
#include "Effect.h"

BEGIN(Client)
class CEffect_Wave :
    public CEffect
{
private:
    CEffect_Wave(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_Wave(const CEffect_Wave& rhs);
    virtual ~CEffect_Wave() = default;

public:
    virtual HRESULT Initialize_Prototype(const _char* szFileName);
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    CCollider* m_pColliderCom = { nullptr };

    _float m_fDelayDuration = { 0.2f };
    _float m_fDelayCurrentTime = { 0.f };

private:
    HRESULT Add_Components();
    HRESULT File_Open(const _char* szFileName);

public:
    static CEffect_Wave* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* szFilePath);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free();
};
END
