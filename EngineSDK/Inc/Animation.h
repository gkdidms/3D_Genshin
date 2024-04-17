#pragma once
#include "Base.h"

BEGIN(Engine)
class CAnimation final:
    public CBase
{
private:
    CAnimation();
    CAnimation(const CAnimation& rhs);
    virtual ~CAnimation() = default;

public:
    _bool IsFinished() const { return m_IsFinished; }
    _bool IsLoopFinished() const { return m_IsLoopFinished; }
    _bool IsFirst() const { return m_IsFirst; }

public:
    HRESULT Initialize(aiAnimation* pAiAnimation, const vector<class CBone*> Bones);
    HRESULT Initialize(const char* pName, _double Duration, _double TickPerSecond, _uint iNumChannels, vector<class CChannel*> Channels);

public:
    void Update_TransformationMatrix(const _float& fTimeDelta, const vector<CBone*> Bones, _bool isLoop);
    void Linear_TransformationMatrix(const _float& fTimeDelta, const vector<CBone*> Bones);
    void Reset();
    void Loop_Reset() { m_IsLoopFinished = false; }

private:
    char m_szName[MAX_PATH] = "";
    double m_Duration = { 0.0 };
    double m_TickPerSecond = { 0.0 };
    _uint m_iNumChannels = { 0 };

    vector<class CChannel*> m_Channels;

    double m_iCurrentPosition = { 0 };

    vector<_uint> m_CurrentKeyFrameIndex;
    _bool m_IsFinished = { false };

private:
    _bool m_IsLoopFinished = { false }; // 루프 애니메이션이 끝났는지 아닌지 확인
    _bool m_IsFirst = { true };

public:
    static CAnimation* Create(aiAnimation* pAiAnimation, const vector<class CBone*> Bones);
    static CAnimation* Create(const char* pName, _double Duration, _double TickPerSecond, _uint iNumChannels, vector<class CChannel*> Channels);
    CAnimation* Clone();
    virtual void Free() override;
};
END
