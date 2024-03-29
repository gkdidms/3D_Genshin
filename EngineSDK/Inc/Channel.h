#pragma once
#include "Base.h"

BEGIN(Engine)
class CChannel final:
    public CBase
{
private:
    CChannel();
    virtual ~CChannel() = default;

public:
    HRESULT Initialize(aiNodeAnim* pChannel, const vector<class CBone*> Bones);

    void Update_TransformationMatrix(double CurrentPosition, const vector<class CBone*> Bones, _uint* iCurrentKeyFrameIndex);

private:
    char m_szName[MAX_PATH] = "";
    _uint m_iBoneIndex = { 0 };
    _uint m_iNumKeyFrames = { 0 };

    vector<KEYFRAME> m_KeyFrames;


public:
    static CChannel* Create(aiNodeAnim* pChannel, const vector<class CBone*> Bones);
    virtual void Free() override;
};
END
