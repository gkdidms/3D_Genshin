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
    _float3* Get_VtxPos() { return m_pVtxPos; }
    _uint* Get_VtxIndices() { return m_pVtxIndices; }
    _uint Get_NumFaces() { return m_iNumFaces; }

public:
    _uint Get_MaterialIndex() { return m_iMaterialIndex; }
    void Fill_Matrices(const vector<class CBone*>& Bones, _float4x4* BoneTransformMatrix);

public:
    HRESULT Initialize_Prototype(MESHTYPE eMeshType, aiMesh* pAiMesh, _fmatrix PreTransformMatrix, vector<class CBone*> Bones);
    HRESULT Initialize_Prototype(MESHTYPE eMeshType, _fmatrix PreTransformMatrix, 
        const char* pName,
        _int iMaterialIndex,
        _int iNumVertices,
        _int iNumIndices,
        _int iNumFaces,
        unsigned int* Indices,
        vector<VTXANIMMESH> AnimMeshed,
        vector<VTXMESH> Meshed,
        _int iNumBones,
        vector<_float4x4> OffsetMatrices,
        vector<_int> BoneIndices,
        _int iNumWeight,
        vector<class CBone*> Bones);

private:
    _uint m_iMaterialIndex = { 0 };

    _uint m_iNumBones = { 0 };
    _uint m_iNumFaces = { 0 };

    vector<_uint> m_BoneIndices;
    vector<_float4x4> m_OffsetMatrices;

    char m_szName[MAX_PATH] = "";

    _float3* m_pVtxPos;
    _uint* m_pVtxIndices;

private:
    HRESULT Ready_Vertices_For_NonAnim(aiMesh* pAiMesh, _fmatrix PreTransformMatrix);
    HRESULT Ready_Vertices_For_Anim(aiMesh* pAiMesh, vector<class CBone*> Bones);

    HRESULT Ready_Vertices_For_NonAnim(vector<VTXMESH> Meshed, _fmatrix PreTransformMatrix);
    HRESULT Ready_Vertices_For_Anim(vector<VTXANIMMESH> AnimMeshed, _int iNumBones, vector<_float4x4> OffsetMatrices, vector<_int> BoneIndices, _int iNumWeight, vector<class CBone*> Bones);

public:
    static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MESHTYPE eMeshType,  aiMesh* pAiMesh, _fmatrix PreTransformMatrix, vector<class CBone*> Bones);
    static CMesh* Create(
        ID3D11Device* pDevice, 
        ID3D11DeviceContext* pContext, 
        MESHTYPE eMeshType, 
        _fmatrix PreTransformMatrix, 
        const char* pName,
        _int iMaterialIndex,
        _int iNumVertices,
        _int iNumIndices,
        _int iNumFaces,
        unsigned int* Indices,
        vector<VTXANIMMESH> AnimMeshed,
        vector<VTXMESH> Meshed,
        _int iNumBones,
        vector<_float4x4> OffsetMatrices,
        vector<_int> BoneIndices,
        _int iNumWeight,
        vector<class CBone*> Bones);

    virtual CComponent* Clone(void* pArg) override;
    virtual void Free() override;
};

END