#include "MeteorMid.h"

#include "../Scene/Scene.h"
#include "../Collision/ColliderCircle.h"

#include "Explosion.h"

#include "Player.h"

CMeteorMid::CMeteorMid()
{
	SetTypeID<CMeteorMid>();
}

CMeteorMid::CMeteorMid(const CMeteorMid& Obj) :
	CMeteor(Obj)
{

}

CMeteorMid::~CMeteorMid()
{
}


bool CMeteorMid::Init(CGameObject* Obj)
{
	CMeteor::Init(Obj);



	//체력 30
	m_HP = 30;

	SetRenderLayer(ERenderLayer::Enemy);
	SetAnimation("MeteorMid");

	//사이즈는 크기에 맞게 지정
	m_Size = m_Animation->GetAnimSize();
	SetPivot(0.5f, 1.f);

	//위치 지정
	m_Pos.x = (float)(rand() % ORIGINAL_GAME_RES_WIDTH);
	m_Pos.y = -100.f;

	//목표위치 지정
	Vector2 Dest;
	Dest.x = (float)(rand() % ORIGINAL_GAME_RES_WIDTH);
	Dest.y = (float)ORIGINAL_GAME_RES_HEIGHT + 100.f;

	MoveToDest(true, EMoveToMethod::MoveDir, Dest);

	SetMaxSpeed((float)(rand() % 201 + 100));

	//충돌체 생성
	CColliderCircle* Coll = AddCollider< CColliderCircle>("MeteorMid");
	Coll->SetRadius(m_Size.x * m_Pivot.x);
	Coll->SetCollisionBeginFunc< CMeteorMid>(this, &CMeteorMid::CollisionBegin);
	Coll->SetOffset(0.f, -15.f);
	Coll->SetCollisionProfile(ECollisionChannel::Enemy);



	return true;
}

void CMeteorMid::Start()
{
	m_Start = true;


}


void CMeteorMid::Update(float DeltaTime)
{
	if (!m_Start)
		Start();


	CMeteor::Update(DeltaTime);
}

void CMeteorMid::PostUpdate(float DeltaTime)
{
	if (!m_Start)
		Start();

	CMeteor::PostUpdate(DeltaTime);



}

void CMeteorMid::Render(HDC hDC, float DeltaTime)
{
	if (!m_Start)
		Start();

	CMeteor::Render(hDC, DeltaTime);
}


int CMeteorMid::InflictDamage(int Damage)
{
	int DMG = CMeteor::InflictDamage(Damage);

	m_HP -= DMG;
	if (m_ItemDropType == EItemList::End)
	{
		StartReaction(0.1f, false, EReactionChannel::HitReaction);
	}

	if (m_HP <= 0)
	{
		SoundPlay("SmallExplosion");
		CExplosion* Boom = m_Scene->CreateObject<CExplosion>("Meteor");
		Boom->SetEssential(EExplosionSize::Small, m_Pos);
		ItemDrop();
		SetActive(false);
	}
		



	return DMG;
}

void CMeteorMid::SetItemDrop(EItemList ItemType)
{
	if (ItemType < EItemList::HMG || ItemType >= EItemList::End)
		return;

	CMeteor::SetItemDrop(ItemType);

	if (m_ItemDropType < EItemList::End)
	{
		StartReaction(1000.f, false, EReactionChannel::HitReaction);
		m_Pos.x = (float)(ORIGINAL_GAME_RES_WIDTH / 2);
		m_MoveToDest.x = (float)(rand() % 101 + 100);
	}
}

void CMeteorMid::CollisionBegin(CCollider* Src, CCollider* Dest)
{
	CGameObject* Obj = Dest->GetOwnerObj();

	if (dynamic_cast<CPlayer*>(Obj))
	{
		SoundPlay("SmallExplosion");

		Obj->InflictDamage(1);

		CExplosion* Boom = m_Scene->CreateObject<CExplosion>("Meteor");
		Boom->SetEssential(EExplosionSize::Small, m_Pos);

		SetActive(false);
	}

}