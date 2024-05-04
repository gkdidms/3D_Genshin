#pragma once
#include "Bounding.h"

BEGIN(Engine)
class CBounding_OBB :
    public CBounding
{
public:
    typedef struct tOBBDesc : public CBounding::BOUNDING_DESC {
        _float3 vExtents;
        _float3 vRotation;
    }BOUNDING_OBB_DESC;

private:
    CBounding_OBB();
    virtual ~CBounding_OBB() = default;

public:
    virtual void* Get_Desc() { return m_pBoundingBox; }

public:
    virtual HRESULT Initialize(const void* pArg);
    virtual void Tick(_fmatrix WorldMatrix) override;
    virtual _bool Intersect(CCollider::TYPE eTargetType, CBounding* pTargetBounding) override;

#ifdef _DEBUG
    virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch) override;
#endif // _DEBUG

private:
    BoundingOrientedBox* m_pOriginalBox = { nullptr };
    BoundingOrientedBox* m_pBoundingBox = { nullptr };

    _float3 m_vExtents = {};

public:
    static CBounding_OBB* Create(const void* pArg);
    virtual void Free();
};

END