#include "DamageExplosion.h"

#include "../Collision/ColliderCircle.h"

#include "../Scene/Scene.h"

#include "../ScoreManager.h"


CDamageExplosion::CDamageExplosion():
	m_Damage(),
	m_Coll()
{
	SetTypeID<CDamageExplosion>();
}

CDamageExplosion::CDamageExplosion(const CDamageExplosion& Obj):
	CExplosion(Obj),
	m_Damage(Obj.m_Damage)
{
	for (int i = 0; i < (int)EExplosionSize::Max; ++i)
	{
		m_vecSizeName[i] = Obj.m_vecSizeName[i];
	}

	if (Obj.m_Coll)
	{
		m_Coll = AddCollider<CColliderCircle>("ExplosionCollider");
		
		//���� ����
		*m_Coll = *Obj.m_Coll;
	}
}

CDamageExplosion::~CDamageExplosion()
{
}



bool CDamageExplosion::Init(CGameObject* Obj)
{
	CExplosion::Init(Obj);

	SetDir(0.f, -1.f);
	SetMaxSpeed(10.f);

	return true;
}

void CDamageExplosion::Update(float DeltaTime)
{
	CExplosion::Update(DeltaTime);

	AddForce(10.f);
}


void CDamageExplosion::SetEssential(EExplosionSize ExplosionSize, const Vector2& Pos, int Damage)
{
	CExplosion::SetEssential(ExplosionSize, Pos);


	//2������ ��Ƽ���� ����
	AddNotify<CDamageExplosion>(m_vecSizeName[(int)ExplosionSize], 2, this, &CDamageExplosion::Frame2Notify);


	//�浹ü ����
	m_Coll = AddCollider<CColliderCircle>("ExplosionColl");
	m_Coll->SetCollisionProfile(ECollisionChannel::PlayerAttack);

	if (ExplosionSize == EExplosionSize::Small)
	{
		SetOffset(0.f, -17.f);
		m_Coll->SetOffset(0.f, -17.f);
		m_Coll->SetRadius(17.f);
	}

	else if (ExplosionSize == EExplosionSize::Midium)
	{
		SetOffset(0.f, -25.f);
		m_Coll->SetOffset(0.f, -25.f);
		m_Coll->SetRadius(25.f);

	}
	else if (ExplosionSize == EExplosionSize::Big)
	{
		SetOffset(0.f, -40.f);
		m_Coll->SetOffset(0.f, -40.f);
		m_Coll->SetRadius(40.f);

	}

	m_Coll->SetCollisionBeginFunc<CDamageExplosion>(this, &CDamageExplosion::CollisionBegin);

	m_Damage = Damage;

	m_isReady = true;
	SetActive(true);
}

void CDamageExplosion::CollisionBegin(CCollider* Src, CCollider* Dest)
{
	Dest->GetOwnerObj()->InflictDamage(m_Damage);
	CScoreManager::GetInst()->AddScore(100);
}

void CDamageExplosion::Frame2Notify()
{
	//�ڽ��� �浹ü ����
	ClearCollider();
}
