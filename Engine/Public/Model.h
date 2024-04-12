#pragma once
#include "Component.h"
#include "Mesh.h"
#include "Animation.h"
#include "Bone.h"

BEGIN(Engine)
class ENGINE_DLL CModel final:
    public CComponent
{
public:
    typedef struct tAnimDesc{
        tAnimDesc(_uint iCurrentAnimIndex, _bool isLoop)
            : iCurrentAnimIndex{ iCurrentAnimIndex }, isLoop{ isLoop } {}

        _uint iCurrentAnimIndex = { 0 };
        _bool isLoop = { false };
    }ANIM_DESC;

    enum CREATETYPE { CREATE_READ, CREATE_AI };

private:
    CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
    CModel(const CModel& rhs);
    virtual ~CModel() = default;

public:
    _uint Get_NumMeshes() { return m_iNumMeshes; }
    _uint Get_NumMaterials() { return m_iNumMaterials; }

    _bool Get_Animation_Finished() const { return m_Animations[m_tAnimDesc.iCurrentAnimIndex]->IsFinished(); } //현재 재생되고 있는 애니메이션 모델이 종료 되었는지 확인하는 함수
    _bool Get_Animation_Finished(_uint iIndex) const { return m_Animations[iIndex]->IsFinished(); } // 매개변수의 인덱스 애니메이션이 종료 되었는지 확인하는 함수
    _bool Get_LoopAnimation_Finished() const { return m_Animations[m_tAnimDesc.iCurrentAnimIndex]->IsLoopFinished(); } // 현재 재생되고 있는 루프 애니메이션이 한번 루프를 돌았는지 확인하는 함수

public:
    HRESULT Initialize_Prototype(CMesh::MESHTYPE eMeshType, const _char* szModelFilePath, _fmatrix PreTransformMatrix, const _char* szBinaryFilePath, CREATETYPE eCreateType);
    HRESULT Initialize(void* pArv) override;
    void Render(_uint iMeshIndex);

    HRESULT Bind_Material(class CShader* pShader, const char* strConstansName, _uint iMeshIndex, aiTextureType iTextureType);
    HRESULT Bind_BoneMatrices(class CShader* pShader, const char* strConstansName, _uint iMeshIndex);

public:
    void Play_Animation(const _float& fTimeDelta);
    void Play_Animation(const _float& fTimeDelta, _float4* vMovePos, _bool isInterpolation = true);
    void Set_Animation(ANIM_DESC tAnimdesc) {
        if (Get_LoopAnimation_Finished())
            m_Animations[tAnimdesc.iCurrentAnimIndex]->Loop_Reset();

        if (m_tAnimDesc.iCurrentAnimIndex == tAnimdesc.iCurrentAnimIndex)
            return;

        m_Animations[tAnimdesc.iCurrentAnimIndex]->Reset();
        m_tAnimDesc = tAnimdesc;
    }

private:
    const aiScene* m_pAIScene = { nullptr };
    Assimp::Importer m_Importer;
    _float4x4 m_PreTransformMatrix;

    CMesh::MESHTYPE m_eMeshType = { CMesh::TYPE_END };

private:
    _uint m_iNumMeshes = { 0 };
    vector<class CMesh*> m_Meshes;

private:
    _uint m_iNumMaterials = { 0 };
    vector<MESH_MATERIAL> m_Materials;

private:
    _uint m_iNumBones = { 0 };
    _uint m_iRootBoneIndex = { 0 }; // "Bip001" 뼈 인덱스 저장
    vector<class CBone*> m_Bones;

    _float4x4 m_MeshBoneMatrices[512];

private:
    _uint m_iNumAnimations = { 0 };
    vector<class CAnimation*> m_Animations;
    ANIM_DESC m_tAnimDesc{ 0, false};

private:
    HRESULT Ready_Meshes();
    HRESULT Ready_Materials(const char* pModelFilePath);
    CTexture* Ready_Materials(const char* pModelFilePath, const char* pFilePath);
    HRESULT Ready_Bones(const aiNode* pAINode, _int iCountBoneIndex);
    HRESULT Ready_Animation();

    HRESULT Ready_Model(const _char* szModelFilePath, const _char* szBinaryFilePath);

public:
    static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CMesh::MESHTYPE eMeshType, const _char* szModelFilePath, _fmatrix PreTransformMatrix, const _char* szBinaryFilePath = nullptr,CREATETYPE eCreateType = CREATE_AI);
    virtual CComponent* Clone(void* vArg) override;
    virtual void Free() override;
};
END
