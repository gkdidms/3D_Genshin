#include "Animation.h"

#include "Channel.h"

CAnimation::CAnimation()
{
}

HRESULT CAnimation::Initialize(aiAnimation* pAiAnimation, const vector<class CBone*> Bones)
{
    strcpy_s(m_szName, pAiAnimation->mName.data);

    m_Duration = pAiAnimation->mDuration;
    m_TickPerSecond = pAiAnimation->mTicksPerSecond;
    m_iNumChannels = pAiAnimation->mNumChannels;

    for (size_t i = 0; i < m_iNumChannels; ++i)
    {
        CChannel* pChannel = CChannel::Create(pAiAnimation->mChannels[i], Bones);
        if (nullptr == pChannel)
            return E_FAIL;

        m_Channels.emplace_back(pChannel);
    }

    return S_OK;
}

void CAnimation::Update_TransformationMatrix(const _float& fTimeDelta, const vector<CBone*> Bones)
{
    m_iCurrentPosition += m_TickPerSecond * fTimeDelta;

    if (m_iCurrentPosition >= m_Duration)
    {
        //애니메이션 끝
        m_iCurrentPosition = 0.0;
    }

    for (auto& iter : m_Channels)
    {
        iter->Update_TransformationMatrix(m_iCurrentPosition, Bones);
    }
}

CAnimation* CAnimation::Create(aiAnimation* pAiAnimation, const vector<class CBone*> Bones)
{
    CAnimation* pInstance = new CAnimation();

    if (FAILED(pInstance->Initialize(pAiAnimation, Bones)))
        Safe_Release(pInstance);

    return pInstance;
}

void CAnimation::Free()
{
    for (auto& iter : m_Channels)
        Safe_Release(iter);

    m_Channels.clear();
}
