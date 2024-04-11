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
    HRESULT Initialize(const char* pName, _int iBoneIndex, _uint iNumKeyFrames, vector<KEYFRAME> KeyFrames);

    void Update_TransformationMatrix(double CurrentPosition, const vector<class CBone*> Bones, _uint* iCurrentKeyFrameIndex);
    void First_TransformationMatrix(double CurrentPosition, const vector<class CBone*> Bones, _uint* iCurrentKeyFrameIndex);
private:
    char m_szName[MAX_PATH] = "";
    _uint m_iBoneIndex = { 0 };
    _uint m_iNumKeyFrames = { 0 };

    vector<KEYFRAME> m_KeyFrames;

public:
    static CChannel* Create(aiNodeAnim* pChannel, const vector<class CBone*> Bones);
    static CChannel* Create(const char* pName, _int iBoneIndex, _uint iNumKeyFrames, vector<KEYFRAME> KeyFrames);
    virtual void Free() override;
};
END
