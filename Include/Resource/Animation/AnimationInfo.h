#pragma once

#include "../../GameInfo.h"
#include "AnimationSequence.h"

struct AnimationNotify
{
	

	//����ü�� Frame ���� �����ϴ� ���,
	//Notify�� ȣ���� Frame ��ȣ�� vector ��ȣ�� ��ġ�ϰ� �����Ϸ��� ��
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

public://�� �ִϸ��̼��� ó�� ����ϴ� ���·� �����ϴ� �޼ҵ�
	void Init();

private:
	//�� Ŭ������ ����ִ� �ִϸ��̼� �������� �ּ�
	CSharedPtr<CAnimationSequence> m_Sequence; //0 = �Ϲ�ä��, 1 = ��Ʈ����Ʈä��

	//�󸶳� �������� ����� ������?
	Vector2 m_Offset;

	//�ִϸ��̼� ��ü�� �ǹ� ��
	Vector2 m_Pivot;

	//�� ��° ���̾�� ����Ǿ�� �ϴ���?
	int m_Layer;
	
	//���� �� �������� ����ǰ� �ִ���?
	int m_Frame;

	//�������� �ٲ�� ���� �귶����(���� ������ ��ü���� �󸶳� ���Ҵ���)
	float m_Time;
	
	
	//�� �����ӿ� �� �� �ɸ�������
	float m_FrameTime;

	//�� ��� �ð�
	float m_PlayTime;

	//��� �ӵ�
	//ex)���ο� ������ ���� �������� �׿� ���� ������ ��ȯ �ӵ��� ����)
	float m_PlayScale;

	//������ �ݺ���� �� ������ ����
	ELoopFlag m_LoopFlag;
	//������ CurrentAnimation ��Ͽ��� �ƿ� ������ 
	//����� ������ ������ ����
	bool	m_PlayOnce;
	bool	m_UseDuration;
	float	m_Duration;

	//����� ����
	//Temp: �ѹ��� �����. ChangeAnimation �Ǵ� ����� ������ ��� Reverse ����
	bool m_Reverse;
	bool m_ReverseOnce;

	//����� ����Ǿ��� ��� ȣ���� �޼ҵ�
	//������ �ƴ� ��� �� �����ӿ��� �����Ǵµ�,
	//�ش� ���¿��� �ѹ��� ȣ���� �ɼ� �ֵ��� AnimationNotify ����ü�� ����Ѵ�.
	AnimationNotify m_EndFunction;
	//Temp: �ѹ��� EnFunction ���. ChangeAnimation �Ǵ� EndFunction�� ����Ǹ�
	//Endfunction ����.
	bool m_EndFuncOnce;

	//�����Ҵ�� ��Ƽ���� ����ü�� ��Ƽ� ���� - ������ �迭 ����
	//[i][j] i-> ������ ��ȣ, j->�ش� �����ӿ��� ����� Notify �Լ� ��ȣ
	std::vector<std::vector<AnimationNotify*>> m_vecNotify;


public:

	bool GetReverse() const;
	float GetPlayTime() const;
	//�ѹ��� �۵��ϴ� ��ɵ��� �ʱ�ȭ(Reverse, EndFunction)
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

		//���� ���
		//Notify->Frame = Frame;

		Notify->Func = std::bind(Func, Obj);

		//������ ���� ���� vector ����� �̸� Ȯ��
		m_vecNotify.resize(m_Sequence->GetFrameCount());

		if (Frame >= m_vecNotify.size())
			return;

		//notyfy�� ȣ���ϰ� ���� Frame ��ȣ�� ���� index�� notify�� ����.
		m_vecNotify[Frame].push_back(Notify);
	}

};

