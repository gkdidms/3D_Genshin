#pragma once
#include "Monster_Bullet.h"

BEGIN(Engine)
class CModel;
class CVIBuffer_Trail;
END

BEGIN(Client)
class CBoss_Normal :
    public CMonster_Bullet
{
private:
    CBoss_Normal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CBoss_Normal(const CBoss_Normal& rhs);
    virtual ~CBoss_Normal() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Tick(const _float& fTimeDelta);
    virtual void Tick(const _float& fTimeDelta);
    virtual void Late_Tick(const _float& fTimeDelta);
    virtual HRESULT Render();

private:
    CShader* m_pShaderCom = { nullptr };
    CShader* m_pTrailShaderCom = { nullptr };
    CTexture* m_pTextureCom[2] = { nullptr };
    CModel* m_pModelCom = { nullptr };
    CVIBuffer_Trail* m_pTrailVIBufferCom = { nullptr };

private:
    _float m_fAccelTime = { 0.f };
    _float m_fPower = { 7.f };
    _float m_fHeight = { 0.f }; // 플레이어 y값 저장

private:
    HRESULT Add_Components();
    HRESULT Bind_ResourceData();
    HRESULT Bind_TrailResourceData();
    _bool Move_Arrow(const _float& fTimeDelta);

public:
    static CBoss_Normal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free();
};
END
