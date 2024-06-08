#pragma once
#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL CSoundMgr :
	public CBase
{
private:
	CSoundMgr();
	virtual ~CSoundMgr();

public:
	HRESULT Initialize();

public:
	void Ready_Sound();
	void PlaySound_W(TCHAR* pSoundKey, CHANNELID eID, float fVolume);
	void PlayBGM(TCHAR* pSoundKey, float fVolume);
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

public:
	static CSoundMgr* Create();
	virtual void Free() override;
};
END