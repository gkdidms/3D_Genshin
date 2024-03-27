#pragma once
#include "Base.h"

BEGIN(Engine)
class CAnimation final:
    public CBase
{
private:
    CAnimation();
    virtual ~CAnimation() = default;

public:
    HRESULT Initialize(aiAnimation* pAiAnimation, const vector<class CBone*> Bones);

    void Update_TransformationMatrix(const _float& fTimeDelta, const vector<CBone*> Bones);

private:
    char m_szName[MAX_PATH] = "";
    double m_Duration = { 0.0 };
    double m_TickPerSecond = { 0.0 };
    _uint m_iNumChannels = { 0 };
    vector<class CChannel*> m_Channels;

    double m_iCurrentPosition = { 0 };

public:
    static CAnimation* Create(aiAnimation* pAiAnimation, const vector<class CBone*> Bones);
    virtual void Free() override;
};
END
