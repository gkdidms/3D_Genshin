#pragma once
#include "VIBuffer.h"

BEGIN(Engine)
class CMesh final :
    public CVIBuffer
{
public:
    enum MESHTYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };

private:
    CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CMesh(const CMesh& rhs);
    virtual ~CMesh() = default;

public:
    _uint Get_MaterialIndex() { return m_iMaterialIndex; }
    void Fill_Matrices(const vector<class CBone*>& Bones, _float4x4* BoneTransformMatrix);

public:
    HRESULT Initialize_Prototype(MESHTYPE eMeshType, aiMesh* pAiMesh, _fmatrix PreTransformMatrix, vector<class CBone*> Bones);

private:
    _uint m_iMaterialIndex = { 0 };

    _uint m_iNumBones = { 0 };
    vector<_uint> m_BoneIndices;
    vector<_float4x4> m_OffsetMatrices;

    char m_szName[MAX_PATH] = "";

private:
    HRESULT Ready_Vertices_For_NonAnim(aiMesh* pAiMesh, _fmatrix PreTransformMatrix);
    HRESULT Ready_Vertices_For_Anim(aiMesh* pAiMesh, vector<class CBone*> Bones);

public:
    static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MESHTYPE eMeshType,  aiMesh* pAiMesh, _fmatrix PreTransformMatrix, vector<class CBone*> Bones);
    virtual CComponent* Clone(void* pArg) override;
    virtual void Free() override;
};

END