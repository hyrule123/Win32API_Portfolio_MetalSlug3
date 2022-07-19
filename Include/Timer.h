#pragma once

#include "GameInfo.h"

class CTimer
{
public:
	CTimer();
	~CTimer();

	void Init();
	float Update();

private:
	LARGE_INTEGER m_Freq;
	LARGE_INTEGER m_LastTime;
	int m_FPS;
	int m_Tick;
	float m_Second;
	float m_FrameLimit;


public:
	void SetFrameLimit(int FramePerSecond) 
	{
		if (FramePerSecond == 0)
			m_FrameLimit = 0.f;
		else
			m_FrameLimit = 1.f / FramePerSecond;
	}

	int GetFPS() const
	{
		return m_FPS;
	}
};

