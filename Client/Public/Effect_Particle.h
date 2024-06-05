#pragma once
#include "EffectObject.h"

#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Instance_Point;
END

BEGIN(Client)
class CEffect_Particle :
    public CEffectObject
{
public:
    enum PARTICLE_TYPE { PARTICLE_DROP, PARTICLE_SPREAT, PARTICLE_FOUNTAIN, PARTICLE_END };
    typedef struct tEffectParticleDesc : public CEffectObject::EFFECT_OBJECT_DESC {
        _uint iNumInstance;
        _uint iParticleType;
        _float3 vOffsetPos;
        _float3 vPivotPos;
        _float3 vRange;
        _float2 vSize;
        _float2 vSpeed;
        _float2 vPower;
        _float2 vLifeTime;
        _bool isLoop;
    } EFFECT_PARTICLE_DESC;

private:
    CEffect_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffect_Particle(const CEffect_Particle& rhs);
    virtual ~CEffect_Particle() = default;

public:
    virtual HRESULT Initialize_Prototype(void* pArg);
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    class CVIBuffer_Instance_Point* m_pVIBufferCom = { nullptr };
    class CTexture* m_pTextureCom = { nullptr };

    _uint m_iParticleType = { PARTICLE_END };

    EFFECT_PARTICLE_DESC m_Desc{};

private:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();

public:
    static CEffect_Particle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
    virtual CGameObject* Clone(void* pArg);
    virtual void Free();
};

END