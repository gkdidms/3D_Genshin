#include "Bone.h"

CBone::CBone()
{
}

HRESULT CBone::Initialize(const aiNode* pNode, _int iParentIndex)
{
    // 이름 저장하기
    strcpy_s(m_szNodeName, pNode->mName.data);

    m_iParentIndex = iParentIndex;

    // 전치
    memcpy(&m_TransformMatrix, &pNode->mTransformation, sizeof(_float4x4));
    XMStoreFloat4x4(&m_TransformMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformMatrix)));

    XMStoreFloat4x4(&m_CombinedTransformMatrix ,XMMatrixIdentity());

    return S_OK;
}

void CBone::Update_CombinedTransformMatrix(const vector<CBone*>& Bones, _fmatrix PreTransformMatrix)
{
    if (-1 == m_iParentIndex)
        XMStoreFloat4x4(&m_CombinedTransformMatrix, XMLoadFloat4x4(&m_TransformMatrix) * PreTransformMatrix);
    else
        XMStoreFloat4x4(&m_CombinedTransformMatrix, XMLoadFloat4x4(&m_TransformMatrix) * XMLoadFloat4x4(Bones[m_iParentIndex]->Get_CombinedTransformMatrix()));
}

CBone* CBone::Create(const aiNode* pNode, _int iParentIndex)
{
    CBone* pInstance = new CBone();

    if (FAILED(pInstance->Initialize(pNode, iParentIndex)))
        Safe_Release(pInstance);

    return pInstance;
}

void CBone::Free()
{
}
