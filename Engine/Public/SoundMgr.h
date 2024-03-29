#pragma once
#include "Base.h"

class CSound;

BEGIN(Engine)
class ENGINE_DLL SoundMgr :
	public CBase
{
	DECLARE_SINGLETON(SoundMgr)

private:
	explicit SoundMgr();
	virtual ~SoundMgr();

public:
	void Ready_Sound();
	void PlaySound_W(TCHAR* pSoundKey, CHANNELID eID, float fVolume);
	void PlaySound_NonStop(TCHAR* pSoundKey, CHANNELID eID, float fVolume);
	void PlayBGM(TCHAR* pSoundKey, float fVolume);
	void PlayEffectContinue(TCHAR* pSoundKey, float fVolume, CHANNELID eID);
	void StopSound(CHANNELID eID);
	void StopAll();
	void SetChannelVolume(CHANNELID eID, float fVolume);


private:
	void LoadSoundFile();

private:
	// ���� ���ҽ� ������ ���� ��ü 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;

	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD_CHANNEL* m_pChannelArr[SOUND_END];

	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD_SYSTEM* m_pSystem = { nullptr };

private:
	virtual void Free();
};
END