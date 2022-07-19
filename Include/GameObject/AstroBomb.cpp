#include "AstroBomb.h"

//충돌체
#include "../Collision/ColliderBox.h"
#include "../../Include/Scene/Scene.h"

//폭발 이펙트 생성용
#include "DamageExplosion.h"

//충돌함수에서 DELTA_TIME 접근
#include "../GameManager.h"


CAstroBomb::CAstroBomb() :
	m_Target(),
	m_MaxTurnDirX(0.5f),
	m_isRight(),
	m_Delay(),
	m_StartIgnition()
{
	SetTypeID<CAstroBomb>();
}

CAstroBomb::CAstroBomb(const CAstroBomb& Obj) :
	CProjectile(Obj),
	m_Target(Obj.m_Target),
	m_MaxTurnDirX(Obj.m_MaxTurnDirX),
	m_isRight(),
	m_Delay(Obj.m_Delay),
	m_StartIgnition(Obj.m_StartIgnition)
{
}

CAstroBomb::~CAstroBomb()
{
}

bool CAstroBomb::Init(CGameObject* Obj)
{
	CProjectile::Init(Obj);

	if (!m_Animation->FindAnimInfo(m_Name))
		return false;

	SetAnimation(m_Name);

	if (m_Name == "BombRight")
		m_isRight = true;
	else if (m_Name == "BombLeft")
		m_isRight = false;

	m_MaxSpeed = 20.f;
	SetSize(16.f, 48.f);

	//렌더링레이어
	SetRenderLayer(ERenderLayer::EffectLow);

	//충돌체 1 설정 -> 닿았을 때 폭발하는 용도
	CColliderBox* Coll = AddCollider<CColliderBox>("AstroBombColl");
	Coll->SetCollisionProfile(ECollisionChannel::PlayerAttack);
	Coll->SetSize(m_Size);
	Coll->SetCollisionBeginFunc(this, &CAstroBomb::CollisionBegin);
	Coll->SetOffset(0.f, -45.f);


	//m_Damage = 0;	자체 데미지는 없음. 전방 범위 스플래시 데미지만 있음.
	if (m_isRight)
	{
		SetDir(1.f, 1.f);	//방향 설정
		SetPivot(0.f, 0.f);

		//endfunction -> 로켓 자리잡히면 점화되는 용도
		SetEndFunction<CAstroBomb>("BombRight", this, &CAstroBomb::AstroBombStartEnd);

		Coll->SetPivot(m_Pivot);
		
	}
	else
	{
		SetDir(-1.f, 1.f);
		SetPivot(1.f, 0.f);

		//endfunction -> 로켓 자리잡히면 점화되는 용도
		SetEndFunction<CAstroBomb>("BombLeft", this, &CAstroBomb::AstroBombStartEnd);

		Coll->SetPivot(m_Pivot);
		
	}

	
	




	Coll = nullptr;
	//충돌체 2 설정 -> 닿았을 때 유도되는 용도
	Coll = AddCollider<CColliderBox>("AstroBombHomming");
	Coll->SetCollisionProfile(ECollisionChannel::PlayerAttack);

	//게임 화면 크기의 충돌체를 설정
	Coll->SetSize(100.f, (float)ORIGINAL_GAME_RES_HEIGHT);
	Coll->SetPivot(0.5f, 1.f);
	Coll->SetCollisionBeginFunc(this, &CAstroBomb::HommingBegin);



	//컬링 설정
	SetCullingDelete(ECullingDelete::Bottom, false);



	m_isReady = true;

	return true;
}



bool CAstroBomb::LoadResource()
{
	if (!LoadCSVPreset(TEXT("SFX/AstroBomb.csv")))
		return false;

	return true;
}

void CAstroBomb::Update(float DeltaTime)
{
	CProjectile::Update(DeltaTime);


	if (CheckColliding(m_Target))
	{
		//x축에서 얼마나 떨어져있는지를 체크
		float PosX = m_Pos.x - m_Target->GetPos().x;

		if (PosX > 0.f)	//양수이면 왼쪽에 있다는 뜻이므로
			m_Dir.x -= m_MaxTurnDirX * DeltaTime;
		else if (PosX < 0.f)
			m_Dir.x += m_MaxTurnDirX * DeltaTime;
	}
	else
	{
		m_Target = nullptr;
	}

	if (m_StartIgnition)
	{
		//딜레이동안에는 느리게
		m_Delay -= DeltaTime;
		if (m_Delay >= 0.f)
		{
			AddForce(100.f);
		}
		else
		{
			AddForce(800.f);

			//최대 속도 800으로 
			m_MaxSpeed = 800.f;
		}
	}
}

void CAstroBomb::SetEssential(float PosX, float PosY, bool SlowIgnition)
{
	m_isReady = true;

	m_Pos.x = PosX;
	m_Pos.y = PosY;

	if (SlowIgnition)
	{
		m_Delay = 0.2f;
	}
}




void CAstroBomb::AstroBombStartEnd()
{
	SoundPlay("RocketLauncherFire");

	//애니메이션 추가
	SetAnimation("BombIgnition");

	if (!m_isRight)
		m_Animation->SetOffset("BombIgnition", -9.f, -11.f);
	else
		m_Animation->SetOffset("BombIgnition", 10.f, -11.f);

	//방향 변경
	SetDir(0.f, -1.f);
	
	//Update에 점화되었다고 전달
	m_StartIgnition = true;
}

void CAstroBomb::CollisionBegin(CCollider* Src, CCollider* Dest)
{
	SoundPlayOneChan("RocketLauncherHit");

	//피격이펙트 표시. 점수 처리는 충돌체에서 함.
	CDamageExplosion* SFX = m_Scene->CreateObject<CDamageExplosion>("BigExplosion");
	SFX->SetEssential(EExplosionSize::Big, Src->GetHitPoint(), 100);

	//Dest->GetOwnerObj()->InflictDamage(m_Damage);

	SetActive(false);
}

void CAstroBomb::HommingBegin(CCollider* Src, CCollider* Dest)
{
	//타겟이 설정되어있지 않으면 가장 먼저 충돌판정 난 대상을 타겟으로 지정
	if (!m_Target)
		m_Target = Dest->GetOwnerObj();
}
