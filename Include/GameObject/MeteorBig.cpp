#include "MeteorBig.h"

#include "../Collision/ColliderBox.h"
#include "../Collision/ColliderCircle.h"
#include "../Collision/ColliderLine.h"
#include "Player.h"
#include "../Scene/Scene.h"
#include "MeteorBigHalf.h"
#include "Explosion.h"

CMeteorBig::CMeteorBig():
	m_Player(),
	m_PlayerColl(),
	m_MyColl(),
	m_isCheckedColl()
{
	SetTypeID<CMeteorBig>();
}

CMeteorBig::CMeteorBig(const CMeteorBig& Obj) :
	CMeteor(Obj),
	m_Player(),
	m_PlayerColl(),
	m_MyColl(),
	m_isCheckedColl(Obj.m_isCheckedColl)
{


}

CMeteorBig::~CMeteorBig()
{
}



bool CMeteorBig::Init(CGameObject* Obj)
{
	CMeteor::Init(Obj);

	SetRenderLayer(ERenderLayer::Enemy);

	m_HP = 400;
	SetSize(100.f, 80.f);

	//이동명령 on
	m_Pos.x = 100.f;
	m_Pos.y = -100.f;

	//목표위치 지정
	SetMaxSpeed(30.f);
	Vector2 Dest;
	Dest.x = 100.f;
	Dest.y = (float)ORIGINAL_GAME_RES_HEIGHT + 150.f;
	MoveToDest(true, EMoveToMethod::MoveDir, Dest);

	//애니메이션, 히트이펙트 엔드펑션 등록
	SetAnimation("MeteorBig");
	SetEndFunction< CMeteorBig>("MeteorBigHit", this, &CMeteorBig::HitEffectEnd);
	//이 클래스는 "MeteorBigDestroy" 애니메이션을 재생하고 사라진다(조각 2개 생성)
	SetEndFunction<CMeteorBig>("MeteorBigDestroy", this, &CMeteorBig::MeteorBigDestroy);



	m_MyColl = AddCollider<CColliderBox>("MeteorBigColl");
	if (!m_MyColl)
		return false;
	m_MyColl->SetCollisionProfile(ECollisionChannel::Enemy);
	m_MyColl->SetSize(m_Size);
	m_MyColl->SetPivot(m_Pivot);
	m_MyColl->SetOffset(0.f, -20.f);


	//StartFunc에서 포인터 등록, EndFunc에서 포인터 해제
	m_MyColl->SetCollisionBeginFunc< CMeteorBig>(this, &CMeteorBig::CollisionBegin);
	m_MyColl->SetCollisionEndFunc<CMeteorBig>(this, &CMeteorBig::CollisionEnd);




	return true;
}

void CMeteorBig::Start()
{
	m_Start = true;
}


void CMeteorBig::Update(float DeltaTime)
{
	if (!m_Start)
		Start();

	CMeteor::Update(DeltaTime);

	CollisionUpdate();

	if (m_Pos.y > 170.f)
	{
		do
		{
			if (m_isCheckedColl)
				break;

			Vector2 Pos = m_Scene->GetPlayer()->GetPos();
			Vector2 Pivot = m_Scene->GetPlayer()->GetPivot();
			Vector2 Size = m_Scene->GetPlayer()->GetSize();

			if (Pos.y < m_Pos.y)
				break;

			bool isInflictDmg = false;
			float PlayerLeft = Pos.x - (Size.x * Pivot.x);
			float PlayerRight = Pos.x + Size.x;
			float MyLeft = m_Pos.x - (m_Size.x * m_Pivot.x);
			float MyRight = MyLeft + m_Size.x;
				
			if (PlayerLeft > MyLeft && PlayerLeft < MyRight)
			{
				isInflictDmg = true;
			}
				
			if (PlayerRight > MyLeft && PlayerRight < MyLeft)
			{
				isInflictDmg = true;
			}
				
			if (isInflictDmg)
			{
				m_isCheckedColl = true;

				CExplosion* Boom = m_Scene->CreateObject<CExplosion>("MeteorDamage");
				Boom->SetEssential(EExplosionSize::Small, Vector2(Pos.x, Pos.y - (m_Size.y * m_Pivot.y)));
				m_Scene->GetPlayer()->InflictDamage(1);
				InflictDamage(10000);
				ChangeAnimation("MeteorBigDestroy");
			}

		} while (false);

	}
	
}

void CMeteorBig::PostUpdate(float DeltaTime)
{
	if (!m_Start)
		Start();

	CMeteor::PostUpdate(DeltaTime);
}

void CMeteorBig::Render(HDC hDC, float DeltaTime)
{
	if (!m_Start)
		Start();

	CMeteor::Render(hDC, DeltaTime);
}


void CMeteorBig::SetEssential(float StartPosX, float DestPosX)
{
	m_Pos.x = StartPosX;
	m_MoveToDest.x = DestPosX;
}


int CMeteorBig::InflictDamage(int Damage)
{
	

	int DMG = CMeteor::InflictDamage(Damage);

	ChangeAnimation("MeteorBigHit");

	m_HP -= DMG;
	if (m_HP <= 0)
	{
		SoundPlay("BigMeteorCrack");
		ClearCollider();
		ChangeAnimation("MeteorBigDestroy");
	}


	return DMG;
}

void CMeteorBig::CollisionBegin(CCollider* Src, CCollider* Dest)
{
	m_Player = dynamic_cast<CPlayer*>(Dest->GetOwnerObj());

	if (m_Player)
	{
		m_PlayerColl = dynamic_cast<CColliderBox*>(Dest);
	}
		
	if (!m_PlayerColl)
	{
		m_Player = nullptr;
		return;
	}
}


void CMeteorBig::CollisionEnd(CCollider* Src, CCollider* Dest)
{
	if (dynamic_cast<CPlayer*>(Dest->GetOwnerObj()) == m_Player)
	{
		m_Player = nullptr;
		m_PlayerColl = nullptr;
	}
		
}

void CMeteorBig::CollisionUpdate()
{
	if (!m_Player || !m_PlayerColl || !m_MyColl)
		return;

	BoxInfo MyInfo = m_MyColl->GetInfo();
	BoxInfo PlayerInfo = m_PlayerColl->GetInfo();

	float AdjustX = 0.f;
	float AdjustY = 0.f;

	do
	{
		//완전이 포함되는 경우는 이쪽에서 처리할 문제가 아니라 위아래로 처리할 문제임
		if (MyInfo.LT.x > PlayerInfo.LT.x && MyInfo.RB.x < PlayerInfo.RB.x)
			break;
		if (PlayerInfo.LT.x > MyInfo.LT.x && PlayerInfo.RB.x < MyInfo.RB.x)
			break;

		//플레이어가 오른쪽을 침범하고 있을때(이떄 플레이어의 왼쪽도 왼쪽에 있어야함
		if (MyInfo.LT.x < PlayerInfo.RB.x && PlayerInfo.LT.x < MyInfo.LT.x)
		{
			AdjustX = MyInfo.LT.x - PlayerInfo.RB.x;
		}

		//플레이어가 왼쪽을 침범하고 있을때
		else if (MyInfo.RB.x > PlayerInfo.LT.x && MyInfo.RB.x < PlayerInfo.RB.x)
		{
			AdjustX = MyInfo.RB.x - PlayerInfo.LT.x;
		}
	} while (false);
		
	do
	{
		//완전이 포함되는 경우는 이쪽에서 처리할 문제가 아니라 좌우로 처리할 문제임
		if (MyInfo.LT.y > PlayerInfo.LT.y && MyInfo.RB.y < PlayerInfo.RB.y)
			break;
		if (PlayerInfo.LT.y > MyInfo.LT.y && PlayerInfo.RB.y < MyInfo.RB.y)
			break;

		//플레이어가 위을 침범하고 있을때(이때 플레이어의 위쪽도 사각형 위쪽에 있어야함)
		if (MyInfo.LT.y < PlayerInfo.RB.y && PlayerInfo.LT.y < MyInfo.LT.y)
		{
			AdjustY = MyInfo.LT.y - PlayerInfo.RB.y;
		}

		//플레이어가 아래를 침범하고 있을때(이때 플레이어의 아랫쪽도 사각형 아래에 있어야함
		else if (MyInfo.RB.y > PlayerInfo.LT.y && MyInfo.RB.y < PlayerInfo.RB.y)
		{
			AdjustY = MyInfo.RB.y - PlayerInfo.LT.y;
		}
	} while (false);

	if (AdjustX == 0.f)
	{
		if (AdjustY < 0.f)
			AdjustY -= 0.1f;
		else
			AdjustY += 0.1f;

		m_Player->MoveValue(Vector2(0.f, AdjustY));
		return;
	}
	else if (AdjustY == 0.f)
	{
		if (AdjustX < 0.f)
			AdjustX -= 0.1f;
		else
			AdjustX += 0.1f;

		m_Player->MoveValue(Vector2(AdjustX, 0.f));
		return;
	}


	if (fabs(AdjustX) < fabs(AdjustY))
	{
		if (AdjustX < 0.f)
			AdjustX -= 0.1f;
		else
			AdjustX += 0.1f;

		m_Player->MoveValue(Vector2(AdjustX, 0.f));
	}
	else
	{
		if (AdjustY < 0.f)
			AdjustY -= 0.1f;
		else
			AdjustY += 0.1f;

		m_Player->MoveValue(Vector2(0.f, AdjustY));
	}
}

void CMeteorBig::HitEffectEnd()
{
	ChangeAnimation("MeteorBig");
}

void CMeteorBig::MeteorBigDestroy()
{
	ItemDrop();//아이템 드롭

	//반쪽 애니메이션 2개 생성
	CMeteorBigHalf* BH = m_Scene->CreateObject< CMeteorBigHalf>("MeteorBigHalf");
	BH->SetEssential(m_Pos, EMeteorBigHalfPart::Left);
	BH = m_Scene->CreateObject< CMeteorBigHalf>("MeteorBigHalf");
	BH->SetEssential(m_Pos, EMeteorBigHalfPart::Right);



	SetActive(false);
}
