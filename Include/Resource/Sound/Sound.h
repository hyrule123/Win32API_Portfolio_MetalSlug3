#pragma once
#include "../../Ref.h"



class CSound :
	public CRef
{
	friend class CSoundManager;

private:
	CSound();
	~CSound();
	void Init();

	FMOD::System* m_System;
	FMOD::Sound* m_Sound;
	FMOD::ChannelGroup* m_ChannelGroup;
	FMOD::Channel* m_Channel;

	//루프 관련
	bool m_isLoop;
public:
	bool GetLoop() const;

	
	//재생 관련
private:
	//bool m_isPlaying;
public:
	bool GetPlaying() const;

	void Play();
	void Stop();
	void Pause();
	void Resume();
	void FadeIn(float Duration);
	void FadeOut(float Duration);
	void SetNoDelay();

	

	//사운드 로드
	bool LoadSound(FMOD::System* System, 
		FMOD::ChannelGroup* Group, 
		bool Loop, 
		const char* FileName, 
		const std::string& PathName = SOUND_PATH);

};

