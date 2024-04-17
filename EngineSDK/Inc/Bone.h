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
    const _float4x4* Get_CombinedTransformationMatrix() const {
        return &m_CombinedTransformMatrix;
    }
    _vector Get_Scale() { return XMLoadFloat3(&m_vScale); }
    _vector Get_Rotation() { return XMLoadFloat4(&m_vRotation); }
    _vector Get_Translation() { return XMLoadFloat3(&m_vTranslation); }

public:
    void Set_TranformationMatrix(_fmatrix TransformationMatrix) { XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix); }
    void Set_StateVector(_fvector vScale, _fvector vRotation, _fvector vTranslation) {
        XMStoreFloat3(&m_vScale, vScale);
        XMStoreFloat4(&m_vRotation, vRotation);
        XMStoreFloat3(&m_vTranslation, vTranslation);
    }

public:
    HRESULT Initialize(const aiNode* pNode, _int iParentIndex);
    HRESULT Initialize(const char* pName, _int iParentIndex, _fmatrix TransformationMatrix);
    void Update_CombinedTransformMatrix(const vector<CBone*>& Bones, _fmatrix PreTransformMatrix);

private:
    char m_szNodeName[MAX_PATH] = "";

    _int m_iParentIndex = { 0 };
    _float4x4 m_TransformationMatrix;
    _float4x4 m_CombinedTransformMatrix;

    _float3 m_vScale;
    _float4 m_vRotation;
    _float3 m_vTranslation;

public:
    static CBone* Create(const aiNode* pNode, _int iParentIndex);
    static CBone* Create(const char* pName, _int iParentIndex, _fmatrix TransformationMatrix);
    CBone* Clone();
    virtual void Free();
};
END
