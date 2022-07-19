#include "BackObjects.h"
#include "../Scene/Scene.h"


#include "../Collision/ColliderBox.h"

//폭발
#include "Explosion.h"

CBackObjects::CBackObjects():
	m_BackObjType()
{
	SetTypeID<CBackObjects>();
}

CBackObjects::CBackObjects(const CBackObjects& Obj) :
	CCharacter(Obj),
	m_BackObjType(Obj.m_BackObjType)
{
}

CBackObjects::~CBackObjects()
{
}



bool CBackObjects::LoadResource()
{
	if(!LoadCSVPreset(TEXT("SFX/BackObjects.csv")))
		return false;

	return true;
}

bool CBackObjects::Init(CGameObject* Obj)
{
	CGameObject::Init(Obj);

	return true;
}

void CBackObjects::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);


}

void CBackObjects::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);

}

void CBackObjects::Render(HDC hDC, float DeltaTime)
{
	CGameObject::Render(hDC, DeltaTime);
}


int CBackObjects::InflictDamage(int Damage)
{
	return CGameObject::InflictDamage(Damage);
}

void CBackObjects::SetEssential(EBackObjType Type)
{
	m_BackObjType = Type;

	m_Pos.x = (float)(rand() % ORIGINAL_GAME_RES_WIDTH);

	SetCullingDelete(ECullingDelete::All, false);

	switch (m_BackObjType)
	{
	case EBackObjType::SmallAstro:
	{
		//애니메이션 지정
		SetAnimation("SmallAstroBody");
		SetAnimation("SmallAstroWindow");
		SetAnimation("SmallAstroTail");

		//이동 위치 지정 및 컬링 설정
		m_Pos.y = 250.f;
		SetCullingDelete(ECullingDelete::Top, true);

		SetRenderLayer(ERenderLayer::Back);	//렌더링 레이어
		SetMaxSpeed(50.f);	//이동속도

		//MoveTo 지정

		MoveToDest(true, EMoveToMethod::MoveDir, Vector2(m_Pos.x, m_Pos.y - 600.f));

		//충돌체 생성, 크기 지정
		CColliderBox* Coll = AddCollider<CColliderBox>("SmallAstroColl");
		Coll->SetCollisionProfile(ECollisionChannel::BackAlly);
		Coll->SetPivot(0.5f, 1.f);
		Coll->SetSize(23.f, 23.f);
		Coll->SetCollisionBeginFunc< CBackObjects>(this, &CBackObjects::CollisionBegin);
	}

		break;
	case EBackObjType::BigAstro:
	{
		//애니메이션 지정
		SetAnimation("BigAstroBody");
		SetAnimation("BigAstroWindow");
		SetAnimation("BigAstroTail");

		//이동 위치 지정 및 컬링 설정
		m_Pos.y = 300.f;
		SetCullingDelete(ECullingDelete::Top, true);

		SetRenderLayer(ERenderLayer::BackFront);
		SetMaxSpeed(100.f);

		//MoveTo 지정
		MoveToDest(true, EMoveToMethod::MoveDir, Vector2(m_Pos.x, m_Pos.y - 600.f));


		//충돌체 생성, 크기 지정
		CColliderBox* Coll = AddCollider<CColliderBox>("BigAstroColl");
		Coll->SetCollisionProfile(ECollisionChannel::BackAlly);
		Coll->SetPivot(0.5f, 1.f);
		Coll->SetSize(38.f, 38.f);
		Coll->SetCollisionBeginFunc< CBackObjects>(this, &CBackObjects::CollisionBegin);
	}
		break;
	case EBackObjType::LaserBack:
	{
		//애니메이션 지정
		SetAnimation("LaserBack");


		//이동 위치 지정 및 컬링 설정
		m_Pos.y = -100.f;
		SetCullingDelete(ECullingDelete::Bottom, true);

		SetRenderLayer(ERenderLayer::Back);
		SetMaxSpeed(100.f);

		//MoveTo 지정
		MoveToDest(true, EMoveToMethod::MoveDir, Vector2(m_Pos.x, m_Pos.y + 500.f));

		//충돌체 생성, 크기 지정
		CColliderBox* Coll = AddCollider<CColliderBox>("LaserBack");
		Coll->SetCollisionProfile(ECollisionChannel::BackEnemy);
		Coll->SetPivot(0.5f, 1.f);
		Coll->SetSize(10.f, 10.f);
		Coll->SetOffset(0.f, 90.f);
		Coll->SetCollisionBeginFunc<CBackObjects>(this, &CBackObjects::CollisionBegin);
	}
		break;

	case EBackObjType::LaserFront:
	{
		//애니메이션 지정
		SetAnimation("LaserFront");

		//이동 위치 지정 및 컬링 설정
		m_Pos.y = -100.f;
		SetCullingDelete(ECullingDelete::Bottom, true);

		SetRenderLayer(ERenderLayer::BackFront);
		SetMaxSpeed(150.f);

		//MoveTo 지정
		MoveToDest(true, EMoveToMethod::MoveDir, Vector2(m_Pos.x, m_Pos.y + 500.f));


		//충돌체 생성, 크기 지정
		CColliderBox* Coll = AddCollider<CColliderBox>("Laser");
		Coll->SetCollisionProfile(ECollisionChannel::BackEnemy);
		Coll->SetPivot(0.5f, 1.f);
		Coll->SetSize(10.f, 10.f);
		Coll->SetOffset(0.f, 120.f);
		Coll->SetCollisionBeginFunc< CBackObjects>(this, &CBackObjects::CollisionBegin);
	}
	break;
	case EBackObjType::DebrisBig:
	{
		//애니메이션 지정
		std::string debris = "DebrisBig";
		debris += std::to_string(rand() % 5);

		SetAnimation(debris);


		//이동 위치 지정 및 컬링 설정
		m_Pos.y = -50.f;
		SetCullingDelete(ECullingDelete::Bottom, true);

		SetRenderLayer(ERenderLayer::BackFront);
		SetMaxSpeed(40.f);

		//MoveTo 지정
		MoveToDest(true, EMoveToMethod::MoveDir, Vector2(m_Pos.x, m_Pos.y + 500.f));
	}
		break;

	case EBackObjType::DebrisSmall:
	{
		//애니메이션 지정
		std::string debris = "DebrisSmall";
		debris += std::to_string(rand() % 7);

		SetAnimation(debris);


		//이동 위치 지정 및 컬링 설정
		m_Pos.y = -50.f;
		SetCullingDelete(ECullingDelete::Bottom, true);

		SetRenderLayer(ERenderLayer::BackFront);
		SetMaxSpeed(20.f);

		//MoveTo 지정
		MoveToDest(true, EMoveToMethod::MoveDir, Vector2(m_Pos.x, m_Pos.y + 500.f));

	}
	break;

	}
}

void CBackObjects::CollisionBegin(CCollider* Src, CCollider* Dest)
{
	if (m_BackObjType == EBackObjType::BigAstro || m_BackObjType == EBackObjType::SmallAstro)
	{
		CExplosion* Boom = m_Scene->CreateObject<CExplosion>("BackExplosion");
		Boom->SetEssential(EExplosionSize::Back, m_Pos);
	}

	SetActive(false);
}




