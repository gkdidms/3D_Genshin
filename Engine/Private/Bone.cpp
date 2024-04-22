#include "Bone.h"

CBone::CBone()
{
}

HRESULT CBone::Initialize(const char* pName, _int iParentIndex, _fmatrix TransformationMatrix)
{
    strcpy_s(m_szNodeName, pName);

    m_iParentIndex = iParentIndex;

    XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);

    XMStoreFloat4x4(&m_CombinedTransformMatrix, XMMatrixIdentity());

    XMStoreFloat3(&m_vScale, XMVectorSet(1.f, 1.f, 1.f, 0.f));
    XMStoreFloat4(&m_vRotation, XMVectorSet(0.f, 0.f, 0.f, 1.f));
    XMStoreFloat3(&m_vTranslation, XMVectorSet(0.f, 0.f, 0.f, 1.f));

    return S_OK;
}

void CBone::Update_CombinedTransformMatrix(const vector<CBone*>& Bones, _fmatrix PreTransformMatrix)
{
    if (-1 == m_iParentIndex)
        XMStoreFloat4x4(&m_CombinedTransformMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * PreTransformMatrix);
    else
        XMStoreFloat4x4(&m_CombinedTransformMatrix, XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(Bones[m_iParentIndex]->Get_CombinedTransformationMatrix()));
}

CBone* CBone::Create(const char* pName, _int iParentIndex, _fmatrix TransformationMatrix)
{
    CBone* pInstance = new CBone();

    if (FAILED(pInstance->Initialize(pName, iParentIndex, TransformationMatrix)))
        Safe_Release(pInstance);

    return pInstance;
}

CBone* CBone::Clone()
{
    return new CBone(*this);
}

void CBone::Free()
{
}
