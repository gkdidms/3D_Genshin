#include "Animation.h"

#include "Channel.h"

CAnimation::CAnimation()
{
}

CAnimation::CAnimation(const CAnimation& rhs)
    : m_Duration{rhs.m_Duration },
    m_TickPerSecond { rhs.m_TickPerSecond },
    m_iNumChannels { rhs.m_iNumChannels },
    m_Channels { rhs.m_Channels },
    m_iCurrentPosition { rhs.m_iCurrentPosition },
    m_isFinished { rhs.m_isFinished },
    m_CurrentKeyFrameIndex { rhs.m_CurrentKeyFrameIndex }
{
    for (auto& pChannel : m_Channels)
        Safe_AddRef(pChannel);
}

HRESULT CAnimation::Initialize(aiAnimation* pAiAnimation, const vector<class CBone*> Bones)
{
    strcpy_s(m_szName, pAiAnimation->mName.data);

    m_Duration = pAiAnimation->mDuration;
    m_TickPerSecond = pAiAnimation->mTicksPerSecond;
    m_iNumChannels = pAiAnimation->mNumChannels;

    m_CurrentKeyFrameIndex.resize(m_iNumChannels);

    for (size_t i = 0; i < m_iNumChannels; ++i)
    {
        CChannel* pChannel = CChannel::Create(pAiAnimation->mChannels[i], Bones);
        if (nullptr == pChannel)
            return E_FAIL;

        m_Channels.emplace_back(pChannel);
    }

    return S_OK;
}

void CAnimation::Update_TransformationMatrix(const _float& fTimeDelta, const vector<CBone*> Bones, _bool isLoop)
{
    m_iCurrentPosition += m_TickPerSecond * fTimeDelta;

    if (m_iCurrentPosition >= m_Duration)
    {
        //애니메이션 끝
        m_iCurrentPosition = 0.0;

        if (!isLoop) m_isFinished = true;
    }

    if (!m_isFinished)
    {
        for (size_t i = 0; i < m_iNumChannels; ++i)
        {
            m_Channels[i]->Update_TransformationMatrix(m_iCurrentPosition, Bones, &m_CurrentKeyFrameIndex[i]);
        }
    }
}

void CAnimation::Reset()
{
    m_iCurrentPosition = 0.0;
    m_isFinished = false;
}

CAnimation* CAnimation::Create(aiAnimation* pAiAnimation, const vector<class CBone*> Bones)
{
    CAnimation* pInstance = new CAnimation();

    if (FAILED(pInstance->Initialize(pAiAnimation, Bones)))
        Safe_Release(pInstance);

    return pInstance;
}

CAnimation* CAnimation::Clone()
{
    return new CAnimation(*this);
}

void CAnimation::Free()
{
    for (auto& iter : m_Channels)
        Safe_Release(iter);

    m_Channels.clear();
}
