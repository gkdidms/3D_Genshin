#pragma once
#include "Component.h"
#include "Mesh.h"

BEGIN(Engine)
class ENGINE_DLL CModel final:
    public CComponent
{
private:
    CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CModel(const CModel& rhs);
    virtual ~CModel() = default;

public:
    _uint Get_NumMeshes() { return m_iNumMeshes; }
    _uint Get_NumMaterials() { return m_iNumMaterials; }

public:
    HRESULT Initialize_Prototype(CMesh::MESHTYPE eMeshType, const char* szModelFilePath, _fmatrix PreTransformMatrix);
    HRESULT Initialize(void* pArv) override;
    void Render(_uint iMeshIndex);

    HRESULT Bind_Material(class CShader* pShader, const char* strConstansName, _uint iMeshIndex, aiTextureType iTextureType);
    HRESULT Bind_BoneMatrices(class CShader* pShader, const char* strConstansName, _uint iMeshIndex);

public:
    void Play_Animation(const _float& fTimeDelta);
    void Set_Animation(_uint iCurrentIndex) {
        m_iCurrentAnimIndex = iCurrentIndex;
    }

private:
    const aiScene* m_pAIScene = { nullptr };
    Assimp::Importer m_Importer;
    _float4x4 m_PreTransformMatrix;

private:
    _uint m_iNumMeshes = { 0 };
    vector<class CMesh*> m_Meshes;

private:
    _uint m_iNumMaterials = { 0 };
    vector<MESH_MATERIAL> m_Materials;

private:
    _uint m_iNumBones = { 0 };
    vector<class CBone*> m_Bones;

    _float4x4 m_MeshBoneMatrices[512];

private:
    _uint m_iNumAnimations = { 0 };
    vector<class CAnimation*> m_Animations;
    _uint m_iCurrentAnimIndex = { 0 };

private:
    HRESULT Ready_Meshes(CMesh::MESHTYPE eMeshType);
    HRESULT Ready_Materials(const char* pModelFilePath);
    HRESULT Ready_Bones(const aiNode* pAINode, _int iCountBoneIndex);
    HRESULT Ready_Animation();

public:
    static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CMesh::MESHTYPE eMeshType, const char* szModelFilePath, _fmatrix PreTransformMatrix);
    virtual CComponent* Clone(void* vArg) override;
    virtual void Free() override;
};
END
