#include "ProjectileEnemy.h"

#include "../Scene/Scene.h"

#include "../GameManager.h"

#include "../Collision/ColliderBox.h"
#include "../Collision/ColliderCircle.h"

#include "Explosion.h"


CProjectileEnemy::CProjectileEnemy():
	m_ProjectileType(),
	m_vecProjectileName{},
	m_Coll(),
	m_CollCircle()
{
	SetTypeID<CProjectileEnemy>();
}

CProjectileEnemy::CProjectileEnemy(const CProjectileEnemy& Obj) :
	CProjectile(Obj),
	m_ProjectileType(Obj.m_ProjectileType),
	m_Coll(),
	m_CollCircle(Obj.m_CollCircle)
{
	int size = (int)EProjectileEnemy::End;
	for (int i = 0; i < size; ++i)
	{
		m_vecProjectileName[i] = Obj.m_vecProjectileName[i];
	}
}

CProjectileEnemy::~CProjectileEnemy()
{
}

bool CProjectileEnemy::LoadResource()
{
	if (!LoadCSVPreset(TEXT("Enemy/UFOProjectile.csv")))
		return false;

	if (!LoadCSVPreset(TEXT("SFX/MiniUFOAttack.csv")))
		return false;

	if (!LoadCSVPreset(TEXT("Enemy/DaiManjiAttack.csv")))
		return false;

	m_vecProjectileName[(int)EProjectileEnemy::UFOLaser] = "UFO_Projectile";
	m_vecProjectileName[(int)EProjectileEnemy::MiniUFOAttack] = "MiniUFOAttack";
	m_vecProjectileName[(int)EProjectileEnemy::DaiManjiAttack] = "DaiManjiAttack";

	return true;
}

bool CProjectileEnemy::Init(CGameObject* Obj)
{
	CProjectile::Init(Obj);
	
	//전부 켜줌
	SetCullingDelete(ECullingDelete::All, true);

	SetRenderLayer(ERenderLayer::EffectLow);
	

	return true;
}

void CProjectileEnemy::Start()
{
	m_Start = true;
	if (m_Coll)
		m_Coll->SetCollisionProfile(ECollisionChannel::EnemyAttack);
}

void CProjectileEnemy::Update(float DeltaTime)
{
	CProjectile::Update(DeltaTime);

	switch (m_ProjectileType)
	{
	case EProjectileEnemy::UFOLaser:
	{
		//애니메이션 사이즈가 변경될 때마다 오프셋값을 변경
		Vector2 Size = m_Animation->GetAnimSize();
		if(Size.y > 30.f)
			m_Coll->SetOffset(0.f, Size.y);

	}
		break;
	}

}

void CProjectileEnemy::PostUpdate(float DeltaTime)
{
	CProjectile::PostUpdate(DeltaTime);
}

void CProjectileEnemy::Render(HDC hDC, float DeltaTime)
{
	CProjectile::Render(hDC, DeltaTime);
}

void CProjectileEnemy::SetEssential(EProjectileEnemy Type, const Vector2& Dir, const Vector2& Pos)
{
	m_ProjectileType = Type;
	m_Dir = Dir;
	m_Pos = Pos;

	SetAnimation(m_vecProjectileName[(int)m_ProjectileType]);

	//타입별 개별 설정
	switch (m_ProjectileType)
	{
	case EProjectileEnemy::UFOLaser:
	{
		SetMaxSpeed(300.f);

		m_Coll = AddCollider<CColliderBox>("UFOLaserColl");
		m_Coll->SetPivot(0.5f, 0.3f);
		m_Coll->SetSize(4.f, 20.f);
		m_Coll->SetCollisionBeginFunc< CProjectileEnemy>(this, &CProjectileEnemy::CollisionBegin);
		//m_MyAnimation = m_Animation->FindAnimInfo(m_vecProjectileName[(int)m_ProjectileType])->
		break;
	}
	case EProjectileEnemy::MiniUFOAttack:
	{
		SetMaxSpeed(200.f);

		m_Coll = AddCollider<CColliderBox>("MiniUFOAttackColl");
		if (!m_Coll)
			return;
		m_Coll->SetPivot(0.5f, 1.f);
		m_Coll->SetSize(10.f, 10.f);
		m_Coll->SetCollisionBeginFunc< CProjectileEnemy>(this, &CProjectileEnemy::CollisionBegin);
		break;
	}
	case EProjectileEnemy::DaiManjiAttack:
	{
		SetMaxSpeed(100.f);
		m_CollCircle = AddCollider<CColliderCircle>("DaiManjiAttackColl");
		if (!m_CollCircle)
			return;
		m_CollCircle->SetCollisionProfile(ECollisionChannel::EnemyAttack);
		m_CollCircle->SetRadius(32.f);
		m_CollCircle->SetOffset(0.f, -35.f);
		m_CollCircle->SetCollisionBeginFunc<CProjectileEnemy>(this, &CProjectileEnemy::CollisionBegin);
		break;
	}
		
	}


	m_isReady = true;
}



void CProjectileEnemy::CollisionBegin(CCollider* Src, CCollider* Dest)
{
	Dest->GetOwnerObj()->InflictDamage(1);

	if (m_ProjectileType == EProjectileEnemy::DaiManjiAttack)
	{
		SoundPlay("BigExplosion");
		CExplosion* Explosion = m_Scene->CreateObject<CExplosion>("ProjectileExplosion");
		Explosion->SetEssential(EExplosionSize::Big, Src->GetHitPoint());
	}
	else
	{
		SoundPlay("SmallExplosion");
		CExplosion* Explosion = m_Scene->CreateObject<CExplosion>("ProjectileExplosion");
		Explosion->SetEssential(EExplosionSize::Small, Src->GetHitPoint());
	}

	SetActive(false);
}


