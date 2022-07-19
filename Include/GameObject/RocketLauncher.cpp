#include "RocketLauncher.h"

//충돌체
#include "../Collision/ColliderBox.h"
#include "../../Include/Scene/Scene.h"

//폭발 이펙트 생성용
#include "RocketLauncherExplosion.h"

//연기 생성용
#include "JustPlay.h"

//로켓 제한수 접근용
#include "Player.h"

//충돌함수에서 DELTA_TIME 접근
#include "../GameManager.h"

#include "../Scene/SceneResource.h"


CRocketLauncher::CRocketLauncher():
	m_TrailTimer(0.1f),
	m_TrailOn(),
	m_Target(),
	m_MaxTurnDirX(0.5f)
{
	SetTypeID<CRocketLauncher>();
}

CRocketLauncher::CRocketLauncher(const CRocketLauncher& Obj) :
	CProjectile(Obj),
	m_TrailTimer(Obj.m_TrailTimer),
	m_TrailOn(),
	m_Target(Obj.m_Target),
	m_MaxTurnDirX(Obj.m_MaxTurnDirX)
{
}

CRocketLauncher::~CRocketLauncher()
{
	m_Scene->GetPlayer()->DecreaseRocket();
}

bool CRocketLauncher::Init(CGameObject* Obj)
{
	CProjectile::Init(Obj);
	
	//로켓 애니메이션 지정
	SetAnimation("Rocket_Start");

	SetRenderLayer(ERenderLayer::EffectHigh);
	m_MaxSpeed = 50.f;
	//m_Damage = 0;	자체 데미지는 없음. 연쇄 스플래시 데미지만 있음
	SetDir(0.f, -1.f);	//방향 설정

	SetSize(10.f, 25.f);
	SetPivot(0.5f, 1.f);

	//충돌체 1 설정 -> 닿았을 때 폭발하는 용도
	CColliderBox* Coll = AddCollider<CColliderBox>("RocketColl");
	Coll->SetCollisionProfile(ECollisionChannel::PlayerAttack);
	Coll->SetSize(m_Size);
	Coll->SetPivot(0.5f, 1.f);
	Coll->SetOffset(0.f, -25.f);
	Coll->SetCollisionBeginFunc(this, &CRocketLauncher::CollisionBegin);


	Coll = nullptr;
	//충돌체 2 설정 -> 닿았을 때 유도되는 용도
	Coll = AddCollider<CColliderBox>("Homming");
	Coll->SetCollisionProfile(ECollisionChannel::PlayerAttack);
	
	//게임 화면 크기의 충돌체를 설정
	Coll->SetSize(100.f, (float)ORIGINAL_GAME_RES_HEIGHT);
	Coll->SetPivot(0.5f, 1.f);
	Coll->SetOffset(0.f, -25.f);
	Coll->SetCollisionBeginFunc(this, &CRocketLauncher::HommingBegin);

	//endfunction
	SetEndFunction< CRocketLauncher>("Rocket_Start", this, &CRocketLauncher::RocketStartEnd);


	//컬링 설정
	SetCullingDelete(ECullingDelete::Bottom, false); 

	SoundPlay("RocketLauncherFire");
	

	m_isReady = true;

	return true;
}

bool CRocketLauncher::LoadResource()
{
	if (!LoadCSVPreset(TEXT("SFX/RocketLauncher.csv")))
		return false;

	return true;
}

void CRocketLauncher::Update(float DeltaTime)
{
	CProjectile::Update(DeltaTime);

	//연기 출력
	m_TrailTimer -= DeltaTime;
	if (m_TrailTimer < 0.f && m_TrailOn)
	{
		m_TrailTimer = 0.05f;

		CJustPlay* Trail = m_Scene->CreateObject<CJustPlay>("RocketTrail");

		Trail->SetEssential(m_Pos);
	}



	


	if (CheckColliding(m_Target))
	{
		//x축에서 얼마나 떨어져있는지를 체크
		float PosX = m_Pos.x - m_Target->GetPos().x;

		if (PosX > 0.f)	//양수이면 왼쪽에 있다는 뜻이므로
			m_Dir.x -= m_MaxTurnDirX * DeltaTime;
		else if (PosX < 0.f)
			m_Dir.x += m_MaxTurnDirX * DeltaTime;

		SetDir(m_Dir);	//정규화
	}
	else
	{
		m_Target = nullptr;
	}



	//주어진 방향 벡터 방향으로 이동
	AddForce(100.f);
}



void CRocketLauncher::RocketStartEnd()
{
	m_TrailOn = true;

	m_MaxSpeed = 150.f;

	ChangeAnimation("Rocket_Ignition");


}

void CRocketLauncher::CollisionBegin(CCollider* Src, CCollider* Dest)
{
	m_Scene->GetSceneResource()->SoundPlay("RocketLauncherHit");

	CRocketLauncherExplosion* SFX = m_Scene->CreateObject<CRocketLauncherExplosion>("Rocket_SmallExplosion");

	//피격이펙트 표시
	SFX->SetEssential(m_Dir, Src->GetHitPoint());

	//Dest->GetOwnerObj()->InflictDamage(m_Damage);

	SetActive(false);
}

void CRocketLauncher::HommingBegin(CCollider* Src, CCollider* Dest)
{
	//타겟이 설정되어있지 않으면 가장 먼저 충돌판정 난 대상을 타겟으로 지정
	if (!m_Target)
		m_Target = Dest->GetOwnerObj();
}
