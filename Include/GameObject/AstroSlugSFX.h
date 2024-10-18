#pragma once
#include "Effect.h"


//사용법: 따라다녀야 할 경우 주인 오브젝트를 넣고 생성
//일반적인 경우 그냥 생성
class CAstroSlugSFX :
	public CEffect
{
	friend class CScene;
protected:
	CAstroSlugSFX();
	CAstroSlugSFX(const CAstroSlugSFX& Obj);
	virtual ~CAstroSlugSFX();
public:
	virtual bool LoadResource();
	virtual void Start();
	virtual bool Init(CGameObject* Obj = nullptr);
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
	bool SetSFX(const std::string& Name);


public:
	//아스트로 슬러그에서 접근하는 함수. 테일 부스트 꺼질떄 역재생용
	void TailBoost1ReverseSet();

private:
	bool m_isTailSFX;
	Vector2 m_TailSFXDir;
	void Booster1EndFunc();
	void TailLeftEnd();
	void TailRightEnd();
	void TailStraightEnd();
	void TailIdleEnd();
	void CreateTailSFX();
	void TailBoost1ReverseEnd();



};

