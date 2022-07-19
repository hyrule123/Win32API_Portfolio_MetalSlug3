#include "MiniUFOElite.h"

#include "Player.h"

#include "../Scene/MainScene.h"

//발사체
#include "ProjectileEnemy.h"

//이펙트
#include "Effect.h"

//충돌체
#include "../Collision/ColliderBox.h"

//위치 계산 카메라


CMiniUFOElite::CMiniUFOElite():
	m_HitTimer(),
	m_MainScene(),
	m_SerialFire(),
	m_InAction()
{
	SetTypeID<CMiniUFOElite>();
}

CMiniUFOElite::CMiniUFOElite(const CMiniUFOElite& Obj) :
	CMiniUFO(Obj),
	m_HitTimer(Obj.m_HitTimer),
	m_MainScene(Obj.m_MainScene),
	m_SerialFire(Obj.m_SerialFire),
	m_InAction(Obj.m_InAction)
{
}

CMiniUFOElite::~CMiniUFOElite()
{
}

bool CMiniUFOElite::LoadResource()
{
	CreateAnimation();

	if (!LoadCSVPreset(TEXT("Enemy/MiniUFOElite.csv")))
		return false;

	return true;
}

bool CMiniUFOElite::Init(CGameObject* Obj)
{

	CEnemy::Init(Obj);

	SetCullingDelete(ECullingDelete::All, false);
	SetCullingDelete(ECullingDelete::CullingOn, false);

	m_MainScene = dynamic_cast<CMainScene*>(m_Scene);
	if (!m_MainScene)
		return false;


	SetAnimation("MiniUFOElite");

	m_HP = 150;
	m_MaxSpeed = 200.f;
	SetDeAccel(0.8f);
	m_Size.Set(30.f, 30.f);

	CColliderBox* Coll = AddCollider<CColliderBox>("MiniUFOElite");
	Coll->SetCollisionProfile(ECollisionChannel::Enemy);
	Coll->SetPivot(m_Pivot);
	Coll->SetSize(m_Size);

	//CColliderCircle* Coll = AddCollider<CColliderCircle>("MiniUFO");
	//Coll->SetRadius(50.f);

	//End Function
	//사망모션이후 제거
	SetEndFunction<CMiniUFOElite>("MiniUFOEliteDeath", this, &CMiniUFOElite::MiniUFOEliteDeathEndFunc);
	//공격
	SetEndFunction<CMiniUFOElite>("EliteAttackSFX", this, &CMiniUFOElite::AttackEndFunc);


	//성공적으로 생성되면 자신이 등록되었음을 알림
	CMainScene* Scene = dynamic_cast<CMainScene*>(m_Scene);
	if (Scene)
		Scene->AddMiniUFOCounter(1);


	// 1/4 확률로 무기 드롭
	if (rand() % 3 == 0)
	{
		int RandWeapon = rand() % ((int)EItemList::Bomb + 1);
		SetItemDrop((EItemList)RandWeapon);
	}

	return true;
}


void CMiniUFOElite::Update(float DeltaTime)
{

	CEnemy::Update(DeltaTime);

	if (m_HP > 0)
	{
		Routine(DeltaTime);
		if (m_HitTimer > 0.f)
		{
			m_HitTimer -= DeltaTime;
			if (m_HitTimer < 0.f)
			{
				ChangeAnimContinue("MiniUFOElite");
			}
		}
	}

	//보스전 돌입이 확인되면 즉시 제거
	if (m_MainScene->CheckCutscenePlay())
	{
		if (m_HP > 0)
			InflictDamage(10000);
	}


		
}

void CMiniUFOElite::PostUpdate(float DeltaTime)
{
	CMiniUFO::PostUpdate(DeltaTime);


}


void CMiniUFOElite::SetEssential(float DestPosX)
{
	m_MoveToDest.x = DestPosX;
}

int CMiniUFOElite::InflictDamage(int Damage)
{
	//아이템 드랍은 CEnemy에서 처리.
	int DMG = CEnemy::InflictDamage(Damage);

	m_HP -= DMG;

	//0.1초간 피격 애니메이션을 재생
	m_HitTimer = 0.1f;
	ChangeAnimContinue("MiniUFOEliteHit");

	if (m_HP <= 0)
	{
		SoundPlay("MiniUFODestroy");

		//이동 중지
		m_Speed = 0;
		m_MoveToOn = false;


		ItemDrop();
		ClearCollider();

		//여기서는 고유 사망 애니메이션을 재생.
		StopAnimation("EliteAttackSFX");	//공격 애니메이션 레이어
		ChangeAnimation("MiniUFOEliteDeath");
	}

	return DMG;
}

void CMiniUFOElite::Start()
{
	m_Start = true;


}


void CMiniUFOElite::Routine(float DeltaTime)
{
	switch (m_Routine)
	{
	case (UINT8)EMiniUFORoutine::Delay:
	{
		if (CheckFirstEnter())
		{
			m_Timer.InitCooltime(0.5f);
			m_Timer.EnterCooltime();
		}
		if (m_Timer.UpdateCooltime(DeltaTime))
			GoNextRoutine();

		break;
	}

	case (UINT8)EMiniUFORoutine::Moveto:
	{
		if (CheckFirstEnter())
		{
			float RandX = (float)(rand() % (ORIGINAL_GAME_RES_WIDTH - 40) + 40);
			float RandY = (float)(rand() % 60 + 40);
			MoveToDest(true, EMoveToMethod::AddForce, Vector2(RandX, RandY));
		}

		//이동 종료되면 다음 루틴으로
		if (!m_MoveToOn)
			GoNextRoutine();
	}
	break;

	case (UINT8)EMiniUFORoutine::Wait:
	{
		if (CheckFirstEnter())
		{
			//0~1초간 대기
			m_Timer.InitCooltime((float)(rand()% 11 + 10) / 10.f);
			m_Timer.EnterCooltime();
		}
		if (m_Timer.UpdateCooltime(DeltaTime))
			GoNextRoutine();

		break;
	}

	case (UINT8)EMiniUFORoutine::Attack:
	{
		if (CheckFirstEnter())
		{
			//애니메이션을 재생하면 알아서 총 발사함
			m_SerialFire = 2;
			m_InAction = true;
			ChangeAnimation("EliteAttackSFX");
		}

		if (!m_InAction)
		{
			//액션이 끝나면 다시 처음으로 돌아간다.
			m_Routine = 0;
			m_FirstEnter = true;
		}

	}
	break;
	}
}


void CMiniUFOElite::AttackEndFunc()
{
	//총을 발사
	CPlayer* Player = m_Scene->GetPlayer();
	if (Player)
	{
		SoundPlay("MiniUFOLaser");

		Vector2 Dir = (Player->GetPos() - m_Pos).Normalize();
		CProjectileEnemy* Proj = m_Scene->CreateObject<CProjectileEnemy>("MiniUFOEliteProj");
		Proj->SetEssential(EProjectileEnemy::MiniUFOAttack, Dir, m_Pos);
	}

	--m_SerialFire;
	if (m_SerialFire <= 0)
	{
		m_InAction = false;
		StopAnimation("EliteAttackSFX");
	}
		
}

void CMiniUFOElite::MiniUFOEliteDeathEndFunc()
{
	SetActive(false);
}
