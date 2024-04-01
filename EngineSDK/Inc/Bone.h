#pragma once
#include "Base.h"

BEGIN(Engine)
class CBone final:
    public CBase
{
private:
    CBone();
    virtual ~CBone() = default;

public:
    _bool Compare_NodeName(const char* pName) { return !strcmp(m_szNodeName, pName); }

public:
    const _float4x4* Get_CombinedTransformMatrix() const { return &m_CombinedTransformMatrix; }
    void Set_TranformationMatrix(_fmatrix TransformationMatrix) { XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix); }

public:
    HRESULT Initialize(const aiNode* pNode, _int iParentIndex);
    HRESULT Initialize(const char* pName, _int iParentIndex, _fmatrix TransformationMatrix);
    void Update_CombinedTransformMatrix(const vector<CBone*>& Bones, _fmatrix PreTransformMatrix);

private:
    char m_szNodeName[MAX_PATH] = "";

    _int m_iParentIndex = { 0 };
    _float4x4 m_TransformationMatrix;
    _float4x4 m_CombinedTransformMatrix;

public:
    static CBone* Create(const aiNode* pNode, _int iParentIndex);
    static CBone* Create(const char* pName, _int iParentIndex, _fmatrix TransformationMatrix);
    CBone* Clone();
    virtual void Free();
};
END
