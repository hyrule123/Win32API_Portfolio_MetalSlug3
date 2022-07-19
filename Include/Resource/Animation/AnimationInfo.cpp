#include "AnimationInfo.h"

CAnimationInfo::CAnimationInfo() :
	m_Frame(0),
	m_Time(0.f),
	m_FrameTime(0.f),
	m_PlayTime(1.f),
	m_PlayScale(1.f),
	m_LoopFlag(ELoopFlag::NoLoop),
	m_Reverse(false),
	m_ReverseOnce(false),
	m_EndFuncOnce(false),
	m_Layer(0),
	m_Pivot(0.5f, 1.f),
	m_PlayOnce(),
	m_UseDuration(false),
	m_Duration(-1.f)
{
}

CAnimationInfo::CAnimationInfo(const CAnimationInfo& Obj) :
	m_Sequence(Obj.m_Sequence),
	m_Offset(Obj.m_Offset),
	m_Pivot(Obj.m_Pivot),
	m_Layer(Obj.m_Layer),
	m_Frame(Obj.m_Frame),
	m_Time(Obj.m_Time),
	m_FrameTime(Obj.m_FrameTime),
	m_PlayTime(Obj.m_PlayTime),
	m_PlayScale(Obj.m_PlayScale),
	m_LoopFlag(Obj.m_LoopFlag),
	m_PlayOnce(Obj.m_PlayOnce),
	m_Reverse(Obj.m_Reverse),
	m_ReverseOnce(Obj.m_ReverseOnce),
	m_EndFuncOnce(Obj.m_EndFuncOnce),
	m_UseDuration(Obj.m_UseDuration),
	m_Duration(Obj.m_Duration),
	m_EndFunction(),
	m_vecNotify{}
{
	//Init() 이후에 초기화되는 함수
	//m_EndFunction(Obj.m_EndFunction)

	//vecNotify는 어차피 Init() 이후에 생성되므로 복사할 필요 없음.
	//std::vector<std::vector<AnimationNotify*>> m_vecNotify;
}

CAnimationInfo::~CAnimationInfo()
{
	size_t size = m_vecNotify.size();

	for (size_t i = 0; i < size; ++i)
	{
		size_t size = m_vecNotify[i].size();
		for (size_t j = 0; j < size; ++j)
		{
			SAFE_DELETE(m_vecNotify[i][j]);
		}
		
	}

}

void CAnimationInfo::Init()
{
	if (m_Reverse)
		m_Frame = (int)m_Sequence->GetFrameCount() - 1;
	else
		m_Frame = 0;

	m_Time = 0.f;

	//노티파이 이차원 배열 call 여부 초기화
	size_t size = m_vecNotify.size();
	for (int i = 0; i < size; ++i)
	{
		if (m_vecNotify[i].empty())
			continue;

		size_t _size = m_vecNotify[i].size();

		for (int j = 0; j < _size; ++j)
		{
			m_vecNotify[i][j]->Call = false;
		}
	}

	m_EndFunction.Call = false;
}



bool CAnimationInfo::GetReverse() const
{
	return m_Reverse;
}

float CAnimationInfo::GetPlayTime() const
{
	return m_PlayTime;
}

void CAnimationInfo::ResetOnce()
{
	if (m_ReverseOnce)
	{
		m_ReverseOnce = false;
		m_Reverse = false;
	}
	if (m_EndFuncOnce)
	{
		m_EndFuncOnce = false;
		DeleteEndFunction();
	}
	if (m_PlayOnce)
	{
		m_PlayOnce = false;
	}
	if (m_UseDuration)
	{
		m_UseDuration = false;
		m_Duration = -1.f;
	}
}

const std::string& CAnimationInfo::GetName()
{
	return m_Sequence->GetName();
}

void CAnimationInfo::DeleteEndFunction()
{
	m_EndFunction.Func = nullptr;
	m_EndFunction.Call = false;
}
