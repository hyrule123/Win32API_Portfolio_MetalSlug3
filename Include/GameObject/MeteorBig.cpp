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

	//�̵���� on
	m_Pos.x = 100.f;
	m_Pos.y = -100.f;

	//��ǥ��ġ ����
	SetMaxSpeed(30.f);
	Vector2 Dest;
	Dest.x = 100.f;
	Dest.y = (float)ORIGINAL_GAME_RES_HEIGHT + 150.f;
	MoveToDest(true, EMoveToMethod::MoveDir, Dest);

	//�ִϸ��̼�, ��Ʈ����Ʈ ������� ���
	SetAnimation("MeteorBig");
	SetEndFunction< CMeteorBig>("MeteorBigHit", this, &CMeteorBig::HitEffectEnd);
	//�� Ŭ������ "MeteorBigDestroy" �ִϸ��̼��� ����ϰ� �������(���� 2�� ����)
	SetEndFunction<CMeteorBig>("MeteorBigDestroy", this, &CMeteorBig::MeteorBigDestroy);



	m_MyColl = AddCollider<CColliderBox>("MeteorBigColl");
	if (!m_MyColl)
		return false;
	m_MyColl->SetCollisionProfile(ECollisionChannel::Enemy);
	m_MyColl->SetSize(m_Size);
	m_MyColl->SetPivot(m_Pivot);
	m_MyColl->SetOffset(0.f, -20.f);


	//StartFunc���� ������ ���, EndFunc���� ������ ����
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
		//������ ���ԵǴ� ���� ���ʿ��� ó���� ������ �ƴ϶� ���Ʒ��� ó���� ������
		if (MyInfo.LT.x > PlayerInfo.LT.x && MyInfo.RB.x < PlayerInfo.RB.x)
			break;
		if (PlayerInfo.LT.x > MyInfo.LT.x && PlayerInfo.RB.x < MyInfo.RB.x)
			break;

		//�÷��̾ �������� ħ���ϰ� ������(�̋� �÷��̾��� ���ʵ� ���ʿ� �־����
		if (MyInfo.LT.x < PlayerInfo.RB.x && PlayerInfo.LT.x < MyInfo.LT.x)
		{
			AdjustX = MyInfo.LT.x - PlayerInfo.RB.x;
		}

		//�÷��̾ ������ ħ���ϰ� ������
		else if (MyInfo.RB.x > PlayerInfo.LT.x && MyInfo.RB.x < PlayerInfo.RB.x)
		{
			AdjustX = MyInfo.RB.x - PlayerInfo.LT.x;
		}
	} while (false);
		
	do
	{
		//������ ���ԵǴ� ���� ���ʿ��� ó���� ������ �ƴ϶� �¿�� ó���� ������
		if (MyInfo.LT.y > PlayerInfo.LT.y && MyInfo.RB.y < PlayerInfo.RB.y)
			break;
		if (PlayerInfo.LT.y > MyInfo.LT.y && PlayerInfo.RB.y < MyInfo.RB.y)
			break;

		//�÷��̾ ���� ħ���ϰ� ������(�̶� �÷��̾��� ���ʵ� �簢�� ���ʿ� �־����)
		if (MyInfo.LT.y < PlayerInfo.RB.y && PlayerInfo.LT.y < MyInfo.LT.y)
		{
			AdjustY = MyInfo.LT.y - PlayerInfo.RB.y;
		}

		//�÷��̾ �Ʒ��� ħ���ϰ� ������(�̶� �÷��̾��� �Ʒ��ʵ� �簢�� �Ʒ��� �־����
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
	ItemDrop();//������ ���

	//���� �ִϸ��̼� 2�� ����
	CMeteorBigHalf* BH = m_Scene->CreateObject< CMeteorBigHalf>("MeteorBigHalf");
	BH->SetEssential(m_Pos, EMeteorBigHalfPart::Left);
	BH = m_Scene->CreateObject< CMeteorBigHalf>("MeteorBigHalf");
	BH->SetEssential(m_Pos, EMeteorBigHalfPart::Right);



	SetActive(false);
}
