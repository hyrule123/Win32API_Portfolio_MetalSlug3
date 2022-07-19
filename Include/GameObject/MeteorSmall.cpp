#include "MeteorSmall.h"

#include "../Collision/ColliderCircle.h"
#include "PistolHit.h"
#include "../Scene/Scene.h"

CMeteorSmall::CMeteorSmall()
{
	SetTypeID<CMeteorSmall>();
}

CMeteorSmall::CMeteorSmall(const CMeteorSmall& Obj) :
	CMeteor(Obj)
{

}

CMeteorSmall::~CMeteorSmall()
{



}


bool CMeteorSmall::Init(CGameObject* Obj)
{
	CMeteor::Init(Obj);


	SetRenderLayer(ERenderLayer::EffectLow);

	std::string Name = "MeteorSmall";
	Name += std::to_string(rand() % 2);
	SetAnimation(Name);

	//사이즈는 크기에 맞게 지정
	m_Size = m_Animation->GetAnimSize();
	SetPivot(0.5f, 1.f);

	//위치 지정
	m_Pos.x = (float)(rand() % ORIGINAL_GAME_RES_WIDTH);
	m_Pos.y = -100.f;

	//목표위치 지정
	Vector2 Dest;
	Dest.x = (float)(rand() % ORIGINAL_GAME_RES_WIDTH);
	Dest.y = (float)ORIGINAL_GAME_RES_HEIGHT + 10.f;

	MoveToDest(true, EMoveToMethod::MoveDir, Dest);

	SetMaxSpeed((float)(rand() % 201 + 100));

	//충돌체 생성
	CColliderCircle* Coll = AddCollider< CColliderCircle>("MeteorSmall");
	Coll->SetRadius(m_Size.x * m_Pivot.x);
	Coll->SetCollisionBeginFunc< CMeteorSmall>(this, &CMeteorSmall::CollisionBegin);
	Coll->SetOffset(0.f, -3.f);
	Coll->SetCollisionProfile(ECollisionChannel::EnemyAttack);

	return true;
}

void CMeteorSmall::Start()
{
	m_Start = true;


}


void CMeteorSmall::Update(float DeltaTime)
{
	if (!m_Start)
		Start();

	CMeteor::Update(DeltaTime);
}

void CMeteorSmall::PostUpdate(float DeltaTime)
{
	if (!m_Start)
		Start();

	CMeteor::PostUpdate(DeltaTime);
}

void CMeteorSmall::Render(HDC hDC, float DeltaTime)
{
	if (!m_Start)
		Start();

	CMeteor::Render(hDC, DeltaTime);
}


void CMeteorSmall::CollisionBegin(CCollider* Src, CCollider* Dest)
{
	CPistolHit* Hit = m_Scene->CreateObject<CPistolHit>("CPistolHit");
	if (Hit)
	{
		Hit->SetPos(m_Pos);
	}
	SetActive(false);
}