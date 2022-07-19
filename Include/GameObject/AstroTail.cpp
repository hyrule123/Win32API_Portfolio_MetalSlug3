#include "AstroTail.h"

#include "AstroSlugSFX.h"

CAstroTail::CAstroTail():
	m_MovedDist()
{
	SetTypeID<CAstroTail>();
}

CAstroTail::CAstroTail(const CAstroTail& Obj) :
	CJustMovingSFX(Obj),
	m_MovedDist(Obj.m_MovedDist)
{
}

CAstroTail::~CAstroTail()
{
}



bool CAstroTail::LoadResource()
{
	if (!LoadCSVPreset(TEXT("SFX/AstroTailSFX.csv")))
		return false;

	return true;
}

void CAstroTail::Update(float DeltaTime)
{
	CJustMovingSFX::Update(DeltaTime);

	AddImpact(m_MaxSpeed);
}

void CAstroTail::SetEssential(const Vector2& Dir, const Vector2& Pos, float MaxSpeed)
{
	m_isReady = true;
	m_Dir = Dir;
	m_Pos = Pos;
	m_MaxSpeed = MaxSpeed;
}

