#include "SoundManager.h"

#include "Sound.h"

CSoundManager::CSoundManager()
{
}

CSoundManager::~CSoundManager()
{
	m_mapSound.clear();

	{
		auto iter = m_mapChannelGroup.begin();
		auto iterEnd = m_mapChannelGroup.end();

		while (iter != iterEnd)
		{
			iter->second->release();

			++iter;
		}
	}


	if (m_System)
	{
		m_System->close();
		m_System->release();
	}

}

bool CSoundManager::Init()
{
	//시스템 생성
	FMOD_RESULT Result = FMOD::System_Create(&m_System);
	if (Result != FMOD_OK)
		return false;

	//초기화
	Result = m_System->init(256, FMOD_INIT_NORMAL, nullptr);
	if (Result != FMOD_OK)
		return false;

	//마스터 채널 그룹 생성 및 저장
	Result = m_System->getMasterChannelGroup(&m_MasterGroup);
	if (Result != FMOD_OK)
		return false;

	//마스터 그룹도 map에 삽입
	m_mapChannelGroup.insert(std::make_pair("Master", m_MasterGroup));

	//추가로 몇가지 채널도 만들어놓자.
	CreateSoundChannelGroup("BGM");
	CreateSoundChannelGroup("SFX");
	CreateSoundChannelGroup("UI");


	return true;
}

bool CSoundManager::CreateSoundChannelGroup(const std::string& Name)
{
	//이미 만들어져 있으면 해당 더이상 진행하지 않고 true를 반환 
	if (FindChannelGroup(Name))
		return true;

	FMOD::ChannelGroup* Group = nullptr;

	FMOD_RESULT Result = m_System->createChannelGroup(Name.c_str(), &Group);

	if (Result != FMOD_OK)
		return false;

	m_MasterGroup->addGroup(Group, false);


	m_mapChannelGroup.insert(std::make_pair(Name, Group));


	return true;
}

FMOD::ChannelGroup* CSoundManager::FindChannelGroup(const std::string& Name)
{
	auto iter = m_mapChannelGroup.find(Name);

	if (iter == m_mapChannelGroup.end())
		return nullptr;

	return iter->second;
}

CSound* CSoundManager::FindSound(const std::string& Name)
{
	auto iter = m_mapSound.find(Name);

	if (iter == m_mapSound.end())
		return nullptr;

	return iter->second;
}

bool CSoundManager::LoadSound(const std::string& GroupName, const std::string& Name, bool Loop, const char* FileName, const std::string& PathName)
{
	//사운드 클래스를 찾고 있으면 true 반환(이미 만들어져 있으므로)
	CSound* Sound = FindSound(Name);
	if (Sound)
		return true;

	//채널그룹을 찾고 없으면 false 반환
	FMOD::ChannelGroup* ChannelGroup = FindChannelGroup(GroupName);
	if (!ChannelGroup)
		return false;

	Sound = new CSound;
	Sound->SetName(Name);

	if (!Sound->LoadSound(m_System, ChannelGroup, Loop, FileName, PathName))
	{
		delete Sound;
		return false;
	}

	//제대로 만들어졌을 경우 맵에 추가
	m_mapSound.insert(std::make_pair(Name, Sound));

	return true;
}


bool CSoundManager::SetMasterVolume(int Volume)
{
	m_MasterGroup->setVolume(Volume / 100.f);

	return true;
}



bool CSoundManager::SetVolume(const std::string& GroupName, int Volume)
{
	FMOD::ChannelGroup* Group = FindChannelGroup(GroupName);
	if (!Group)
		return false;

	Group->setVolume(Volume / 100.f);

	return true;
}

bool CSoundManager::SoundPlay(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	Sound->Play();

	return true;
}

bool CSoundManager::SoundStop(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	Sound->Stop();

	return true;
}

bool CSoundManager::SoundPause(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	Sound->Pause();

	return true;
}

bool CSoundManager::SoundResume(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	Sound->Resume();

	return true;
}



bool CSoundManager::GetPlaying(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	return Sound->GetPlaying();
}

bool CSoundManager::SoundFadeIn(const std::string& Name, float Duration)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	Sound->FadeIn(Duration);

	return true;
}

bool CSoundManager::SoundFadeOut(const std::string& Name, float Duration)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	Sound->FadeOut(Duration);

	return true;
}

bool CSoundManager::SoundPlayOneChan(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	if (!Sound->GetPlaying())
	{
		Sound->Play();
		return true;
	}
	
	return false;
}

bool CSoundManager::SoundSetNodelay(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	if (Sound->GetPlaying())
	{
		Sound->SetNoDelay();
		return true;
	}

	return false;
}

void CSoundManager::ReleaseSound(const std::string& Name)
{
	auto iter = m_mapSound.find(Name);

	if (iter == m_mapSound.end())
		return;

	if (iter->second->GetRefCount() == 1)
		m_mapSound.erase(iter);

}

void CSoundManager::Update()
{
	//FMOD의 상태도 업데이트 해준다.
	m_System->update();
}
