#pragma once
#include "BlendObject.h"

#include "Client_Defines.h"

BEGIN(Client)
class CEffectObject abstract :
    public CBlendObject
{
public:
    typedef struct tEffectObjectDesc {
        const _float4x4* pParentMatrix;
        _float4x4 WorldMatrix;

        _float4x4 RotationMatrix;
        _float3 vScale;
        _float4 vPos;
        _uint iShaderPass;
        _bool isTrailMove = { false };
        _float fSpeed = { 0.f };

        _uint iMoveType;
        _float fDuration;
    } EFFECT_OBJECT_DESC;

protected:
    CEffectObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CEffectObject(const CEffectObject& rhs);
    virtual ~CEffectObject() = default;

public:
    virtual HRESULT Initialize_Prototype(void* pArg);
    virtual HRESULT Initialize(void* pArg) override;
    virtual void Priority_Tick(const _float& fTimeDelta) override;
    virtual void Tick(const _float& fTimeDelta) override;
    virtual void Late_Tick(const _float& fTimeDelta) override;
    virtual HRESULT Render() override;

protected:
    const _float4x4* m_ParentMatrix = {nullptr};
    _float4x4 m_WorldMatrix = {};

protected:
    _uint m_iShaderPass = { 0 };
    _bool m_isTrailMove = { false };
    _float m_fSpeed = { 0.f };
    _float m_fDuration = { 0.f };

protected:
    virtual HRESULT Add_Components() = 0;
    virtual HRESULT Bind_ResourceData() = 0;

public:
    virtual void Free() override;
};
END
