#pragma once
#include "Bullet.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Trail;
END

BEGIN(Client)
class CFeiyan_Normal_Trail :
    public CBullet
{
public:
    typedef struct tFeiyanNormalTrailDesc {
        const _float4x4* pParentMatrix;
    } FEIYAN_NORMAL_TRAIL_DESC;

private:
    CFeiyan_Normal_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CFeiyan_Normal_Trail(const CFeiyan_Normal_Trail& rhs);
    virtual ~CFeiyan_Normal_Trail() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    CShader* m_pShaderCom = { nullptr };
    CTexture* m_pTextureCom = { nullptr };
    CVIBuffer_Trail* m_pVIBufferCom = { nullptr };

private:
    const _float4x4* m_pParentMatrix = { nullptr };

private:
    HRESULT Add_Components();
    HRESULT Bind_ResourceData();

public:
    static CFeiyan_Normal_Trail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free() override;
};
END
