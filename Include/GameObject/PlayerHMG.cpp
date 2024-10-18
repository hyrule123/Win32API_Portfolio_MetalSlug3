#include "PlayerHMG.h"

//충돌체
#include "../Collision/ColliderBox.h"
#include "../../Include/Scene/Scene.h"

//총알 이펙트 생성용
#include "PistolHit.h"

#include "../Scene/SceneResource.h"

#include "../ScoreManager.h"

CPlayerHMG::CPlayerHMG()
{
	SetTypeID<CPlayerHMG>();
}

CPlayerHMG::CPlayerHMG(const CPlayerHMG& Obj) :
	CProjectile(Obj)
{
}

CPlayerHMG::~CPlayerHMG()
{
}

bool CPlayerHMG::Init(CGameObject* Obj)
{
	CProjectile::Init(Obj);

	m_MaxSpeed = 600.f;
	m_Damage = 10;

	SetSize(5.f, 30.f);
	SetPivot(0.5f, 0.5f);

	SetRenderLayer(ERenderLayer::EffectLow);

	//충돌체 설정
	CColliderBox* Coll = AddCollider<CColliderBox>("Bullet");

	Coll->SetCollisionProfile(ECollisionChannel::PlayerAttack);
	Coll->SetSize(m_Size);

	//호출 함수 설정
	Coll->SetCollisionBeginFunc(this, &CPlayerHMG::CollisionBegin);

	m_isReady = true;

	return true;
}

bool CPlayerHMG::LoadResource()
{
	SetTexture("HMG", TEXT("SFX/HMGSpace.bmp"));
	SetColorKey(0, 248, 0);

	return true;
}

void CPlayerHMG::SetEssential(Vector2 Dir, Vector2 Pos)
{
	m_isReady = true;

	m_Dir = Dir;
	m_Pos.x = Pos.x;
	m_Pos.y = Pos.y - 100.f;
}



void CPlayerHMG::CollisionBegin(CCollider* Src, CCollider* Dest)
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