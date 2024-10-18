#include "Animation.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"
#include "../ResourceManager.h"
#include "../Texture/Texture.h"



CAnimation::CAnimation():
	m_OwnerObj(),
	m_Scene(),
	m_CurrentAnimation{},
	m_CurrentAnimationEndFunc(),
	m_CurrentAnimationNotifyInitCheck(),
	m_HighestLayer(0)
{
}

CAnimation::CAnimation(const CAnimation& Obj) :
	m_Scene(Obj.m_Scene),
	m_OwnerObj(),
	m_CurrentAnimation{},
	m_CurrentAnimationNotifyInitCheck(),
	m_HighestLayer(Obj.m_HighestLayer)
{

	auto iter = Obj.m_mapAnimationInfo.begin();
	auto iterEnd = Obj.m_mapAnimationInfo.end();
	while (iter != iterEnd)
	{
		//복사 생성자를 통해 복사
		CAnimationInfo* Info = new CAnimationInfo(*(iter->second));
		std::string Name = iter->first;
		m_mapAnimationInfo.insert(std::make_pair(Name, Info));

		++iter;
	}

	//Init() 단계에서 초기화되는 변수들
	//m_OwnerObj
	//std::function<void()> m_CurrentAnimationEndFunc;
	//CAnimationInfo* m_CurrentAnimation[(int)EAnimationLayer::Max];
	////최대 레이어 번호를 저장하는 변수. 순회를 돌때 최대 레이어 번호까지만 돈다.
	//int m_HighestLayer;
}

CAnimation::~CAnimation()
{
	auto	iter = m_mapAnimationInfo.begin();
	auto	iterEnd = m_mapAnimationInfo.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

void CAnimation::Update(float DeltaTime)
{
	for (int i = 0; i <= (int)m_HighestLayer; ++i)
	{
		//해당 레이어의 애니메이션이 존재하지 않으면 continue
		if (!m_CurrentAnimation[i])
			continue;

		CAnimationInfo* Info = m_CurrentAnimation[i];


		//시간 먼저 계산
		Info->m_Time += DeltaTime * Info->m_PlayScale;

		//duration을 사용하는지 계산
		if (Info->m_UseDuration)
		{
			Info->m_Duration -= DeltaTime;
			if (Info->m_Duration <= 0.f)
			{
				Info->m_UseDuration = false;
				StopAnimation(i);
			}
		}

		bool AnimationEnd = false;

		//한 프레임이 넘어갈 시간이 지났으면
		if (Info->m_Time > Info->m_FrameTime)
		{
				
			//시간을 도로 초기화하고
			Info->m_Time = 0.f;

			//프레임을 교체해준다.(역재생인지 확인하고)
			if (Info->m_Reverse)
			{
				--Info->m_Frame;

				//마지막 프레임인지도 체크
				if (Info->m_Frame < 0)
				{
					Info->m_Frame = 0;
					AnimationEnd = true;
				}
			}
			else
			{
				++Info->m_Frame;


				//정재생일 경우는 마지막 프레임 번호가 전체 프레임 장수에 도달하면
				//cf) 마지막 프레임 번호(배열의 index) + 1  == 전체 프레임 장수
				//또는 EndFunction이 호출되었다면 마지막 프레임에 도달한것이다.
				if (Info->m_Frame == Info->m_Sequence->GetFrameCount())
				{
					Info->m_Frame = (int)Info->m_Sequence->GetFrameCount() - 1;
					AnimationEnd = true;
				}

				
			}
		}
		else if (Info->m_EndFunction.Call)
			AnimationEnd = true;


		//Notify 체크 부분
		//현재 프레임을 얻어온다.
		int index = Info->m_Frame;

		//현재 프레임 번호에 해당하는 노티파이는 노티파이 배열의 인덱스에 있는 함수와 동일하다.
		//만약 이 배열이 비어있다면 스킵해도 됨
		if (!Info->m_vecNotify.empty())
		{


			if (!Info->m_vecNotify[index].empty())
			{
				//노티파이가 있을 경우 해당 노티파이 배열을 순회하며 전부 실행시킨다.
				size_t size = Info->m_vecNotify[index].size();
				for (size_t i = 0; i < size; ++i)
				{
					if (!Info->m_vecNotify[index][i]->Call)
					{
						Info->m_vecNotify[index][i]->Func();
				
						//실행시키고 call도 true로 변경한다.
						Info->m_vecNotify[index][i]->Call = true;
					}
				}
			}

		}


			
			
			
			
			


		//애니메이션 끝 처리 부분
		if (AnimationEnd)
		{
			if (m_CurrentAnimationEndFunc)
			{
				m_CurrentAnimationEndFunc();
			}

			switch (Info->m_LoopFlag)
			{
			case ELoopFlag::NoLoop:
				//역재생이면 마지막 프레임인 0번 프레임에서 스탑
				if (Info->m_Reverse)
				{
					Info->m_Frame = 0;
				}

				//정재생이면 마지막 프레임인 전체 프레임 장수 -1에서 스탑
				else
				{
					Info->m_Frame = (int)Info->m_Sequence->GetFrameCount() - 1;
				}

				//여긴 애니메이션 재생이 끝나면 계속 마지막 프레임에 있기 때문에
				//계속 진입하게 되므로
				//Call 변수를 통해서 한번만 실행한다.
				if ((!Info->m_EndFunction.Call) && Info->m_EndFunction.Func)
				{
					Info->m_EndFunction.Func();
					Info->m_EndFunction.Call = true;
					if (Info->m_EndFuncOnce)
					{
						Info->m_EndFuncOnce = false;
						Info->DeleteEndFunction();
					}
				}
				break;
			case ELoopFlag::Loop:
				//거기에 역재생이면
				if (Info->m_Reverse)
				{
					//다시 마지막 프레임으로 이동시켜 역재생을 시작한다.
					Info->m_Frame = (int)Info->m_Sequence->GetFrameCount() - 1;
				}
				//정재생이면 첫 프레임(0)으로 이동해서 재생을 시작한다.
				else
				{
					Info->m_Frame = 0;
				}

				//EndFunc를 실행
				//여긴 어차피 한 프레임만 들어오고 반복되지 않으므로 그냥 실행해주면 된다.
				if (Info->m_EndFunction.Func)
					Info->m_EndFunction.Func();
				break;
			case ELoopFlag::LoopRound://지그재그형식으로 프레임을 왕복
				if (Info->m_Reverse)
				{
					//역재생을 끝낸다
					Info->m_Reverse = false;
					++Info->m_Frame;
				}

				else
				{
					//역재생을 시작한다.
					Info->m_Reverse = true;
					--Info->m_Frame;
				}

				//EndFunc를 실행
				//여긴 어차피 한 프레임만 들어오고 반복되지 않으므로 그냥 실행해주면 된다.
				if (Info->m_EndFunction.Func)
				{
					Info->m_EndFunction.Call = true;
					Info->m_EndFunction.Func();
				}
					
				break;
			}


			//아직 노티파이 초기화를 하지않았다면 1회 초기화
			if (!m_CurrentAnimationNotifyInitCheck)
			{
				//끝에 도달했음이 확인되면 노티파이를 전부 false로 바꿔준다.
				size_t sizeR = Info->m_vecNotify.size();
				for (size_t i = 0; i < sizeR; ++i)
				{
					if (Info->m_vecNotify[i].empty())
						continue;

					size_t sizeC = Info->m_vecNotify[i].size();
					for (size_t j = 0; j < sizeC; ++j)
					{
						Info->m_vecNotify[i][j]->Call = false;
					}
				}
				m_CurrentAnimationNotifyInitCheck = true;
			}
	

			//한번만 재생하도록 설정했을 경우 처리
			if (Info->m_PlayOnce)
			{
				Info->m_PlayOnce = false;
				StopAnimation(i);
			}
		}
		else
		{
			//마지막 프레임이 아니라면 EndFunction의 call 여부를 false로 전환
			Info->m_EndFunction.Call = false;
			m_CurrentAnimationNotifyInitCheck = false;
		}

	}
}



int CAnimation::GetHighestLayer() const
{
	return m_HighestLayer;
}

void CAnimation::SetHighestLayer(int Layer)
{
	if (m_HighestLayer < Layer)
		m_HighestLayer = Layer;
}




Vector2 CAnimation::GetAnimSize(const std::string& AnimName, int Frame) const
{
	auto iter = m_mapAnimationInfo.find(AnimName);

	if (iter == m_mapAnimationInfo.end())
		return Vector2(0, 0);
	
	if (Frame >= iter->second->m_Sequence->GetFrameCount())
		return Vector2(0, 0);

	const Sprite_FrameData data = iter->second->m_Sequence->GetFrameData(Frame);

	return (data.End - data.Start);
}

Vector2 CAnimation::GetAnimSize(int layer) const
{
	if (!m_CurrentAnimation[layer])
		return Vector2(0, 0);	//이상한 값을 반환해줘서 에러있다고 알려줌

	int CurrentFrame = m_CurrentAnimation[layer]->m_Frame;

	const Sprite_FrameData data = m_CurrentAnimation[layer]->m_Sequence->GetFrameData(CurrentFrame);

	return (data.End - data.Start);
}

Vector2 CAnimation::GetAnimPivot(const std::string& AnimName) const
{

	auto iter = m_mapAnimationInfo.find(AnimName);

	if (iter == m_mapAnimationInfo.end())
		return Vector2(0, 0);


	return iter->second->m_Pivot;
}

Vector2 CAnimation::GetAnimPivot(int layer) const
{
	if (!m_CurrentAnimation[layer])
		return Vector2(0, 0);

	
	return m_CurrentAnimation[layer]->m_Pivot;
}

void CAnimation::PlayCurrentAnimAgain(int layer)
{
	if(m_CurrentAnimation[layer])
		m_CurrentAnimation[layer]->Init();
}

void CAnimation::StopAnimation(const std::string& AnimName)
{
	auto iter = m_mapAnimationInfo.find(AnimName);

	if (iter == m_mapAnimationInfo.end())
		return;

	if (CheckAnimationPlaying(AnimName))
	{
		if (m_CurrentAnimation[iter->second->m_Layer])
			m_CurrentAnimation[iter->second->m_Layer]->ResetOnce();
		m_CurrentAnimation[iter->second->m_Layer] = nullptr;
	}
}

void CAnimation::StopAnimation(int layer)
{
	if (m_CurrentAnimation[layer])
		m_CurrentAnimation[layer]->ResetOnce();

	m_CurrentAnimation[layer] = nullptr;
}

void CAnimation::SyncPlayTime(int srclayer, int destlayer)
{
	if (srclayer == destlayer)
		return;
	else if (!m_CurrentAnimation[srclayer]
		|| !m_CurrentAnimation[destlayer])
		return;
	else if (m_CurrentAnimation[srclayer]->m_Sequence->GetFrameCount() != m_CurrentAnimation[destlayer]->m_Sequence->GetFrameCount())
		return;

	m_CurrentAnimation[destlayer]->m_Frame = m_CurrentAnimation[srclayer]->m_Frame;
	m_CurrentAnimation[destlayer]->m_Time = m_CurrentAnimation[srclayer]->m_Time;

}







void CAnimation::SetLoopFlag(int Layer, ELoopFlag LoopFlag)
{
	if (m_CurrentAnimation[Layer])
	{
		m_CurrentAnimation[Layer]->m_LoopFlag = LoopFlag;
	}
}

int CAnimation::GetCurrFrameNum(int layer) const
{
	if(m_CurrentAnimation[layer])
		return m_CurrentAnimation[layer]->m_Frame;

	return 0;
}

int CAnimation::GetCurrAnimEndFrame(int layer) const
{
	if(m_CurrentAnimation[layer])
		return (int)m_CurrentAnimation[layer]->m_Sequence->GetFrameCount() - 1;

	return 0;
}

bool CAnimation::isNowEndFrame(int layer) const
{
	if(m_CurrentAnimation[layer])
		return m_CurrentAnimation[layer]->m_Frame == (m_CurrentAnimation[layer]->m_Sequence->GetFrameCount() - 1);

	return false;
}



CAnimationInfo* CAnimation::FindAnimInfo(const std::string& AnimName)
{
	auto iter = m_mapAnimationInfo.find(AnimName);

	if (iter == m_mapAnimationInfo.end())
		return nullptr;


	return iter->second;
}


bool CAnimation::AddAnimationInfo(const std::string& AnimName,  float PlayTime, ELoopFlag LoopFlag, bool Reverse, float PlayScale, int Layer, float OffsetX, float OffsetY, float PivotX, float PivotY)
{
	//최대 레이어 수보다 많은 위치를 레이어 인자로 넣으면 그냥 return
	if (Layer >= (int)EAnimationLayer::Max || Layer < 0)
		return false;
	
	//만약 최대 레이어 수를 넘어가는 레이어 번호가 등록되었을 경우 해당 레이어를 최고 번호로 바꿔줌.
	if (m_HighestLayer < Layer)
		m_HighestLayer = Layer;


	CAnimationInfo* AnimInfo = FindAnimInfo(AnimName);

	//혹시나 기존에 만들어진 애니메이션 정보가 있다면 그걸 쓰면 되므로 true 리턴
	if (AnimInfo)
		return true;

	//일단 AnimInfo에 등록할 시퀀스부터 찾아본다.
	CAnimationSequence* AnimSeq = nullptr;

	//만약 Scene이 등록되어 있으면 SceneResouce를 통해 찾는다.
	if (m_Scene)
	{
		AnimSeq = m_Scene->GetSceneResource()->FindAnimationSequence(AnimName);
	}
	else
	{
		AnimSeq = CResourceManager::GetInst()->FindAnimationSequence(AnimName);
	}

	//애니메이션 시퀀스 생성에 실패할 경우 리턴
	if (!AnimSeq)
		return false;

	//조건을 모두 만족했으면 동적할당하고 데이터를 옮겨넣는다.
	AnimInfo = new CAnimationInfo;

	AnimInfo->m_Sequence = AnimSeq;
	AnimInfo->m_LoopFlag = LoopFlag;
	AnimInfo->m_PlayTime = PlayTime;
	AnimInfo->m_PlayScale = PlayScale;
	AnimInfo->m_Reverse = Reverse;
	AnimInfo->m_Layer = Layer;
	AnimInfo->m_Offset = Vector2(OffsetX, OffsetY);
	AnimInfo->m_Pivot = Vector2(PivotX, PivotY);

	//프레임타임은 직접 계산해서 대입한다.
	AnimInfo->m_FrameTime = AnimInfo->m_PlayTime / AnimInfo->m_Sequence->GetFrameCount();

	if (m_mapAnimationInfo.empty())
		m_CurrentAnimation[Layer] = AnimInfo;


	//모두 완료됐으면 vector에 삽입한다.
	m_mapAnimationInfo.insert(std::make_pair(AnimName, AnimInfo));

	return true;
}

void CAnimation::SetFrame(const std::string& AnimName, int Frame)
{
	CAnimationInfo* Info = FindAnimInfo(AnimName);

	if (!Info)
		return;

	if (Info->m_Sequence->GetFrameCount() <= Frame)
		return;

	Info->m_Frame = Frame;
}

void CAnimation::SetPlayTime(const std::string& AnimName, float PlayTime)
{
	CAnimationInfo* Info = FindAnimInfo(AnimName);

	if (!Info)
		return;

	Info->m_PlayTime = PlayTime;
	Info->m_FrameTime = PlayTime / Info->m_Sequence->GetFrameCount();
}

void CAnimation::SetPlayScale(const std::string& AnimName, float PlayScale)
{
	CAnimationInfo* Info = FindAnimInfo(AnimName);

	if (!Info)
		return;

	Info->m_PlayScale = PlayScale;
}

void CAnimation::SetPlayLoopFlag(const std::string& AnimName, ELoopFlag LoopFlag)
{
	CAnimationInfo* Info = FindAnimInfo(AnimName);

	if (!Info)
		return;

	Info->m_LoopFlag = LoopFlag;
}

void CAnimation::SetLayer(const std::string& AnimName, int Layer)
{
	if (Layer < 0 || Layer >= (int)ERenderLayer::Max)
		return;

	CAnimationInfo* Info = FindAnimInfo(AnimName);

	if (!Info)
		return;

	//만약 현재 재생중이었다면 재생을 중지하고 바꿔준다.
	if (m_CurrentAnimation[Info->m_Layer] == Info)
		StopAnimation(Info->m_Layer);

	//최대 레이어 수가 늘어났는지 확인하고
	SetHighestLayer(Layer);

	//값을 바꿔준다.
	Info->m_Layer = Layer;
	
}

void CAnimation::SetOffset(const std::string& AnimName, float OffsetX, float OffsetY)
{
	CAnimationInfo* Info = FindAnimInfo(AnimName);

	if (!Info)
		return;

	Info->m_Offset.x = OffsetX;
	Info->m_Offset.y = OffsetY;
}

void CAnimation::SetPivot(const std::string& AnimName, float PivotX, float PivotY)
{
	CAnimationInfo* Info = FindAnimInfo(AnimName);

	if (!Info)
		return;

	Info->m_Pivot.x = PivotX;
	Info->m_Pivot.y = PivotY;
}

void CAnimation::SetPlayReverse(const std::string& AnimName, bool Reverse, bool Once)
{
	CAnimationInfo* Info = FindAnimInfo(AnimName);

	if (!Info)
		return;

	Info->m_Reverse = Reverse;
	Info->m_ReverseOnce = Once;
}

void CAnimation::SetAnimation(const std::string& AnimName, bool PlayOnce)
{
	CAnimationInfo* Info = FindAnimInfo(AnimName);


	if (!Info)
		return;


	int layer = Info->m_Layer;

	if (m_CurrentAnimation[layer])
		m_CurrentAnimation[layer]->ResetOnce();

	Info->m_PlayOnce = PlayOnce;

	m_CurrentAnimation[layer] = Info;
	m_CurrentAnimation[layer]->Init();
}


//선생님 코드와 다르게 짰음
void CAnimation::ChangeAnimation(const std::string& AnimName, bool PlayOnce)
{
	CAnimationInfo* Info = FindAnimInfo(AnimName);

	if (!Info)
		return;

	//만약 현재 재생중인 애니메이션과 같은 애니메이션일 경우 return
	else if (m_CurrentAnimation[Info->m_Layer] == Info)
		return;

	int layer = Info->m_Layer;

	if(m_CurrentAnimation[layer])
		m_CurrentAnimation[layer]->ResetOnce();

	m_CurrentAnimation[layer] = Info;
	m_CurrentAnimation[layer]->m_PlayOnce = PlayOnce;
	m_CurrentAnimation[layer]->Init();
}

void CAnimation::SetAnimationDuration(const std::string& AnimName, float Duration)
{
	CAnimationInfo* Info = FindAnimInfo(AnimName);


	if (!Info)
		return;


	int layer = Info->m_Layer;

	if (m_CurrentAnimation[layer])
		m_CurrentAnimation[layer]->ResetOnce();

	Info->Init();
	Info->m_UseDuration = true;
	Info->m_Duration = Duration;

	m_CurrentAnimation[layer] = Info;
}

void CAnimation::ChangeAnimationDuration(const std::string& AnimName, float Duration)
{
	CAnimationInfo* Info = FindAnimInfo(AnimName);

	if (!Info)
		return;


	//만약 현재 재생중인 애니메이션과 같은 애니메이션일 경우 duration을 설정하고 return
	else if (m_CurrentAnimation[Info->m_Layer] == Info)
	{
		m_CurrentAnimation[Info->m_Layer]->m_UseDuration = true;
		m_CurrentAnimation[Info->m_Layer]->m_Duration = Duration;
		
		return;
	}
		

	int layer = Info->m_Layer;

	if (m_CurrentAnimation[layer])
		m_CurrentAnimation[layer]->ResetOnce();

	Info->Init();
	Info->m_UseDuration = true;
	Info->m_Duration = Duration;

	m_CurrentAnimation[layer] = Info;
}




void CAnimation::ChangeAnimContinue(const std::string& AnimName, bool PlayOnce)
{
	CAnimationInfo* Info = FindAnimInfo(AnimName);

	if (!Info)
		return;


	int layer = Info->m_Layer;



	if (m_CurrentAnimation[layer])
	{
		//만약 현재 재생중인 애니메이션과 같은 애니메이션일 경우 return
		if(m_CurrentAnimation[layer] == Info)
			return;

		//바꿀 Info를 미리 초기화해놓고
		Info->Init();

		//1회용 지정 기능을 초기화.
		m_CurrentAnimation[layer]->ResetOnce();

		if (Info->m_Sequence->GetFrameCount() > m_CurrentAnimation[layer]->m_Frame)
		{
			//기존 재생 중이던 애니메이션의 프레임 번호와 재생중이던 시간을 복사
			Info->m_Frame = m_CurrentAnimation[layer]->m_Frame;
			Info->m_Time = m_CurrentAnimation[layer]->m_Time;
		}

	}
	else
	{
		//바꿀 Info를 미리 초기화만 해놓고 재생
		Info->Init();
	}
	Info->m_PlayOnce = PlayOnce;
	m_CurrentAnimation[layer] = Info;
}


bool CAnimation::CheckAnimationPlaying(const std::string& AnimName)
{
	CAnimationInfo* Info = FindAnimInfo(AnimName);
	return m_CurrentAnimation[Info->m_Layer] == Info;
}
