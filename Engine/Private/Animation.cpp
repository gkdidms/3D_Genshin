#include "Animation.h"

#include "Channel.h"

CAnimation::CAnimation()
{
}

CAnimation::CAnimation(const CAnimation& rhs)
    : m_Duration{ rhs.m_Duration },
    m_TickPerSecond { rhs.m_TickPerSecond },
    m_iNumChannels { rhs.m_iNumChannels },
    m_Channels { rhs.m_Channels },
    m_iCurrentPosition { rhs.m_iCurrentPosition },
    m_IsFinished { rhs.m_IsFinished },
    m_CurrentKeyFrameIndex { rhs.m_CurrentKeyFrameIndex }
{
    strcpy_s(m_szName, rhs.m_szName);
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

HRESULT CAnimation::Initialize(const char* pName, _double Duration, _double TickPerSecond, _uint iNumChannels, vector<class CChannel*> Channels)
{
    strcpy_s(m_szName, pName);

    m_Duration = Duration;
    m_TickPerSecond = TickPerSecond;
    m_iNumChannels = iNumChannels;

    m_CurrentKeyFrameIndex.resize(m_iNumChannels);

    for (size_t i = 0; i < m_iNumChannels; ++i)
    {
        m_Channels.emplace_back(Channels[i]);
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

        if (!isLoop) m_IsFinished = true;
    }

    if (!m_IsFinished)
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
    m_IsFinished = false;
    ZeroMemory(&m_CurrentKeyFrameIndex.front(), sizeof(_uint) * m_iNumChannels);
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

CAnimation* CAnimation::Create(const char* pName, _double Duration, _double TickPerSecond, _uint iNumChannels, vector<class CChannel*> Channels)
{
    CAnimation* pInstance = new CAnimation();

    if (FAILED(pInstance->Initialize(pName, Duration, TickPerSecond, iNumChannels, Channels)))
        Safe_Release(pInstance);

    return pInstance;
}

void CAnimation::Free()
{
    for (auto& iter : m_Channels)
        Safe_Release(iter);

    m_Channels.clear();
}
