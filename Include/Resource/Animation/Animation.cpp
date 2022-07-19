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
		//���� �����ڸ� ���� ����
		CAnimationInfo* Info = new CAnimationInfo(*(iter->second));
		std::string Name = iter->first;
		m_mapAnimationInfo.insert(std::make_pair(Name, Info));

		++iter;
	}

	//Init() �ܰ迡�� �ʱ�ȭ�Ǵ� ������
	//m_OwnerObj
	//std::function<void()> m_CurrentAnimationEndFunc;
	//CAnimationInfo* m_CurrentAnimation[(int)EAnimationLayer::Max];
	////�ִ� ���̾� ��ȣ�� �����ϴ� ����. ��ȸ�� ���� �ִ� ���̾� ��ȣ������ ����.
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
		//�ش� ���̾��� �ִϸ��̼��� �������� ������ continue
		if (!m_CurrentAnimation[i])
			continue;

		CAnimationInfo* Info = m_CurrentAnimation[i];


		//�ð� ���� ���
		Info->m_Time += DeltaTime * Info->m_PlayScale;

		//duration�� ����ϴ��� ���
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

		//�� �������� �Ѿ �ð��� ��������
		if (Info->m_Time > Info->m_FrameTime)
		{
				
			//�ð��� ���� �ʱ�ȭ�ϰ�
			Info->m_Time = 0.f;

			//�������� ��ü���ش�.(��������� Ȯ���ϰ�)
			if (Info->m_Reverse)
			{
				--Info->m_Frame;

				//������ ������������ üũ
				if (Info->m_Frame < 0)
				{
					Info->m_Frame = 0;
					AnimationEnd = true;
				}
			}
			else
			{
				++Info->m_Frame;


				//������� ���� ������ ������ ��ȣ�� ��ü ������ ����� �����ϸ�
				//cf) ������ ������ ��ȣ(�迭�� index) + 1  == ��ü ������ ���
				//�Ǵ� EndFunction�� ȣ��Ǿ��ٸ� ������ �����ӿ� �����Ѱ��̴�.
				if (Info->m_Frame == Info->m_Sequence->GetFrameCount())
				{
					Info->m_Frame = (int)Info->m_Sequence->GetFrameCount() - 1;
					AnimationEnd = true;
				}

				
			}
		}
		else if (Info->m_EndFunction.Call)
			AnimationEnd = true;


		//Notify üũ �κ�
		//���� �������� ���´�.
		int index = Info->m_Frame;

		//���� ������ ��ȣ�� �ش��ϴ� ��Ƽ���̴� ��Ƽ���� �迭�� �ε����� �ִ� �Լ��� �����ϴ�.
		//���� �� �迭�� ����ִٸ� ��ŵ�ص� ��
		if (!Info->m_vecNotify.empty())
		{


			if (!Info->m_vecNotify[index].empty())
			{
				//��Ƽ���̰� ���� ��� �ش� ��Ƽ���� �迭�� ��ȸ�ϸ� ���� �����Ų��.
				size_t size = Info->m_vecNotify[index].size();
				for (size_t i = 0; i < size; ++i)
				{
					if (!Info->m_vecNotify[index][i]->Call)
					{
						Info->m_vecNotify[index][i]->Func();
				
						//�����Ű�� call�� true�� �����Ѵ�.
						Info->m_vecNotify[index][i]->Call = true;
					}
				}
			}

		}


			
			
			
			
			


		//�ִϸ��̼� �� ó�� �κ�
		if (AnimationEnd)
		{
			if (m_CurrentAnimationEndFunc)
			{
				m_CurrentAnimationEndFunc();
			}

			switch (Info->m_LoopFlag)
			{
			case ELoopFlag::NoLoop:
				//������̸� ������ �������� 0�� �����ӿ��� ��ž
				if (Info->m_Reverse)
				{
					Info->m_Frame = 0;
				}

				//������̸� ������ �������� ��ü ������ ��� -1���� ��ž
				else
				{
					Info->m_Frame = (int)Info->m_Sequence->GetFrameCount() - 1;
				}

				//���� �ִϸ��̼� ����� ������ ��� ������ �����ӿ� �ֱ� ������
				//��� �����ϰ� �ǹǷ�
				//Call ������ ���ؼ� �ѹ��� �����Ѵ�.
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
				//�ű⿡ ������̸�
				if (Info->m_Reverse)
				{
					//�ٽ� ������ ���������� �̵����� ������� �����Ѵ�.
					Info->m_Frame = (int)Info->m_Sequence->GetFrameCount() - 1;
				}
				//������̸� ù ������(0)���� �̵��ؼ� ����� �����Ѵ�.
				else
				{
					Info->m_Frame = 0;
				}

				//EndFunc�� ����
				//���� ������ �� �����Ӹ� ������ �ݺ����� �����Ƿ� �׳� �������ָ� �ȴ�.
				if (Info->m_EndFunction.Func)
					Info->m_EndFunction.Func();
				break;
			case ELoopFlag::LoopRound://��������������� �������� �պ�
				if (Info->m_Reverse)
				{
					//������� ������
					Info->m_Reverse = false;
					++Info->m_Frame;
				}

				else
				{
					//������� �����Ѵ�.
					Info->m_Reverse = true;
					--Info->m_Frame;
				}

				//EndFunc�� ����
				//���� ������ �� �����Ӹ� ������ �ݺ����� �����Ƿ� �׳� �������ָ� �ȴ�.
				if (Info->m_EndFunction.Func)
				{
					Info->m_EndFunction.Call = true;
					Info->m_EndFunction.Func();
				}
					
				break;
			}


			//���� ��Ƽ���� �ʱ�ȭ�� �����ʾҴٸ� 1ȸ �ʱ�ȭ
			if (!m_CurrentAnimationNotifyInitCheck)
			{
				//���� ���������� Ȯ�εǸ� ��Ƽ���̸� ���� false�� �ٲ��ش�.
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
	

			//�ѹ��� ����ϵ��� �������� ��� ó��
			if (Info->m_PlayOnce)
			{
				Info->m_PlayOnce = false;
				StopAnimation(i);
			}
		}
		else
		{
			//������ �������� �ƴ϶�� EndFunction�� call ���θ� false�� ��ȯ
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
		return Vector2(0, 0);	//�̻��� ���� ��ȯ���༭ �����ִٰ� �˷���

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
	//�ִ� ���̾� ������ ���� ��ġ�� ���̾� ���ڷ� ������ �׳� return
	if (Layer >= (int)EAnimationLayer::Max || Layer < 0)
		return false;
	
	//���� �ִ� ���̾� ���� �Ѿ�� ���̾� ��ȣ�� ��ϵǾ��� ��� �ش� ���̾ �ְ� ��ȣ�� �ٲ���.
	if (m_HighestLayer < Layer)
		m_HighestLayer = Layer;


	CAnimationInfo* AnimInfo = FindAnimInfo(AnimName);

	//Ȥ�ó� ������ ������� �ִϸ��̼� ������ �ִٸ� �װ� ���� �ǹǷ� true ����
	if (AnimInfo)
		return true;

	//�ϴ� AnimInfo�� ����� ���������� ã�ƺ���.
	CAnimationSequence* AnimSeq = nullptr;

	//���� Scene�� ��ϵǾ� ������ SceneResouce�� ���� ã�´�.
	if (m_Scene)
	{
		AnimSeq = m_Scene->GetSceneResource()->FindAnimationSequence(AnimName);
	}
	else
	{
		AnimSeq = CResourceManager::GetInst()->FindAnimationSequence(AnimName);
	}

	//�ִϸ��̼� ������ ������ ������ ��� ����
	if (!AnimSeq)
		return false;

	//������ ��� ���������� �����Ҵ��ϰ� �����͸� �Űִܳ´�.
	AnimInfo = new CAnimationInfo;

	AnimInfo->m_Sequence = AnimSeq;
	AnimInfo->m_LoopFlag = LoopFlag;
	AnimInfo->m_PlayTime = PlayTime;
	AnimInfo->m_PlayScale = PlayScale;
	AnimInfo->m_Reverse = Reverse;
	AnimInfo->m_Layer = Layer;
	AnimInfo->m_Offset = Vector2(OffsetX, OffsetY);
	AnimInfo->m_Pivot = Vector2(PivotX, PivotY);

	//������Ÿ���� ���� ����ؼ� �����Ѵ�.
	AnimInfo->m_FrameTime = AnimInfo->m_PlayTime / AnimInfo->m_Sequence->GetFrameCount();

	if (m_mapAnimationInfo.empty())
		m_CurrentAnimation[Layer] = AnimInfo;


	//��� �Ϸ������ vector�� �����Ѵ�.
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

	//���� ���� ������̾��ٸ� ����� �����ϰ� �ٲ��ش�.
	if (m_CurrentAnimation[Info->m_Layer] == Info)
		StopAnimation(Info->m_Layer);

	//�ִ� ���̾� ���� �þ���� Ȯ���ϰ�
	SetHighestLayer(Layer);

	//���� �ٲ��ش�.
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


//������ �ڵ�� �ٸ��� ®��
void CAnimation::ChangeAnimation(const std::string& AnimName, bool PlayOnce)
{
	CAnimationInfo* Info = FindAnimInfo(AnimName);

	if (!Info)
		return;

	//���� ���� ������� �ִϸ��̼ǰ� ���� �ִϸ��̼��� ��� return
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


	//���� ���� ������� �ִϸ��̼ǰ� ���� �ִϸ��̼��� ��� duration�� �����ϰ� return
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
		//���� ���� ������� �ִϸ��̼ǰ� ���� �ִϸ��̼��� ��� return
		if(m_CurrentAnimation[layer] == Info)
			return;

		//�ٲ� Info�� �̸� �ʱ�ȭ�س���
		Info->Init();

		//1ȸ�� ���� ����� �ʱ�ȭ.
		m_CurrentAnimation[layer]->ResetOnce();

		if (Info->m_Sequence->GetFrameCount() > m_CurrentAnimation[layer]->m_Frame)
		{
			//���� ��� ���̴� �ִϸ��̼��� ������ ��ȣ�� ������̴� �ð��� ����
			Info->m_Frame = m_CurrentAnimation[layer]->m_Frame;
			Info->m_Time = m_CurrentAnimation[layer]->m_Time;
		}

	}
	else
	{
		//�ٲ� Info�� �̸� �ʱ�ȭ�� �س��� ���
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
