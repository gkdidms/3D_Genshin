#pragma once
#include "Monster_Bullet.h"

BEGIN(Engine)
class CVIBuffer_Trail;
class CVIBuffer_RectZ;
END

BEGIN(Client)
class CBoss_Range_Bullet :
    public CMonster_Bullet
{
private:
    CBoss_Range_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CBoss_Range_Bullet(const CBoss_Range_Bullet& rhs);
    virtual ~CBoss_Range_Bullet() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Tick(const _float& fTimeDelta);
    virtual void Tick(const _float& fTimeDelta);
    virtual void Late_Tick(const _float& fTimeDelta);
    virtual HRESULT Render();

private:
    CShader* m_pShaderCom = { nullptr };
    CTexture* m_pTextureCom[2] = { nullptr };
    CVIBuffer_RectZ* m_pVIBufferCom = { nullptr };
    CVIBuffer_Trail* m_pTrailVIBufferCom = { nullptr };

private:
    _float m_fAccelTime = { 0.f };
    _float m_fPower = { 7.f };
    _float m_fHeight = { 0.f }; // 플레이어 y값 저장

    _bool m_isUp = { false };

private:
    HRESULT Add_Components();
    HRESULT Bind_ResourceData();
    void Move_Up(const _float fTimeDelta);

public:
    static CBoss_Range_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free();
};
END
