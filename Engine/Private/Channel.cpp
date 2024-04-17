#include "Channel.h"

#include "Bone.h"

CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(aiNodeAnim* pChannel, const vector<CBone*> Bones)
{
	strcpy_s(m_szName, pChannel->mNodeName.data);

	_uint iBoneIndex = { 0 };
	auto iter = find_if(Bones.begin(), Bones.end(), [&](CBone* pBone)->_bool {
		if (pBone->Compare_NodeName(m_szName))
			return true;

		++iBoneIndex;

		return false;
	});

	m_iBoneIndex = iBoneIndex;

	m_iNumKeyFrames = max(pChannel->mNumScalingKeys, pChannel->mNumRotationKeys);
	m_iNumKeyFrames = max(m_iNumKeyFrames, pChannel->mNumPositionKeys);

	KEYFRAME KeyFrame{};

	for (size_t i = 0; i < m_iNumKeyFrames; ++i)
	{
		if (pChannel->mNumScalingKeys > i)
		{
			memcpy(&KeyFrame.m_vScale, &pChannel->mScalingKeys[i].mValue, sizeof(_float3));
			KeyFrame.Time = pChannel->mScalingKeys[i].mTime;
		}
		if (pChannel->mNumRotationKeys > i)
		{
			//쿼터니언은 w,x,y,z 순으로 저장이 되어있어서 메모리 복사를 하면 안되고 값을 대입해주어야 한다.
			KeyFrame.vRotate.x = pChannel->mRotationKeys[i].mValue.x;
			KeyFrame.vRotate.y = pChannel->mRotationKeys[i].mValue.y;
			KeyFrame.vRotate.z = pChannel->mRotationKeys[i].mValue.z;
			KeyFrame.vRotate.w = pChannel->mRotationKeys[i].mValue.w;

			KeyFrame.Time = pChannel->mRotationKeys[i].mTime;
		}
		if (pChannel->mNumPositionKeys > i)
		{
			memcpy(&KeyFrame.vPosition, &pChannel->mPositionKeys[i].mValue, sizeof(_float3));
			KeyFrame.Time = pChannel->mPositionKeys[i].mTime;
		}

		m_KeyFrames.emplace_back(KeyFrame);
	}

	return S_OK;
}

HRESULT CChannel::Initialize(const char* pName, _int iBoneIndex, _uint iNumKeyFrames, vector<KEYFRAME> KeyFrames)
{
	strcpy_s(m_szName, pName);

	m_iBoneIndex = iBoneIndex;

	m_iNumKeyFrames = iNumKeyFrames;

	m_KeyFrames = KeyFrames;
		
	return S_OK;
}

void CChannel::Update_TransformationMatrix(double CurrentPosition, const vector<CBone*> Bones, _uint* iCurrentKeyFrameIndex)
{
	KEYFRAME LastKeyFrame = m_KeyFrames.back();

	_vector vScale, vRotation, vTranslation;
	if (CurrentPosition >= LastKeyFrame.Time)
	{
		vScale = XMLoadFloat3(&LastKeyFrame.m_vScale);
		vRotation = XMLoadFloat4(&LastKeyFrame.vRotate);
		vTranslation = XMVectorSetW(XMLoadFloat3(&LastKeyFrame.vPosition), 1.f);
	}
	else
	{
		while (CurrentPosition >= m_KeyFrames[*iCurrentKeyFrameIndex + 1].Time)
			++*iCurrentKeyFrameIndex;

		_float fRatio = (CurrentPosition - m_KeyFrames[*iCurrentKeyFrameIndex].Time) / (m_KeyFrames[*iCurrentKeyFrameIndex + 1].Time - m_KeyFrames[*iCurrentKeyFrameIndex].Time);

		vScale = XMVectorLerp(XMLoadFloat3(&m_KeyFrames[*iCurrentKeyFrameIndex].m_vScale), XMLoadFloat3(&m_KeyFrames[*iCurrentKeyFrameIndex + 1].m_vScale), fRatio);
		vRotation = XMQuaternionSlerp(XMLoadFloat4(&m_KeyFrames[*iCurrentKeyFrameIndex].vRotate), XMLoadFloat4(&m_KeyFrames[*iCurrentKeyFrameIndex + 1].vRotate), fRatio);
		vTranslation = XMVectorSetW(XMVectorLerp(XMLoadFloat3(&m_KeyFrames[*iCurrentKeyFrameIndex].vPosition), XMLoadFloat3(&m_KeyFrames[*iCurrentKeyFrameIndex + 1].vPosition), fRatio), 1.f);
	}

	_matrix TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);
	
	Bones[m_iBoneIndex]->Set_TranformationMatrix(TransformationMatrix);
	Bones[m_iBoneIndex]->Set_StateVector(vScale, vRotation, vTranslation);
}

void CChannel::First_TransformationMatrix(double CurrentPosition, const vector<class CBone*> Bones, double LinearDuration)
{
	_vector vScale, vRotation, vTranslation;

	_vector vPreScale, vPreRotation, vPreTranslation;
	vPreScale = Bones[m_iBoneIndex]->Get_Scale();
	vPreRotation = Bones[m_iBoneIndex]->Get_Rotation();
	vPreTranslation = Bones[m_iBoneIndex]->Get_Translation();

	_float fRatio = (CurrentPosition - m_KeyFrames[0].Time) / LinearDuration;
	
	vScale = XMVectorLerp(vPreScale, XMLoadFloat3(&m_KeyFrames[0].m_vScale), fRatio);
	vRotation = XMQuaternionSlerp(vPreRotation, XMLoadFloat4(&m_KeyFrames[0].vRotate), fRatio);
	vTranslation = XMVectorSetW(XMVectorLerp(vPreTranslation, XMLoadFloat3(&m_KeyFrames[0].vPosition), fRatio), 1.f);
	
	_matrix TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);

	Bones[m_iBoneIndex]->Set_TranformationMatrix(TransformationMatrix);
}

CChannel* CChannel::Create(aiNodeAnim* pChannel, const vector<class CBone*> Bones)
{
	CChannel* pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(pChannel, Bones)))
		Safe_Release(pInstance);

	return pInstance;
}

CChannel* CChannel::Create(const char* pName, _int iBoneIndex, _uint iNumKeyFrames, vector<KEYFRAME> KeyFrames)
{
	CChannel* pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(pName, iBoneIndex, iNumKeyFrames, KeyFrames)))
		Safe_Release(pInstance);

	return pInstance;
}

void CChannel::Free()
{
	m_KeyFrames.clear();
}
