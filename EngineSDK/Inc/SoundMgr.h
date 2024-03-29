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
	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;

	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[SOUND_END];

	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem = { nullptr };

private:
	virtual void Free();
};
END