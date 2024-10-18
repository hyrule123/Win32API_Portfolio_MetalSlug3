#include "Shotgun.h"
//충돌체
#include "../Collision/ColliderBox.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

//주인 위치를 받아오기 위함
#include "Player.h"

#include "../GameManager.h"

#include "../ScoreManager.h"

CShotgun::CShotgun() :
	m_Player(),
	m_Offset()
{
	SetTypeID<CShotgun>();
}

CShotgun::CShotgun(const CShotgun& Obj) :
	CProjectile(Obj),
	m_Player(Obj.m_Player),
	m_Offset(Obj.m_Offset)
{
}

CShotgun::~CShotgun()
{
}

bool CShotgun::Init(CGameObject* Obj)
{
	CProjectile::Init(Obj);

	//렌더링레이어
	SetRenderLayer(ERenderLayer::EffectLow);


	//애니메이션 및 노티파이(충돌체 제거)
	SetAnimation("Shotgun_Fire");
	AddNotify<CShotgun>("Shotgun_Fire", 2, this, &CShotgun::ColliderDelete);

	//endfunction 설정(재생 후 제거)
	SetEndFunction<CShotgun>("Shotgun_Fire", this, &CShotgun::AnimEndDelete);

	m_Scene->GetSceneResource()->SoundPlay("Shotgun");


	m_Damage = 200;
	SetSize(67.f, 90.f);
	SetPivot(0.5f, 1.f);
	SetMaxSpeed(0.f);

	//충돌체 설정
	CColliderBox* Coll = AddCollider<CColliderBox>("Shotgun");

	Coll->SetCollisionProfile(ECollisionChannel::PlayerAttack);
	Coll->SetSize(m_Size);
	Coll->SetPivot(m_Pivot);

	//호출 함수 설정
	Coll->SetCollisionBeginFunc(this, &CShotgun::CollisionBegin);

	//반짝임 설정
	CGameManager::GetInst()->SetWhiteOut(1);

	return true;
}

void CShotgun::PostUpdate(float DeltaTime)
{
	CProjectile::PostUpdate(DeltaTime);

	if (m_Player)
		m_Pos = m_Player->GetPos() + m_Offset;
}

bool CShotgun::LoadResource()
{
	if(!LoadCSVPreset(TEXT("SFX/ShotgunSpace.csv")))
		return false;

	return true;
}

void CShotgun::SetEssential(CPlayer* Player, const Vector2& Offset)
{
	m_isReady = true;

	m_Player = Player;
	m_Offset = Offset;
}



void CShotgun::CollisionBegin(CCollider* Src, CCollider* Dest)
{
	Dest->GetOwnerObj()->InflictDamage(m_Damage);
	CScoreManager::GetInst()->AddScore(100);
}

void CShotgun::ColliderDelete()
{
	ClearCollider();
}

void CShotgun::AnimEndDelete()
{
	SetActive(false);
}
