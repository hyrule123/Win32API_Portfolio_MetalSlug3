#include "Timer.h"

CTimer::CTimer():
	m_FPS(0),
	m_Second(0.f),
	m_Tick(0)
{
}

CTimer::~CTimer()
{

}

void CTimer::Init()
{
	//고해상도 타이머의 1초당 진동수를 얻어온다.
	QueryPerformanceFrequency(&m_Freq);
	QueryPerformanceCounter(&m_LastTime);

	SetFrameLimit(60);
}

float CTimer::Update()
{
	float DeltaTime = 0.f;

	do 
	{
		LARGE_INTEGER Time;
		QueryPerformanceCounter(&Time);

		DeltaTime += (Time.QuadPart - m_LastTime.QuadPart) / (float)m_Freq.QuadPart;

		m_LastTime = Time;
	} while (DeltaTime < m_FrameLimit);
	
	m_Second += DeltaTime;
	++m_Tick;

	if(m_Second > 1.f)
	{ 
		m_FPS = m_Tick;
		m_Tick = 0;
		m_Second = 0.f;
	}

	return DeltaTime > m_FrameLimit ? m_FrameLimit : DeltaTime;
}
