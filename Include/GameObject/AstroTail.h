#pragma once
#include "JustMovingSFX.h"

//AstroSlug 전용 클래스. 다른 클래스 사용 X
class CAstroTail :
	public CJustMovingSFX
{
	friend class CScene;

protected:
	CAstroTail();
	CAstroTail(const CAstroTail& Obj);
	virtual ~CAstroTail();
	float m_MovedDist;

public:
	virtual bool LoadResource();
	virtual void Update(float DeltaTime);
	virtual void SetEssential(const Vector2& Dir, const Vector2& Pos, float MaxSpeed);
};

