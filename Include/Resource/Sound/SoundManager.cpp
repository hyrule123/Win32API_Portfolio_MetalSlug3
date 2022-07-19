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
	//�ý��� ����
	FMOD_RESULT Result = FMOD::System_Create(&m_System);
	if (Result != FMOD_OK)
		return false;

	//�ʱ�ȭ
	Result = m_System->init(256, FMOD_INIT_NORMAL, nullptr);
	if (Result != FMOD_OK)
		return false;

	//������ ä�� �׷� ���� �� ����
	Result = m_System->getMasterChannelGroup(&m_MasterGroup);
	if (Result != FMOD_OK)
		return false;

	//������ �׷쵵 map�� ����
	m_mapChannelGroup.insert(std::make_pair("Master", m_MasterGroup));

	//�߰��� ��� ä�ε� ��������.
	CreateSoundChannelGroup("BGM");
	CreateSoundChannelGroup("SFX");
	CreateSoundChannelGroup("UI");


	return true;
}

bool CSoundManager::CreateSoundChannelGroup(const std::string& Name)
{
	//�̹� ������� ������ �ش� ���̻� �������� �ʰ� true�� ��ȯ 
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
	//���� Ŭ������ ã�� ������ true ��ȯ(�̹� ������� �����Ƿ�)
	CSound* Sound = FindSound(Name);
	if (Sound)
		return true;

	//ä�α׷��� ã�� ������ false ��ȯ
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

	//����� ��������� ��� �ʿ� �߰�
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
	//FMOD�� ���µ� ������Ʈ ���ش�.
	m_System->update();
}
