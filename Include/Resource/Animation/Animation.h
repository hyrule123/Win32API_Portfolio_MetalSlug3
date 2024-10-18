#pragma once

#include "../../GameInfo.h"
#include "AnimationInfo.h"

class CAnimation
{
	friend class CGameObject;
	friend class CLaser;
	friend class CLaserDebris;
	friend class CDaiManji;

private:
	CAnimation();
	CAnimation(const CAnimation& Obj);
	~CAnimation();

public:
	void Update(float DeltaTime);

private:
	class CGameObject* m_OwnerObj;
	class CScene* m_Scene;
	std::unordered_map<std::string, CAnimationInfo*> m_mapAnimationInfo;
	std::function<void()> m_CurrentAnimationEndFunc;
	bool m_CurrentAnimationNotifyInitCheck;


private:
	CAnimationInfo* m_CurrentAnimation[(int)EAnimationLayer::Max];
	//최대 레이어 번호를 저장하는 변수. 순회를 돌때 최대 레이어 번호까지만 돈다.
	int m_HighestLayer;
public:
	int GetHighestLayer() const;
	void SetHighestLayer(int Layer = 0);

public:
	Vector2 GetAnimSize(const std::string& AnimName, int Frame) const;
	Vector2 GetAnimSize(int layer = 0) const;
	Vector2 GetAnimPivot(const std::string& AnimName) const;
	Vector2 GetAnimPivot(int layer = 0) const;
	void PlayCurrentAnimAgain(int layer = 0);
	void StopAnimation(const std::string& AnimName);
	void StopAnimation(int layer = 0);

	void SyncPlayTime(int srclayer, int destlayer);

	//현재 재생중인 애니메이션의 루프여부를 true로 전환
	void SetLoopFlag(int Layer = 0, ELoopFlag LoopFlag = ELoopFlag::Loop);

	int GetCurrFrameNum(int layer = 0)	const;
	int GetCurrAnimEndFrame(int layer = 0) const;
	bool isNowEndFrame(int layer = 0) const;
	CAnimationInfo* FindAnimInfo(const std::string& AnimName);
	bool AddAnimationInfo(const std::string& AnimName,
		float PlayTime = 1.f, ELoopFlag LoopFlag = ELoopFlag::NoLoop, bool Reverse = false,
		float PlayScale = 1.f, int Layer = 0, 
		float OffsetX = 0.f, float OffsetY = 0.f,
		float PivotX = 0.f, float PivotY = 0.f);

	void SetFrame(const std::string& AnimName, int Frame);
	void SetPlayTime(const std::string& AnimName, float PlayTime);
	void SetPlayScale(const std::string& AnimName, float PlayScale);
	void SetPlayLoopFlag(const std::string& AnimName, ELoopFlag LoopFlag);
	void SetLayer(const std::string& AnimName, int Layer);
	void SetOffset(const std::string& AnimName, float OffsetX, float OffsetY);
	void SetPivot(const std::string& AnimName, float PivotX, float PivotY);

	//한번만 역재생 가능. 다른 애니메이션으로 교체되면 초기화.
	void SetPlayReverse(const std::string& AnimName, bool Reverse, bool Once);


	void SetAnimation(const std::string& AnimName, bool PlayOnce = false);
	void ChangeAnimation(const std::string& AnimName, bool PlayOnce = false);

	void SetAnimationDuration(const std::string& AnimName, float Duration);
	void ChangeAnimationDuration(const std::string& AnimName, float Duration);

	//현재 프레임 번호와 재생된 시간을 유지한채로 애니메이션 전환
	void ChangeAnimContinue(const std::string& AnimName, bool PlayOnce = false);
	bool CheckAnimationPlaying(const std::string& AnimName);




	//등록하면 모든 애니메이션 재생이 끝날 경우 공통적으로 호출된다.
	template <typename T>
	void SetCurrentAnimEndFunc(T* Obj, void(T::* Func)())
	{
		m_CurrentAnimationEndFunc = std::bind(Func, Obj);
	}


	template <typename T>
	void SetEndFunction(const std::string& AnimName,T* Obj, void (T::* Func)(), bool once = false)
	{
		CAnimationInfo* Info = FindAnimInfo(AnimName);

		if (!Info)
			return;

		Info->SetEndFunction<T>(Obj, Func);
		Info->m_EndFuncOnce = once;
	}

	template <typename T>
	void AddNotify(const std::string& AnimName, int Frame, T* Obj, void(T::* Func)())
	{
		CAnimationInfo* Info = FindAnimInfo(AnimName);

		if (!Info)
			return;

		Info->SetNotify(Frame, Obj, Func);
	}


};

