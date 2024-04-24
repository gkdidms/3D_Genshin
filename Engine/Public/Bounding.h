#pragma once
#include "Base.h"
#include "Collider.h"

BEGIN(Engine)
class CBounding :
    public CBase
{
public:
    typedef struct tBoundingDesc : public CCollider::COLLIDER_DESC {
        _float3 vCenter;
    }BOUNDING_DESC;

protected:
    CBounding();
    virtual ~CBounding();

public:
    virtual HRESULT Initialize(const void* pArg) = 0;
    virtual void Tick(_fmatrix WorldMatrix);

#ifdef _DEBUG
public:
    virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch);
#endif // _DEBUG

protected:
    _float3 m_vCenter;

public:
    virtual void Free() override;
    
};

END