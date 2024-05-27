#pragma once
#include "GameObject.h"

BEGIN(Engine)
class ENGINE_DLL CBlendObject abstract :
    public CGameObject
{
protected:
    CBlendObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CBlendObject(const CBlendObject& rhs);
    virtual ~CBlendObject() = default;

public:
    _float Get_ViewZ() const { return m_fViewZ; }

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Priority_Tick(const _float& fTimeDelta);
    virtual void Tick(const _float& fTimeDelta);
    virtual void Late_Tick(const _float& fTimeDelta);
    virtual HRESULT Render();

private:
    _float m_fViewZ = { 0.f };

protected:
    _float Compute_ViewZ(_fvector vWorldPos);

public:
    virtual void Free() override;
};
END
