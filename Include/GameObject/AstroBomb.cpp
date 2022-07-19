#include "AstroBomb.h"

//�浹ü
#include "../Collision/ColliderBox.h"
#include "../../Include/Scene/Scene.h"

//���� ����Ʈ ������
#include "DamageExplosion.h"

//�浹�Լ����� DELTA_TIME ����
#include "../GameManager.h"


CAstroBomb::CAstroBomb() :
	m_Target(),
	m_MaxTurnDirX(0.5f),
	m_isRight(),
	m_Delay(),
	m_StartIgnition()
{
	SetTypeID<CAstroBomb>();
}

CAstroBomb::CAstroBomb(const CAstroBomb& Obj) :
	CProjectile(Obj),
	m_Target(Obj.m_Target),
	m_MaxTurnDirX(Obj.m_MaxTurnDirX),
	m_isRight(),
	m_Delay(Obj.m_Delay),
	m_StartIgnition(Obj.m_StartIgnition)
{
}

CAstroBomb::~CAstroBomb()
{
}

bool CAstroBomb::Init(CGameObject* Obj)
{
	CProjectile::Init(Obj);

	if (!m_Animation->FindAnimInfo(m_Name))
		return false;

	SetAnimation(m_Name);

	if (m_Name == "BombRight")
		m_isRight = true;
	else if (m_Name == "BombLeft")
		m_isRight = false;

	m_MaxSpeed = 20.f;
	SetSize(16.f, 48.f);

	//���������̾�
	SetRenderLayer(ERenderLayer::EffectLow);

	//�浹ü 1 ���� -> ����� �� �����ϴ� �뵵
	CColliderBox* Coll = AddCollider<CColliderBox>("AstroBombColl");
	Coll->SetCollisionProfile(ECollisionChannel::PlayerAttack);
	Coll->SetSize(m_Size);
	Coll->SetCollisionBeginFunc(this, &CAstroBomb::CollisionBegin);
	Coll->SetOffset(0.f, -45.f);


	//m_Damage = 0;	��ü �������� ����. ���� ���� ���÷��� �������� ����.
	if (m_isRight)
	{
		SetDir(1.f, 1.f);	//���� ����
		SetPivot(0.f, 0.f);

		//endfunction -> ���� �ڸ������� ��ȭ�Ǵ� �뵵
		SetEndFunction<CAstroBomb>("BombRight", this, &CAstroBomb::AstroBombStartEnd);

		Coll->SetPivot(m_Pivot);
		
	}
	else
	{
		SetDir(-1.f, 1.f);
		SetPivot(1.f, 0.f);

		//endfunction -> ���� �ڸ������� ��ȭ�Ǵ� �뵵
		SetEndFunction<CAstroBomb>("BombLeft", this, &CAstroBomb::AstroBombStartEnd);

		Coll->SetPivot(m_Pivot);
		
	}

	
	




	Coll = nullptr;
	//�浹ü 2 ���� -> ����� �� �����Ǵ� �뵵
	Coll = AddCollider<CColliderBox>("AstroBombHomming");
	Coll->SetCollisionProfile(ECollisionChannel::PlayerAttack);

	//���� ȭ�� ũ���� �浹ü�� ����
	Coll->SetSize(100.f, (float)ORIGINAL_GAME_RES_HEIGHT);
	Coll->SetPivot(0.5f, 1.f);
	Coll->SetCollisionBeginFunc(this, &CAstroBomb::HommingBegin);



	//�ø� ����
	SetCullingDelete(ECullingDelete::Bottom, false);



	m_isReady = true;

	return true;
}



bool CAstroBomb::LoadResource()
{
	if (!LoadCSVPreset(TEXT("SFX/AstroBomb.csv")))
		return false;

	return true;
}

void CAstroBomb::Update(float DeltaTime)
{
	CProjectile::Update(DeltaTime);


	if (CheckColliding(m_Target))
	{
		//x�࿡�� �󸶳� �������ִ����� üũ
		float PosX = m_Pos.x - m_Target->GetPos().x;

		if (PosX > 0.f)	//����̸� ���ʿ� �ִٴ� ���̹Ƿ�
			m_Dir.x -= m_MaxTurnDirX * DeltaTime;
		else if (PosX < 0.f)
			m_Dir.x += m_MaxTurnDirX * DeltaTime;
	}
	else
	{
		m_Target = nullptr;
	}

	if (m_StartIgnition)
	{
		//�����̵��ȿ��� ������
		m_Delay -= DeltaTime;
		if (m_Delay >= 0.f)
		{
			AddForce(100.f);
		}
		else
		{
			AddForce(800.f);

			//�ִ� �ӵ� 800���� 
			m_MaxSpeed = 800.f;
		}
	}
}

void CAstroBomb::SetEssential(float PosX, float PosY, bool SlowIgnition)
{
	m_isReady = true;

	m_Pos.x = PosX;
	m_Pos.y = PosY;

	if (SlowIgnition)
	{
		m_Delay = 0.2f;
	}
}




void CAstroBomb::AstroBombStartEnd()
{
	SoundPlay("RocketLauncherFire");

	//�ִϸ��̼� �߰�
	SetAnimation("BombIgnition");

	if (!m_isRight)
		m_Animation->SetOffset("BombIgnition", -9.f, -11.f);
	else
		m_Animation->SetOffset("BombIgnition", 10.f, -11.f);

	//���� ����
	SetDir(0.f, -1.f);
	
	//Update�� ��ȭ�Ǿ��ٰ� ����
	m_StartIgnition = true;
}

void CAstroBomb::CollisionBegin(CCollider* Src, CCollider* Dest)
{
	SoundPlayOneChan("RocketLauncherHit");

	//�ǰ�����Ʈ ǥ��. ���� ó���� �浹ü���� ��.
	CDamageExplosion* SFX = m_Scene->CreateObject<CDamageExplosion>("BigExplosion");
	SFX->SetEssential(EExplosionSize::Big, Src->GetHitPoint(), 100);

	//Dest->GetOwnerObj()->InflictDamage(m_Damage);

	SetActive(false);
}

void CAstroBomb::HommingBegin(CCollider* Src, CCollider* Dest)
{
	//Ÿ���� �����Ǿ����� ������ ���� ���� �浹���� �� ����� Ÿ������ ����
	if (!m_Target)
		m_Target = Dest->GetOwnerObj();
}
