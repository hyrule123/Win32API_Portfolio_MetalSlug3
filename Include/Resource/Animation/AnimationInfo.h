#pragma once

#include "../../GameInfo.h"
#include "AnimationSequence.h"

struct AnimationNotify
{
	

	//구조체에 Frame 값을 저장하는 대신,
	//Notify를 호출할 Frame 번호를 vector 번호와 일치하게 설정하려고 함
	//int Frame;
	bool Call;
	std::function<void()> Func;

	AnimationNotify() :
		Call(false),
		Func(nullptr)
	{
	}


	//AnimationNotify():
	//	Frame(0),
	//	Call(false)
	//{
	//}

};


class CAnimationInfo
{
	friend class CAnimation;
	friend class CGameObject;
	friend class CLaser;
	friend class CLaserDebris;
	friend class CEditViewer;
	friend class CDaiManji;

private:
	CAnimationInfo();
	CAnimationInfo(const CAnimationInfo& Obj);
	~CAnimationInfo();

public://이 애니메이션을 처음 재생하는 상태로 복구하는 메소드
	void Init();

private:
	//이 클래스가 들고있는 애니메이션 시퀀스의 주소
	CSharedPtr<CAnimationSequence> m_Sequence; //0 = 일반채널, 1 = 히트이펙트채널

	//얼마나 떨어져서 재생될 것인지?
	Vector2 m_Offset;

	//애니메이션 자체의 피벗 값
	Vector2 m_Pivot;

	//몇 번째 레이어에서 재생되어야 하는지?
	int m_Layer;
	
	//지금 몇 프레임이 재생되고 있는지?
	int m_Frame;

	//프레임이 바뀌고 몇초 흘렀는지(다음 프레임 교체까지 얼마나 남았는지)
	float m_Time;
	
	
	//한 프레임에 몇 초 걸릴것인지
	float m_FrameTime;

	//총 재생 시간
	float m_PlayTime;

	//재생 속도
	//ex)슬로우 등으로 인해 느려지면 그에 따라 프레임 전환 속도도 감소)
	float m_PlayScale;

	//끝나도 반복재생 할 것인지 여부
	ELoopFlag m_LoopFlag;
	//끝나면 CurrentAnimation 목록에서 아예 나가서 
	//재생을 중지할 것인지 여부
	bool	m_PlayOnce;
	bool	m_UseDuration;
	float	m_Duration;

	//역재생 여부
	//Temp: 한번만 역재생. ChangeAnimation 또는 재생이 끝났을 경우 Reverse 해제
	bool m_Reverse;
	bool m_ReverseOnce;

	//재생이 종료되었을 경우 호출할 메소드
	//루프가 아닐 경우 끝 프레임에서 고정되는데,
	//해당 상태에서 한번만 호출이 될수 있도록 AnimationNotify 구조체를 사용한다.
	AnimationNotify m_EndFunction;
	//Temp: 한번만 EnFunction 등록. ChangeAnimation 또는 EndFunction이 실행되면
	//Endfunction 해제.
	bool m_EndFuncOnce;

	//동적할당된 노티파이 구조체를 모아서 보관 - 이차원 배열 구조
	//[i][j] i-> 프레임 번호, j->해당 프레임에서 재생할 Notify 함수 번호
	std::vector<std::vector<AnimationNotify*>> m_vecNotify;


public:

	bool GetReverse() const;
	float GetPlayTime() const;
	//한번만 작동하는 기능들을 초기화(Reverse, EndFunction)
	void ResetOnce();

	const std::string& GetName();

	template <typename T>
	void SetEndFunction(T* Obj, void (T::* Func)())
	{
		m_EndFunction.Func = std::bind(Func, Obj);
	}

	void DeleteEndFunction();


	template <typename T>
	void SetNotify(int Frame, T* Obj, void(T::* Func)())
	{
		AnimationNotify* Notify = new AnimationNotify;

		//기존 방식
		//Notify->Frame = Frame;

		Notify->Func = std::bind(Func, Obj);

		//프레임 수와 같게 vector 사이즈를 미리 확보
		m_vecNotify.resize(m_Sequence->GetFrameCount());

		if (Frame >= m_vecNotify.size())
			return;

		//notyfy를 호출하고 싶은 Frame 번호와 같은 index에 notify를 삽입.
		m_vecNotify[Frame].push_back(Notify);
	}

};

