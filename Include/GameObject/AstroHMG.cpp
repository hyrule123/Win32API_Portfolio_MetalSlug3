#include "AstroHMG.h"

//충돌체
#include "../Collision/ColliderBox.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

//총알 이펙트 생성용
#include "PistolHit.h"

#include "../ScoreManager.h"

CAstroHMG::CAstroHMG()
{
	SetTypeID<CAstroHMG>();
}

CAstroHMG::CAstroHMG(const CAstroHMG& Obj) :
	CProjectile(Obj)
{
}

CAstroHMG::~CAstroHMG()
{
}

bool CAstroHMG::Init(CGameObject* Obj)
{
	CProjectile::Init(Obj);

	m_MaxSpeed = 600.f;
	m_Damage = 10;

	SetSize(5.f, 30.f);
	SetPivot(0.5f, 1.f);

	SetRenderLayer(ERenderLayer::EffectLow);

	//충돌체 설정
	CColliderBox* Coll = AddCollider<CColliderBox>("Bullet");

	Coll->SetCollisionProfile(ECollisionChannel::PlayerAttack);
	Coll->SetSize(m_Size);
	Coll->SetPivot(0.5f, 1.f);

	//호출 함수 설
	Coll->SetCollisionBeginFunc(this, &CAstroHMG::CollisionBegin);

	m_isReady = true;

	return true;
}

bool CAstroHMG::LoadResource()
{
	SetTexture("HMG", TEXT("SFX/HMGSpace.bmp"));
	SetColorKey(0, 248, 0);

	return true;
}

void CAstroHMG::SetEssential(Vector2 Dir, Vector2 Pos)
{
	m_isReady = true;

	m_Dir = Dir;
	m_Pos.x = Pos.x;
	m_Pos.y = Pos.y;
}



void CAstroHMG::CollisionBegin(CCollider* Src, CCollider* Dest)
{
	
	m_Scene->GetSceneResource()->SoundPlay("BulletHitSound");

	CPistolHit* Effect = m_Scene->CreateObject<CPistolHit>("BulletSFX");

	//약간 랜덤하게 피격이펙트 표시
	if (Effect)
	{
		Effect->SetPos(Src->GetHitPoint() + (rand() % 10));
	}

	Dest->GetOwnerObj()->InflictDamage(m_Damage);

	//점수
	CScoreManager::GetInst()->AddScore(100);

	SetActive(false);
}

