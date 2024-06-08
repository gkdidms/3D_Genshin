#pragma once
#include "Effect.h"

BEGIN(Client)
class CEffect_Monoceros :
    public CEffect
{
private:
    CEffect_Monoceros(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_Monoceros(const CEffect_Monoceros& rhs);
    virtual ~CEffect_Monoceros() = default;

public:
    virtual HRESULT Initialize_Prototype(const _char* szFileName);
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    CCollider* m_pColliderCom = { nullptr };

private:
    _float m_fAccelTime = { 0.f };
    _float m_fPower = { 5.f };
    _float m_fHeight = { 0.f }; // 플레이어 y값 저장

private:
    HRESULT Add_Components();
    HRESULT File_Open(const _char* szFileName);

public:
    static CEffect_Monoceros* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* szFilePath);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free();
};
END
