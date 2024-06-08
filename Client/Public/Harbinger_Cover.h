#pragma once
#include "Monster_Bullet.h"

BEGIN(Engine)
class CVIBuffer_Trail;
END

BEGIN(Client)
class CHarbinger_Cover :
    public CMonster_Bullet
{
public:
    typedef struct tHarbingerCoverDesc : public CMonster_Bullet::BULLET_DESC {
        _vector vPos;
    } BULLET_COVER_DESC;

private:
    CHarbinger_Cover(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CHarbinger_Cover(const CHarbinger_Cover& rhs);
    virtual ~CHarbinger_Cover() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Tick(const _float& fTimeDelta);
    virtual void Tick(const _float& fTimeDelta);
    virtual void Late_Tick(const _float& fTimeDelta);
    virtual HRESULT Render();

public:
    CShader* m_pShaderCom = { nullptr };
    CVIBuffer_Trail* m_pTrailVIBufferCom = { nullptr };
    CTexture* m_pTextureCom = { nullptr };

    _float m_fHeight = { 0.f }; // 플레이어 y값 저장

private:
    HRESULT Add_Componenets();
    HRESULT Bind_ResourceData();

public:
    static CHarbinger_Cover* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free();
};
END
