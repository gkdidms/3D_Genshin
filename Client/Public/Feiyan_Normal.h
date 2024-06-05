#pragma once
#include "Player_Bullet.h"


BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_RectZ;
class CVIBuffer_Trail;
class CCollider;
END

BEGIN(Client)
class CFeiyan_Normal :
    public CPlayer_Bullet
{
private:
    CFeiyan_Normal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CFeiyan_Normal(const CFeiyan_Normal& rhs);
    virtual ~CFeiyan_Normal() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    CShader* m_pShaderCom = { nullptr };
    CTexture* m_pTextureCom = {nullptr};
    CVIBuffer_RectZ* m_pVIBufferCom = { nullptr };

    CGameObject* m_pTrailBuffer = { nullptr };
    CGameObject* m_FireParticle = { nullptr };
    
    const _float4x4* m_pParentMatrix = { };

private:
    HRESULT Add_Components();
    HRESULT Bind_ResourceData();

public:
    static CFeiyan_Normal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};

END