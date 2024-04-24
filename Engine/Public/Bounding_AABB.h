#pragma once
#include "Bounding.h"

BEGIN(Engine)
class CBounding_AABB :
    public CBounding
{
public:
    typedef struct tAABBDesc : public CBounding::BOUNDING_DESC {
        _float3 vExtents;
    } BOUNDING_AABB_DESC;
private:
    CBounding_AABB();
    virtual ~CBounding_AABB() = default;

public:
    virtual HRESULT Initialize(const void* pArg);
    virtual void Tick(_fmatrix WorldMatrix) override;

#ifdef _DEBUG
    virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch) override;
#endif // _DEBUG

private:
    BoundingBox* m_pOriginalBox = { nullptr };
    BoundingBox* m_pBoundingBox = { nullptr };

    _float3 m_vExtents = {};

public:
    static CBounding_AABB* Create(const void* pArg);
    virtual void Free() override;
};
END
