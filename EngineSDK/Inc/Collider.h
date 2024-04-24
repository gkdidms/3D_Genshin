#pragma once
#include "Component.h"

BEGIN(Engine)
class ENGINE_DLL CCollider :
    public CComponent
{
public:
    enum TYPE { COLLIDER_AABB, COLLIDER_OBB, COLLIDER_SPHERE, COLLIDER_END };

    typedef struct tColliderDesc {
        TYPE eType;
    }COLLIDER_DESC;

private:
    CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CCollider(const CCollider& rhs);
    virtual ~CCollider() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void* pArg);
    virtual void Tick(_fmatrix WorldMatirx);

#ifdef _DEBUG
public:
    HRESULT Render();
#endif // _DEBUG


private:
    TYPE m_ColliderType = { COLLIDER_END };

    class CBounding* m_pCurrentBounding = { nullptr };

#ifdef _DEBUG
private:
    PrimitiveBatch<VertexPositionColor>* m_pBatch = { nullptr };
    BasicEffect* m_pShader = { nullptr };
    ID3D11InputLayout* m_pInputLayout = { nullptr };
#endif

public:
    static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    virtual CComponent* Clone(void* pArg) override;
    virtual void Free() override;
};
END
