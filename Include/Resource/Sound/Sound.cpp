#include "Sound.h"

#include "../../PathManager.h"

CSound::CSound():
	m_System(nullptr),
	m_ChannelGroup(nullptr),
	m_Channel(nullptr),
	m_isLoop(false)
{
	SetTypeID<CSound>();
}

CSound::~CSound()
{

	if (m_Sound)
		m_Sound->release();

}

void CSound::Init()
{
}

bool CSound::GetLoop() const
{
	return m_isLoop;
}

bool CSound::GetPlaying() const
{
	bool Playing = false;

	if (m_Channel)
	{
		//근데 거기서 재생 중일 경우
		m_Channel->isPlaying(&Playing);
	}

	return Playing;
}

void CSound::Play()
{
	m_System->playSound(m_Sound, m_ChannelGroup, false, &m_Channel);
	//m_isPlaying = true;

}

void CSound::Stop()
{
	//채널에 할당되어 있을 경우
	if (m_Channel)
	{
		bool Playing = false;

		//근데 거기서 재생 중일 경우
		m_Channel->isPlaying(&Playing);
		if (Playing)
		{
			//재생을 멈추고 채널과의 연결을 해제한다.
			m_Channel->stop();
			m_Channel = nullptr;
		}
	}
}

void CSound::Pause()
{
	if (m_Channel)
	{
		bool Playing = false;

		m_Channel->isPlaying(&Playing);
		if (Playing)
		{
			m_Channel->setPaused(true);
			//m_isPlaying = false;
		}
	}

}

void CSound::Resume()
{
	if (m_Channel)
	{
		bool Playing = false;

		m_Channel->isPlaying(&Playing);
		if (Playing)
		{
			m_Channel->setPaused(false);
			//m_isPlaying = true;
		}
	}
}

void CSound::FadeIn(float Duration)
{
	if (!m_Channel)
		return;

	bool Playing = false;

	m_Channel->isPlaying(&Playing);
	if (Playing)
	{
		m_Channel->setPaused(true);
		//m_isPlaying = false;
	}

	unsigned long long dspclock;
	int rate;
	FMOD_RESULT result;

	result = m_Channel->getSystemObject(&m_System);                        // OPTIONAL : Get System object
	if (result != FMOD_OK)
		return;


	result = m_System->getSoftwareFormat(&rate, 0, 0);                // Get mixer rate
	if (result != FMOD_OK)
		return;

	result = m_Channel->getDSPClock(0, &dspclock);                    // Get the reference clock, which is the parent channel group
	if (result != FMOD_OK)
		return;

	result = m_Channel->addFadePoint(dspclock, 0.0f);                 // Add a fade point at 'now' with full volume.
	if (result != FMOD_OK)
		return;

	result = m_Channel->addFadePoint(dspclock + (unsigned __int64)(rate * Duration), 1.0f);    // Add a fade point 5 seconds later at 0 volume.
	if (result != FMOD_OK)
		return;

	Playing = false;

	m_Channel->isPlaying(&Playing);
	if (Playing)
	{
		m_Channel->setPaused(false);
		//m_isPlaying = true;
	}
}

void CSound::FadeOut(float Duration)
{
	if (!m_Channel)
		return;

	unsigned long long dspclock;
	int rate;
	FMOD_RESULT result;

	result = m_Channel->getSystemObject(&m_System);                        // OPTIONAL : Get System object

	if (result != FMOD_OK)
		return;

	result = m_System->getSoftwareFormat(&rate, 0, 0);                // Get mixer rate
	if (result != FMOD_OK)
		return;

	result = m_Channel->getDSPClock(0, &dspclock);                    // Get the reference clock, which is the parent channel group
	if (result != FMOD_OK)
		return;


	result = m_Channel->addFadePoint(dspclock, 1.0f);                 // Add a fade point at 'now' with full volume.
	if (result != FMOD_OK)
		return;


	result = m_Channel->addFadePoint(dspclock + (unsigned __int64)(rate * Duration), 0.0f);    // Add a fade point 5 seconds later at 0 volume.
	if (result != FMOD_OK)
		return;

	result = m_Channel->setDelay(0, dspclock + (unsigned __int64)(rate * Duration), true);     // Add a delayed stop command at 5 seconds ('stopchannels = true')
	if (result != FMOD_OK)
		return;


}

void CSound::SetNoDelay()
{
	unsigned int dsp_block_len = 0;
	int outputrate = 0;

	/*
	Get information needed later for scheduling.  The mixer block size, and the output rate of the mixer.
*/
	FMOD_RESULT result = m_System->getDSPBufferSize(&dsp_block_len, 0);

	result = m_System->getSoftwareFormat(&outputrate, 0, 0);


	/*
		Play all the sounds at once! Space them apart with set delay though so that they sound like they play in order.
	*/

	static unsigned long long clock_start = 0;
	unsigned int slen;

	Play();
	Pause();

	if (!clock_start)
	{
		result = m_Channel->getDSPClock(0, &clock_start);


		clock_start += (dsp_block_len * 2);                         /* Start the sound into the future, by 2 mixer blocks worth. */
																	/* Should be enough to avoid the mixer catching up and hitting the clock value before we've finished setting up everything. */
																	/* Alternatively the channelgroup we're basing the clock on could be paused to stop it ticking. */
	}


	float freq;

	result = m_Sound->getLength(&slen, FMOD_TIMEUNIT_PCM);            /* Get the length of the sound in samples. */

	result = m_Sound->getDefaults(&freq, 0);                          /* Get the default frequency that the sound was recorded at. */

	slen = (unsigned int)((float)slen / freq * outputrate);     /* Convert the length of the sound to 'output samples' for the output timeline. */

	clock_start += slen;                                        /* Place the sound clock start time to this value after the last one. */


	result = m_Channel->setDelay(clock_start, 0, false);              /* Schedule the channel to start in the future at the newly calculated channelgroup clock value. */

	//Resume();                           /* Unpause the sound.  Note that you won't hear the sounds, they are scheduled into the future. */
}

bool CSound::LoadSound(FMOD::System* System, FMOD::ChannelGroup* Group, bool Loop, const char* FileName, const std::string& PathName)
{
	m_System = System;
	m_ChannelGroup = Group;
	m_isLoop = Loop;

	//주소를 받아온다.
	char FullPath[MAX_PATH] = {};
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);
	if (Info)
	{
		strcpy_s(FullPath, Info->PathMultiByte);
	}
	strcat_s(FullPath, FileName);


	//루프 여부를 확인하고 설정한다.
	FMOD_MODE Mode = FMOD_DEFAULT;
	if (Loop)
	{
		Mode = FMOD_LOOP_NORMAL;
	}


	//만든 변수들을 인자로 넘겨서 사운드를 만들고 실패하면 false를 반환한다.
	if (FMOD_OK != m_System->createSound(FullPath, Mode, nullptr, &m_Sound))
	{
		return false;
	}


	return true;
}
