#include "MeteorBigHalf.h"


#include "MeteorBigHalf.h"

#include "../Collision/ColliderBox.h"
#include "../Collision/ColliderCircle.h"
#include "../Collision/ColliderLine.h"
#include "Player.h"


CMeteorBigHalf::CMeteorBigHalf():
	m_Part()
{
	SetTypeID<CMeteorBigHalf>();
}

CMeteorBigHalf::CMeteorBigHalf(const CMeteorBigHalf& Obj) :
	CMeteor(Obj),
	m_Part(Obj.m_Part)
{

}

CMeteorBigHalf::~CMeteorBigHalf()
{
}



bool CMeteorBigHalf::Init(CGameObject* Obj)
{
	CMeteor::Init(Obj);

	SetRenderLayer(ERenderLayer::Default);

	SetMaxSpeed(20.f);

	return true;
}

void CMeteorBigHalf::Start()
{
	m_Start = true;
}


void CMeteorBigHalf::Update(float DeltaTime)
{
	if (!m_Start)
		Start();

	CMeteor::Update(DeltaTime);

	switch (m_Part)
	{
	case EMeteorBigHalfPart::Left:
		MoveDir(Vector2( - 1.f, 0.f));
		break;
	case EMeteorBigHalfPart::Right:
		MoveDir(Vector2(1.f, 0.f));
		break;
	default:
		break;
	}

	MoveDir(Vector2(0.f, 1.f));
}


void CMeteorBigHalf::SetEssential(const Vector2& Pos, EMeteorBigHalfPart Part)
{
	m_Pos = Pos;
	m_Part = Part;

	switch (m_Part)
	{
	case EMeteorBigHalfPart::Left:
		SetAnimation("MeteorBigHalfLeft");
		break;
	case EMeteorBigHalfPart::Right:
		SetAnimation("MeteorBigHalfRight");
		break;
	default:
		break;
	}
}





