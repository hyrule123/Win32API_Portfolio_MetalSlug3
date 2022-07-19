#pragma once

#include "../../GameInfo.h"

class CSoundManager
{
	friend class CResourceManager;
private:
	CSoundManager();
	~CSoundManager();

	bool Init();

	std::unordered_map<std::string, CSharedPtr<class CSound>>	m_mapSound;

	FMOD::System* m_System;
	FMOD::ChannelGroup* m_MasterGroup;
	std::unordered_map<std::string, FMOD::ChannelGroup*> m_mapChannelGroup;

public:
	//����
	bool CreateSoundChannelGroup(const std::string& Name);

	//Ž��
	FMOD::ChannelGroup* FindChannelGroup(const std::string& Name);
	class CSound* FindSound(const std::string& Name);

	//�ε�
	bool LoadSound(
		const std::string& GroupName,
		const std::string& Name,
		bool Loop,
		const char* FileName,
		const std::string& PathName = SOUND_PATH);

	//��Ÿ ����
	bool SetMasterVolume(int Volume);
	bool SetVolume(const std::string& GroupName, int Volume);
	bool SoundPlay(const std::string& Name);
	bool SoundStop(const std::string& Name);
	bool SoundPause(const std::string& Name);
	bool SoundResume(const std::string& Name);
	bool GetPlaying(const std::string& Name);
	bool SoundFadeIn(const std::string& Name, float Duration);
	bool SoundFadeOut(const std::string& Name, float Duration);
	bool SoundPlayOneChan(const std::string& Name);
	bool SoundSetNodelay(const std::string& Name);

	
	//����(SceneResource ��)
	void ReleaseSound(const std::string& Name);


	void Update();
};

