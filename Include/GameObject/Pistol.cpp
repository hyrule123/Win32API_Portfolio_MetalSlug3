#include "Pistol.h"

//충돌체
#include "../Collision/ColliderCircle.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

//총알 이펙트 생성용
#include "PistolHit.h"

#include "../ScoreManager.h"

CPistol::CPistol()
{
	SetTypeID<CPistol>();
}

CPistol::CPistol(const CPistol& Obj) :
	CProjectile(Obj)
{
}

CPistol::~CPistol()
{
}

bool CPistol::Init(CGameObject* Obj)
{
	CProjectile::Init(Obj);

	m_MaxSpeed = 800.f;
	m_Damage = 10;

	SetSize(6.f, 10.f);
	SetPivot(0.5f, 0.5f);

	//렌더링레이어
	SetRenderLayer(ERenderLayer::EffectLow);


	//충돌체 설정
	CColliderCircle* Coll = AddCollider<CColliderCircle>("Bullet");

	Coll->SetCollisionProfile(ECollisionChannel::PlayerAttack);
	Coll->SetRadius(m_Size.x);

	//호출 함수 설정
	Coll->SetCollisionBeginFunc(this, &CPistol::CollisionBegin);

	m_Scene->GetSceneResource()->SoundPlay("Pistol");



	m_isReady = true;



	return true;
}

bool CPistol::LoadResource()
{
	SetTexture("Pistol", TEXT("SFX/PistolSpace.bmp"));
	SetColorKey();

	return true;
}



void CPistol::CollisionBegin(CCollider* Src, CCollider* Dest)
{
	m_Scene->GetSceneResource()->SoundPlay("BulletHitSound");

	CPistolHit* Effect = m_Scene->CreateObject<CPistolHit>("BulletSFX");

	//약간 랜덤하게 피격이펙트 표시
	if (Effect)
	{
		Vector2 Pos = Src->GetHitPoint();
		Pos.x += (float)(rand() % 10);
		Pos.y += (float)(rand() % 10);
		Effect->SetPos(Pos);
	}


	Dest->GetOwnerObj()->InflictDamage(m_Damage);
	CScoreManager::GetInst()->AddScore(100);

	SetActive(false);
}

