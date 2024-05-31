#pragma once
#include "BlendObject.h"

#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Instance_Point;
END

BEGIN(Client)
class CEffect_Particle_Water :
    public CBlendObject
{
public:
    typedef struct tParticleWaterDesc {
        _matrix ParentMatrix;
    } PARTICLE_WATER_DESC;
private:
    CEffect_Particle_Water(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_Particle_Water(const CEffect_Particle_Water& rhs);
    virtual ~CEffect_Particle_Water() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    class CShader* m_pShaderCom = { nullptr };
    class CVIBuffer_Instance_Point* m_pVIBufferCom = { nullptr };
    class CTexture* m_pTextureCom = { nullptr };

    _float m_fFrame = { 0.f };

private:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();

public:
    static CEffect_Particle_Water* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContextg);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free();
};

END