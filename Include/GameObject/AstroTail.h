#pragma once
#include "JustMovingSFX.h"

//AstroSlug ���� Ŭ����. �ٸ� Ŭ���� ��� X
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

