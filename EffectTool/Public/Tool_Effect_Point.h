#pragma once
#include "Tool_Effect.h"

BEGIN(Engine)
class CVIBuffer_Instance_Point;
class CVIBuffer_Instance;
END

BEGIN(Effect)
class CTool_Effect_Point :
    public CTool_Effect
{
public:
    enum TYPE { DROP, SPREAD, FOUNTAIN, TYPE_END };

public:
    typedef struct tEffectPoint : public CTool_Effect::TOOL_EFFECT_DESC {
        _int iParticleType;
        _uint iNumInstance;
        _float3 vOffsetPos;
        _float3 vPivotPos;
        _float3 vRange;
        _float2 vSize;
        _float2 vSpeed;
        _float2 vPower;
        _float2 vLifeTime;
        _bool isLoop;
    } EFFECT_POINT_DESC;

private:
    CTool_Effect_Point(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CTool_Effect_Point(const CTool_Effect_Point& rhs);
    virtual ~CTool_Effect_Point() = default;

public:
    EFFECT_POINT_DESC Get_ParticleDesc() { return m_EffectDesc; }
    wstring Get_FilePath() { return m_strTextureFilePath; }

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

private:
    wstring m_strTextureFilePath = { L"" };
    CVIBuffer_Instance_Point* m_pVIBufferCom = { nullptr };
    _int m_ParticleType = { TYPE_END };

    EFFECT_POINT_DESC m_EffectDesc;

private:
    virtual HRESULT Add_Components();
    virtual HRESULT Bind_ResourceData();

public:
    static CTool_Effect_Point* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CGameObject* Clone(void* pArg) override;
    virtual void Free() override;
};
END

