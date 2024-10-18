#include "PlayerSpace.h"
#include "../GameManager.h"
#include "../input.h"
#include "../Scene/Scene.h"

#include "../Scene/SceneResource.h"
#include "../Scene/SceneManager.h"

//좌우 애니메이션 재생 관련 입력 확인용
#include "../Input.h"

//충돌체
#include "../Collision/ColliderBox.h"

#include "Pistol.h"

//무기
#include "PlayerHMG.h"
#include "Laser.h"
#include "Shotgun.h"
#include "RocketLauncher.h"
#include "AstroSlugSFX.h"	//로켓 발사 이펙트가 여깄음
#include "AstroBomb.h"

#include "../UI/WindowSpace.h"

//애니메이션 재생
#include "JustPlay.h"

//페이드인아웃
#include "../UI/WidgetFadeInOut.h"

//가스 점수 재생
#include "../UI/WidgetScore.h"

//게임오버
#include "../Scene/GameOver.h"


CPlayerSpace::CPlayerSpace():
	m_DirTime(),
	m_DirTimeMax(0.2f),
	m_Tolerance(0.03f),
	m_CurrentAct(EDirAction::Idle_Pistol),
	m_PrevAct(EDirAction::Idle_Pistol),
	m_CurrentDir(EDirSpace::Mid),
	m_PrevDir(EDirSpace::Mid),
	m_CurrentWeapon(EWeapon::Pistol),
	m_NoActionChange(true),
	m_GunFired(),
	m_HMGQueue(),
	m_HMGDelay(0.06f),	//헤비머신건의 발사 딜레이
	m_HMGDir(-1),
	m_Offset(2.f, -65.f),
	m_BombTurn()
{
	SetTypeID<CPlayerSpace>();

	m_RenderLayer = ERenderLayer::Player;
	m_DirTimeMaxHalf = m_DirTimeMax / 2.f;
	m_HP = 1;
}

CPlayerSpace::CPlayerSpace(const CPlayerSpace& Obj) :
	CPlayer(Obj),
	m_DirTime(Obj.m_DirTime),
	m_DirTimeMax(Obj.m_DirTimeMax),
	m_Tolerance(Obj.m_Tolerance),
	m_CurrentAct(Obj.m_CurrentAct),
	m_PrevAct(Obj.m_PrevAct),
	m_CurrentDir(Obj.m_CurrentDir),
	m_PrevDir(Obj.m_PrevDir),
	m_CurrentWeapon(Obj.m_CurrentWeapon),
	m_NoActionChange(Obj.m_NoActionChange),
	m_GunFired(Obj.m_GunFired),
	m_HMGQueue(Obj.m_HMGQueue),
	m_HMGDelay(Obj.m_HMGDelay),
	m_HMGDir(Obj.m_HMGDir),
	m_Offset(Obj.m_Offset),
	m_BombTurn(Obj.m_BombTurn)
{
	int iMax = (int)EDirSpace::MAX;
	int jMax = (int)EDirAction::MAX;
	for (int i = 0; i < iMax; ++i)
	{
		for (int j = 0; j < jMax; ++j)
		{
			m_vecAnimName[i][j] = Obj.m_vecAnimName[i][j];
		}
	}

	for (int i = 0; i < iMax; ++i)
	{
		m_vecTailBoosterName[i] = Obj.m_vecTailBoosterName[i];

		for (int j = 0; j < EDirLeftRight::LeftRightEnd; ++j)
		{
			m_vecSideBoosterOffset[i][j] = Obj.m_vecSideBoosterOffset[i][j];
		}
	}


}

CPlayerSpace::~CPlayerSpace()
{
	CInput::GetInst()->DeleteBindClass<CPlayerSpace>(this);
}

bool CPlayerSpace::LoadResource()
{
	/////////////////// 애니메이션 생성 ///////////////////
	CreateAnimation();
	if(!LoadCSVPreset(TEXT("Player/PlayerSpace.csv")))
		return false;

	if (!LoadCSVPreset(TEXT("Player/PlayerSpaceSFX.csv")))
		return false;

	RegisterAnimName();

	return true;
}

bool CPlayerSpace::Init(CGameObject* Obj)
{
	CPlayer::Init(Obj);

	


	//위치 설정
	SetPos(50.f, 50.f);
	SetSize(22.f, 50.f);
	SetDir(1.f, 0.f);
	SetPivot(0.5f, 1.f);
	SetScale(1.f);
	SetMaxSpeed(150.f);
	SetDeAccel(1.f);


	//스킬 설정
	//스킬 설정
	m_vecCooltime.resize((size_t)EWeapon::MAX);

	for (int i = 0; i < (int)EWeapon::MAX; ++i)
	{
		switch ((EWeapon)i)
		{
		case EWeapon::Pistol:
			m_vecCooltime[(int)EWeapon::Pistol].Cooltime = 0.1f;
			break;
		case EWeapon::HMG:
			m_vecCooltime[(int)EWeapon::HMG].Cooltime = 0.15f;
			break;
		case EWeapon::Shotgun:
			m_vecCooltime[(int)EWeapon::Shotgun].Cooltime = 0.5f;
			break;
		case EWeapon::Rocket:
			m_vecCooltime[(int)EWeapon::Rocket].Cooltime = 0.2f;
			break;
		case EWeapon::Laser:
			m_vecCooltime[(int)EWeapon::Laser].Cooltime = 0.25f;
			break;
		case EWeapon::Bomb:
			m_vecCooltime[(int)EWeapon::Bomb].Cooltime = 0.3f;
			break;
		default:
			break;
		}
	}


	//공격 종료 EndFunction 등록
	for (int i = (int)EDirSpace::Left2; i < (int)EDirSpace::MAX; ++i)
	{
		AddNotify< CPlayerSpace>(m_vecAnimName[i][(int)EDirAction::Idle_Pistol], 0, this, &CPlayerSpace::IdleStatusNotify);

		AddNotify< CPlayerSpace>(m_vecAnimName[i][(int)EDirAction::Idle_Rifle], 0, this, &CPlayerSpace::IdleStatusNotify);

		SetEndFunction< CPlayerSpace>(m_vecAnimName[i][(int)EDirAction::Idle_Pistol], this, &CPlayerSpace::IdleStatusNotify);

		SetEndFunction< CPlayerSpace>(m_vecAnimName[i][(int)EDirAction::Idle_Rifle], this, &CPlayerSpace::IdleStatusNotify);

		SetEndFunction(
			m_vecAnimName[i][(int)EDirAction::Attack_Pistol], 
			this, &CPlayerSpace::AttackEnd);

		SetEndFunction(
			m_vecAnimName[i][(int)EDirAction::Attack_HMG], 
			this, &CPlayerSpace::HMGLaserAtkEnd);

		SetEndFunction(
			m_vecAnimName[i][(int)EDirAction::Attack_Laser],
			this, &CPlayerSpace::HMGLaserAtkEnd);
		
		SetEndFunction(
			m_vecAnimName[i][(int)EDirAction::Attack_Rocket],
			this, &CPlayerSpace::AttackEnd);

		SetEndFunction(
			m_vecAnimName[i][(int)EDirAction::Attack_Shotgun],
			this, &CPlayerSpace::AttackEnd);

		SetEndFunction(
			m_vecAnimName[i][(int)EDirAction::Rifle_Drop],
			this, &CPlayerSpace::RifleDropEnd);
		
		SetEndFunction(
			m_vecAnimName[i][(int)EDirAction::Death],
			this, &CPlayerSpace::DeathEnd);

		SetEndFunction(
			m_vecAnimName[i][(int)EDirAction::Slug_Ejection],
			this, &CPlayerSpace::SlugEjectionEnd);
	}

	//HMG 노티파이 등록
	for (int i = (int)EDirSpace::Left2; i < (int)EDirSpace::MAX; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			AddNotify<CPlayerSpace>(m_vecAnimName[i][(int)EDirAction::Attack_HMG], j, this, &CPlayerSpace::FireHMG);
		}

	}
	
	////충돌체 설정
	CColliderBox* Collider = AddCollider<CColliderBox>("PlayerBox");

	Collider->SetSize(m_Size);
	Collider->SetCollisionProfile(ECollisionChannel::Player);
	Collider->SetPivot(0.5f, 1.f);





	//남은 총알 수 전달
	m_WindowSpace->SetBulletLeft(m_Bullet);
	m_WindowSpace->SetBombLeft(m_Bomb);



	return true;
}



void CPlayerSpace::ComputeCurrentDir(float DeltaTime)
{
	//움직임이 없을 경우 원점으로 돌아옴
	if (fabs(m_MoveDist.x) <= FLT_EPSILON)
	{
		if (m_DirTime > m_Tolerance)
			m_DirTime -= DeltaTime;
		else if (m_DirTime < -m_Tolerance)
			m_DirTime += DeltaTime;
	};


	//여기까지 했으면 한 프레임의 방향 계산이 끝난것임. 방향 확정
	//10을 곱한뒤 정수로 바꾸어서 소수점을 다 바꿔준뒤
	//열거체에 맞게 더해줌
	//ex)-0.25f(맨 왼쪽) -> -2.5f -> -2 + 2(최댓값만큼 더해준값)
	//->인덱스에 맞는 값이 나옴
	// 
	m_CurrentDir = (EDirSpace)
		((int)(m_DirTime * 10.f) + (int)(m_DirTimeMax * 10.f));


	if (m_CurrentDir > EDirSpace::Right2)
		m_CurrentDir = EDirSpace::Right2;
	else if (m_CurrentDir < EDirSpace::Left2)
		m_CurrentDir = EDirSpace::Left2;

}

void CPlayerSpace::ComputeCurrentAct(float DeltaTime)
{
	//총 안쐈으면 아이들 모션(총 쐈으면 총 발사 메소드에서 액션 변경됨)
	if (!m_GunFired && !m_NoActionChange)
	{
		if (m_CurrentWeapon == EWeapon::Pistol)
			m_CurrentAct = EDirAction::Idle_Pistol;
		else
			m_CurrentAct = EDirAction::Idle_Rifle;
	}
}


void CPlayerSpace::MoveLeft()
{
	if (m_DirTime > 0.f)
		m_DirTime -= DELTA_TIME * 2.f;
	else
		m_DirTime -= DELTA_TIME;

	if (m_DirTime <= -1.f * m_DirTimeMax)//최댓값을 고정(+오차값)
		m_DirTime = -1.f * (m_DirTimeMax + m_Tolerance);
	SetDir(-1.f, 0.f);
	MoveDir(m_Dir);
}

void CPlayerSpace::MoveRight()
{
	if (m_DirTime < 0.f)
		m_DirTime += DELTA_TIME * 2.f;
	else
		m_DirTime += DELTA_TIME;


	if (m_DirTime >= m_DirTimeMax)//최댓값을 고정
		m_DirTime = m_DirTimeMax + m_Tolerance;

	SetDir(1.f, 0.f);
	MoveDir(m_Dir);
}




void CPlayerSpace::MoveUp()
{
	SetDir(0.f, -1.f);
	MoveDir(m_Dir);
}

void CPlayerSpace::MoveDown()
{
	SetDir(0.f, 1.f);
	MoveDir(m_Dir);
}

void CPlayerSpace::LeftKeyDown()
{
	ChangeAnimation("RightBoost", true);
	
	SetAnimOffset(
		"RightBoost",
		m_vecSideBoosterOffset[(int)m_CurrentDir][DirRight].x,
		m_vecSideBoosterOffset[(int)m_CurrentDir][DirRight].y);
}

void CPlayerSpace::RightKeyDown()
{
	ChangeAnimation("LeftBoost", true);

	SetAnimOffset(
		"LeftBoost",
		m_vecSideBoosterOffset[(int)m_CurrentDir][DirLeft].x,
		m_vecSideBoosterOffset[(int)m_CurrentDir][DirLeft].y);
}

void CPlayerSpace::IdleStatusNotify()
{
	m_GunFired = false;
}

void CPlayerSpace::FireGun()
{
	++m_AttackBuffer;
}

void CPlayerSpace::FireGunCheck()
{
	if (m_NoActionChange || m_GunFired)
		return;

	switch (m_CurrentWeapon)
	{
	case EWeapon::Pistol:
	{
		if (!m_vecCooltime[(int)EWeapon::Pistol].isCooltime)
		{
			EnterSkillCoolTIme((int)EWeapon::Pistol);

			m_CurrentAct = EDirAction::Attack_Pistol;
			CPistol* Pistol = m_Scene->CreateObject<CPistol>("Pistol");
			Pistol->SetEssential(Vector2(0.f, -1.f), Vector2(m_Pos.x, m_Pos.y - 80.f));

			m_AttackBuffer = 0;
			m_GunFired = true;
		}
		break;
	}
	case EWeapon::HMG:
		if (!m_vecCooltime[(int)EWeapon::HMG].isCooltime)
		{
			SoundPlay("HMGFire");

			EnterSkillCoolTIme((int)EWeapon::HMG);

			m_CurrentAct = EDirAction::Attack_HMG;

			m_HMGQueue = 4;

			m_AttackBuffer = 0;
			m_GunFired = true;
		}
		break;
	case EWeapon::Shotgun:
		if (!m_vecCooltime[(int)EWeapon::Shotgun].isCooltime)
		{
			EnterSkillCoolTIme((int)EWeapon::Shotgun);

			m_CurrentAct = EDirAction::Attack_Shotgun;

			FireShotgun();

			m_AttackBuffer = 0;
			m_GunFired = true;
		}
		
		break;
	case EWeapon::Rocket:
		if (!m_vecCooltime[(int)EWeapon::Rocket].isCooltime)
		{
			if (m_FiredRocket >= 2)	//만약 맵에 2발 이상 로켓이 발사됐으면 return
				break;
			//쿨타임이 아니면 쿨타임으로 바꾸는 메소드를 호출하고 로직 작성
			EnterSkillCoolTIme((int)EWeapon::Rocket);

			FireRocket();

			m_AttackBuffer = 0;
			m_GunFired = true;
		}

		
		break;
	case EWeapon::Laser:
		if (!m_vecCooltime[(int)EWeapon::Laser].isCooltime)
		{
			EnterSkillCoolTIme((int)EWeapon::Laser);

			FireLaser();

			//선입력 버퍼 초기화
			m_AttackBuffer = 0;
			m_GunFired = true;
		}

		
		break;
	default:
		break;
	}

	
}

void CPlayerSpace::FireBomb()
{
	if (m_Bomb <= 0)
		return;

	if (m_vecCooltime[(int)EWeapon::Bomb].isCooltime)
		return;

	//쿨타임이 아니면 쿨타임으로 바꾸는 메소드를 호출하고 로직 작성
	EnterSkillCoolTIme((int)EWeapon::Bomb);


	if (m_BombTurn)
	{
		CAstroBomb* Bomb = m_Scene->CreateObject<CAstroBomb>("BombRight");
		if (Bomb)
			Bomb->SetEssential(
				m_Pos.x + 10.f, m_Pos.y, m_BombTurn);
	}
	else
	{
		CAstroBomb* Bomb = m_Scene->CreateObject<CAstroBomb>("BombLeft");
		if (Bomb)
			Bomb->SetEssential(
				m_Pos.x - 10.f, m_Pos.y, !m_BombTurn);
	}

	--m_Bomb;
	m_WindowSpace->SetBombLeft(m_Bomb);

	m_BombTurn = !m_BombTurn;

}

void CPlayerSpace::FireNotify()
{
	////스킬 로직 진행
	//CProjectile* Bullet = m_Scene->CreateObject<CProjectile>("PlayerBullet", this);
	//Bullet->SetMaxVelocityDir(1000.f, m_Dir);
	//Bullet->SetPos(m_Pos);
	//Bullet->SetDamage(20.f);
}

void CPlayerSpace::RifleDrop()
{
	if (m_CurrentWeapon == EWeapon::Pistol)
		return;

	m_CurrentAct = EDirAction::Rifle_Drop;

	//총 발사 상태를 true로 변경하여 총발사 불가능하게 하고 무기 변경
	m_GunFired = true;
	m_CurrentWeapon = EWeapon::Pistol;
	m_WindowSpace->SetBulletLeft(m_Bullet);
}

void CPlayerSpace::RifleDropEnd()
{
	m_GunFired = false;
}

void CPlayerSpace::ChangeWeapon(EWeapon Weapon)
{
	switch (Weapon)
	{
	case EWeapon::HMG:
		ChangeRifleHMG();
		SoundPlayOneChan("GetHMG");
		break;
	case EWeapon::Shotgun:
		ChangeRifleShotgun();
		SoundPlayOneChan("GetShotgun");
		break;
	case EWeapon::Rocket:
		ChangeRifleRocket();
		SoundPlayOneChan("GetRocket");
		break;
	case EWeapon::Laser:
		ChangeRifleLaser();
		SoundPlayOneChan("GetLaser");
		break;
	case EWeapon::Bomb:
		m_Bomb += 10;
		m_WindowSpace->SetBombLeft(m_Bomb);
		SoundPlayOneChan("OK");
		break;
	}

	CPlayer::ChangeWeapon(Weapon);
}

void CPlayerSpace::ChangeRifleHMG()
{
	if (m_CurrentWeapon == EWeapon::HMG)
	{
		m_Bullet += 200;
	}
		
	else
	{
		m_CurrentWeapon = EWeapon::HMG;
		m_Bullet = 200;
	}
}

void CPlayerSpace::ChangeRifleShotgun()
{
	if (m_CurrentWeapon == EWeapon::Shotgun)
	{
		m_Bullet += 30;
	}
		
	else
	{
		m_CurrentWeapon = EWeapon::Shotgun;
		m_Bullet = 30;
	}
}

void CPlayerSpace::ChangeRifleRocket()
{
	if (m_CurrentWeapon == EWeapon::Rocket)
	{
		m_Bullet += 30;
	}
		
	else
	{
		m_CurrentWeapon = EWeapon::Rocket;
		m_Bullet = 30;
	}
}

void CPlayerSpace::ChangeRifleLaser()
{
	if (m_CurrentWeapon == EWeapon::Laser)
	{
		m_Bullet += 200;
	}
	else
	{
		m_CurrentWeapon = EWeapon::Laser;
		m_Bullet = 200;
	}
}

void CPlayerSpace::ChangePistol()
{
	m_CurrentWeapon = EWeapon::Pistol;
}

void CPlayerSpace::GetBomb()
{
	m_Bomb += 10;
}

void CPlayerSpace::GetGas()
{
	CWidgetWindow* SpaceWindow = m_Scene->FindWidgetWindow("WindowSpace");

	if (SpaceWindow)
	{
		CWidgetScore* ScoreWind = SpaceWindow->CreateWidget<CWidgetScore>("Score");
		if (ScoreWind)
		{
			ScoreWind->SetText(1000);
			ScoreWind->SetPos(m_Pos);
		}
	}

	GetItem(1000);
}

void CPlayerSpace::Death()
{
	//한번만 진입
	if (m_HP <= -10)
		return;
	m_HP = -10;
	
	m_SideCollOn = false;

	//모든 키바인딩을 제거
	CInput::GetInst()->DeleteBindClass<CPlayerSpace>(this);
	ClearCollider();

	StopAnimation(1);

	SoundPlayOneChan("TarmaDeath");

	m_CurrentAct = EDirAction::Death;
	m_NoActionChange = true;
	SetDir(-1.f, 0.f);
	SetMaxSpeed(50.f);
	AddImpact(200.f);


	SetDeAccel(0.98f);
	SetGravityAccel(9.8f);
	SetPhysicsSimulate(true);
	Jump();
}

void CPlayerSpace::DeathEnd()
{
	//바로 죽으니까 그럴듯하지가 않음..
	//데스 타이머같은걸 하나 만들어서 좀 더 시간을 연장해야 할듯
	//컬링으로 해결
	CWidgetWindow* Window = m_Scene->FindWidgetWindow("WindowSpace");
	if (Window)
	{
		CWidgetFadeInOut* Fade = Window->FadeInOut(false);
		if (Fade)
		{
			Fade->AddEndFunction<CPlayerSpace>(this, &CPlayerSpace::SetGameOver);
		}
	}

	SetCullingDelete(ECullingDelete::Bottom, true);
}

void CPlayerSpace::SlugEjection()
{
	m_CurrentAct = EDirAction::Slug_Ejection;
	SetInvincible(3.f);
	m_NoActionChange = true;
}

void CPlayerSpace::SlugEjectionEnd()
{
	m_CurrentAct = EDirAction::Idle_Pistol;
	m_NoActionChange = false;

	RegisterInput();

	SetAnimation("TailBoostMid");

}

void CPlayerSpace::SetGameOver()
{
	//게임오버 씬으로 전환하고
	CSceneManager::GetInst()->ChangeGameOver();

	//다이나믹 캐스트를 한 뒤
	CGameOver* GameOver = dynamic_cast<CGameOver*>(
		CSceneManager::GetInst()->GetNextScene());
	if (!GameOver)
		return;

	//죽어서 게임오버됐다고 알려줌.
	GameOver->SetEssential(true);
}

int CPlayerSpace::InflictDamage(int Damage)
{
	int Dmg = CPlayer::InflictDamage(Damage);

	m_HP -= Dmg;

	return Dmg;
}

void CPlayerSpace::SetCutsceneMode(bool On)
{
	if (On)
	{
		//자신의 키바인딩을 모두 제거하고 좌측 구석으로 캐릭터를 이동시킨다.
		CInput::GetInst()->DeleteBindClass<CPlayerSpace>(this);
		MoveToDest(true, EMoveToMethod::MoveDir, Vector2(50.f, 200.f));
	}
	else
	{
		//다시 입력을 등록한다.
		RegisterInput();
	}
}




void CPlayerSpace::Start()
{
	CPlayer::Start();
}

void CPlayerSpace::Update(float DeltaTime)
{
	CPlayer::Update(DeltaTime);

	CGameManager::GetInst()->DebugTextOut("GunFire", (int)m_GunFired);

	if (CGameManager::GetInst()->GetDebugRender())
		m_Invincible = 1.f;

	if (m_Animation->isNowEndFrame())
		m_GunFired = false;

	bool KeepFire = false;
	if (m_AttackBuffer > 0)
	{
		FireGunCheck();
		if (m_CurrentWeapon == EWeapon::Laser)
			m_AttackBuffer = 0;
		KeepFire = true;
	}
		
	if (m_Bullet <= 0)
	{
		m_Bullet = 0;
		m_isLaserFire = nullptr;
		RifleDrop();
	}

	m_HMGDelay.UpdateCooltime(DeltaTime);
	if (m_HMGQueue > 0)
	{
		if (m_CurrentWeapon == EWeapon::HMG)
			FireHMG();
		else
			m_HMGQueue = 0;
	}

	if (KeepFire && m_CurrentWeapon == EWeapon::Laser)
	{
		if (m_isLaserFire)
		{
			m_isLaserFire->SetKeepFire(true);
			m_Bullet -= 2;
			m_WindowSpace->SetBulletLeft(m_Bullet);
		}
	}
	else
	{
		if (m_isLaserFire)
		{
			if (m_isLaserFire->GetKeepFire() < 0.05f)
			{
				m_isLaserFire = nullptr;
				m_GunFired = false;
			}
				
		}
	}


	if (m_HP <= 0)
		Death();
}

void CPlayerSpace::PostUpdate(float DeltaTime)
{
	CPlayer::PostUpdate(DeltaTime);

	//m_Pos = m_PrevPos;

	ComputeCurrentDir(DeltaTime);
	ComputeCurrentAct(DeltaTime);

	CGameManager::GetInst()->DebugTextOut("DirTime", m_DirTime);
	CGameManager::GetInst()->DebugTextOut("Current Dir", (int)m_CurrentDir);


	//같을 경우 프레임을 이어서 재생하고
	//다를 경우 새로 재생
	if (m_CurrentAct == m_PrevAct)
	{
		ChangeAnimContinue(
			m_vecAnimName[(int)m_CurrentDir][(int)m_CurrentAct]
		);
	}
	else
	{
		ChangeAnimation(
			m_vecAnimName[(int)m_CurrentDir][(int)m_CurrentAct]
		);
	}

	if (m_HP > 0)
	{
		if (m_CurrentDir != m_PrevDir)
		{
			ChangeAnimContinue(
				m_vecTailBoosterName[(int)m_CurrentDir]
			);
		}
	}
	else
	{
		StopAnimation(1);
	}



	//이전 액션 업데이트
	m_PrevDir = m_CurrentDir;
	m_PrevAct = m_CurrentAct;
}

void CPlayerSpace::Render(HDC hDC, float DeltaTime)
{
	CCharacter::Render(hDC, DeltaTime);

}



void CPlayerSpace::AttackEnd()
{	
	if (m_CurrentWeapon == EWeapon::Pistol)
	{
		m_CurrentAct = EDirAction::Idle_Pistol;
	}
	else
	{
		m_CurrentAct = EDirAction::Idle_Rifle;
	}

	m_GunFired = false;
}

void CPlayerSpace::HMGLaserAtkEnd()
{
	if (m_HMGQueue > 0 || m_isLaserFire)
		return;


	if (m_CurrentWeapon == EWeapon::Pistol)
	{
		m_CurrentAct = EDirAction::Attack_Rocket;
	}
	else
	{
		m_CurrentAct = EDirAction::Attack_Rocket;
	}

	m_GunFired = false;
}

void CPlayerSpace::FireHMG()
{
	if (m_Bullet <= 0)
		return;

	if (!m_HMGDelay.isCooltime)
	{


		//총알 오브젝트 생성
		CPlayerHMG* HMG = m_Scene->CreateObject<CPlayerHMG>("PlayerHMG");
		HMG->SetEssential(Vector2(0.05f * (float)m_HMGDir, -1.f), m_Pos);

		m_HMGDelay.EnterCooltime();

		++m_HMGDir;
		if (m_HMGDir >= 2)
			m_HMGDir = -1;

		--m_HMGQueue;
		--m_Bullet;
		m_WindowSpace->SetBulletLeft(m_Bullet);
	}
}

void CPlayerSpace::FireLaser()
{
	if (m_Bullet <= 0)
		return;

	if (!m_isLaserFire)
	{
		m_isLaserFire = m_Scene->CreateObject<CLaser>("Laser", this);
		m_isLaserFire->SetEssential(0.f, 1.f, m_Pos.x, m_Pos.y, m_Offset.x, m_Offset.y);

		m_Bullet -= 2;
		m_WindowSpace->SetBulletLeft(m_Bullet);
	}

	m_CurrentAct = EDirAction::Attack_Laser;
}

void CPlayerSpace::FireRocket()
{
	if (m_Bullet <= 0)
		return;

	m_CurrentAct = EDirAction::Attack_Rocket;

	CAstroSlugSFX* SFX = m_Scene->CreateObject<CAstroSlugSFX>("RocketLaunch");
	SFX->SetEssential(m_Pos + m_Offset);


	//총알 오브젝트 생성
	CRocketLauncher* Rocket = m_Scene->CreateObject<CRocketLauncher>("Rocket");
	Rocket->SetEssential(Vector2(0.f, -1.f), m_Pos + m_Offset);

	--m_Bullet;
	m_WindowSpace->SetBulletLeft(m_Bullet);
	++m_FiredRocket;	//로켓 발사 수 증가. 이 값은 CRocketLauncher의 소멸자에서 감소함
}

void CPlayerSpace::FireShotgun()
{
	if (m_Bullet <= 0)
		return;

	m_CurrentAct = EDirAction::Attack_Shotgun;

	//총알 오브젝트 생성
	CShotgun* Shotgun = m_Scene->CreateObject<CShotgun>("Shotgun");
	Shotgun->SetEssential(this,m_Offset);

	--m_Bullet;
	m_WindowSpace->SetBulletLeft(m_Bullet);
}




void CPlayerSpace::RegisterAnimName()
{
	m_vecAnimName
		[(int)EDirSpace::Left2][(int)EDirAction::Idle_Pistol]
		= "Left2_Idle_Pistol";
	m_vecAnimName
		[(int)EDirSpace::Left1][(int)EDirAction::Idle_Pistol]
		= "Left1_Idle_Pistol";
	m_vecAnimName
		[(int)EDirSpace::Mid][(int)EDirAction::Idle_Pistol]
		= "Mid_Idle_Pistol";
	m_vecAnimName
		[(int)EDirSpace::Right1][(int)EDirAction::Idle_Pistol]
		= "Right1_Idle_Pistol";
	m_vecAnimName
		[(int)EDirSpace::Right2][(int)EDirAction::Idle_Pistol]
		= "Right2_Idle_Pistol";


	m_vecAnimName
		[(int)EDirSpace::Left2][(int)EDirAction::Attack_Pistol]
		= "Left2_Attack_Pistol";
	m_vecAnimName
		[(int)EDirSpace::Left1][(int)EDirAction::Attack_Pistol]
		= "Left1_Attack_Pistol";
	m_vecAnimName
		[(int)EDirSpace::Mid][(int)EDirAction::Attack_Pistol]
		= "Mid_Attack_Pistol";
	m_vecAnimName
		[(int)EDirSpace::Right1][(int)EDirAction::Attack_Pistol]
		= "Right1_Attack_Pistol";
	m_vecAnimName
		[(int)EDirSpace::Right2][(int)EDirAction::Attack_Pistol]
		= "Right2_Attack_Pistol";


	m_vecAnimName
		[(int)EDirSpace::Left2][(int)EDirAction::Idle_Rifle]
		= "Left2_Idle_Rifle";
	m_vecAnimName
		[(int)EDirSpace::Left1][(int)EDirAction::Idle_Rifle]
		= "Left1_Idle_Rifle";
	m_vecAnimName
		[(int)EDirSpace::Mid][(int)EDirAction::Idle_Rifle]
		= "Mid_Idle_Rifle";
	m_vecAnimName
		[(int)EDirSpace::Right1][(int)EDirAction::Idle_Rifle]
		= "Right1_Idle_Rifle";
	m_vecAnimName
		[(int)EDirSpace::Right2][(int)EDirAction::Idle_Rifle]
		= "Right2_Idle_Rifle";


	m_vecAnimName
		[(int)EDirSpace::Left2][(int)EDirAction::Attack_HMG]
		= "Left2_Attack_HMG";
	m_vecAnimName
		[(int)EDirSpace::Left1][(int)EDirAction::Attack_HMG]
		= "Left1_Attack_HMG";
	m_vecAnimName
		[(int)EDirSpace::Mid][(int)EDirAction::Attack_HMG]
		= "Mid_Attack_HMG";
	m_vecAnimName
		[(int)EDirSpace::Right1][(int)EDirAction::Attack_HMG]
		= "Right1_Attack_HMG";
	m_vecAnimName
		[(int)EDirSpace::Right2][(int)EDirAction::Attack_HMG]
		= "RIght2_Attack_HMG";


	m_vecAnimName
		[(int)EDirSpace::Left2][(int)EDirAction::Attack_Shotgun]
		= "Left2_Attack_Shotgun";
	m_vecAnimName
		[(int)EDirSpace::Left1][(int)EDirAction::Attack_Shotgun]
		= "Left1_Attack_Shotgun";
	m_vecAnimName
		[(int)EDirSpace::Mid][(int)EDirAction::Attack_Shotgun]
		= "Mid_Attack_Shotgun";
	m_vecAnimName
		[(int)EDirSpace::Right1][(int)EDirAction::Attack_Shotgun]
		= "Right1_Attack_Shotgun";
	m_vecAnimName
		[(int)EDirSpace::Right2][(int)EDirAction::Attack_Shotgun]
		= "RIght2_Attack_Shotgun";


	m_vecAnimName
		[(int)EDirSpace::Left2][(int)EDirAction::Attack_Rocket]
		= "Left2_Attack_Rocket";
	m_vecAnimName
		[(int)EDirSpace::Left1][(int)EDirAction::Attack_Rocket]
		= "Left1_Attack_Rocket";
	m_vecAnimName
		[(int)EDirSpace::Mid][(int)EDirAction::Attack_Rocket]
		= "Mid_Attack_Rocket";
	m_vecAnimName
		[(int)EDirSpace::Right1][(int)EDirAction::Attack_Rocket]
		= "Right1_Attack_Rocket";
	m_vecAnimName
		[(int)EDirSpace::Right2][(int)EDirAction::Attack_Rocket]
		= "RIght2_Attack_Rocket";

	m_vecAnimName
		[(int)EDirSpace::Left2][(int)EDirAction::Attack_Laser]
		= "Left2_Attack_Laser";
	m_vecAnimName
		[(int)EDirSpace::Left1][(int)EDirAction::Attack_Laser]
		= "Left1_Attack_Laser";
	m_vecAnimName
		[(int)EDirSpace::Mid][(int)EDirAction::Attack_Laser]
		= "Mid_Attack_Laser";
	m_vecAnimName
		[(int)EDirSpace::Right1][(int)EDirAction::Attack_Laser]
		= "Right1_Attack_Laser";
	m_vecAnimName
		[(int)EDirSpace::Right2][(int)EDirAction::Attack_Laser]
		= "RIght2_Attack_Laser";


	m_vecAnimName
		[(int)EDirSpace::Left2][(int)EDirAction::Rifle_Drop]
		= "Left2_Rifle_Drop";
	m_vecAnimName
		[(int)EDirSpace::Left1][(int)EDirAction::Rifle_Drop]
		= "Left1_Rifle_Drop";
	m_vecAnimName
		[(int)EDirSpace::Mid][(int)EDirAction::Rifle_Drop]
		= "Mid_Rifle_Drop";
	m_vecAnimName
		[(int)EDirSpace::Right1][(int)EDirAction::Rifle_Drop]
		= "Right1_Rifle_Drop";
	m_vecAnimName
		[(int)EDirSpace::Right2][(int)EDirAction::Rifle_Drop]
		= "Right2_Rifle_Drop";

	for (int i = 0; i < (int)EDirSpace::MAX; ++i)
	{
		m_vecAnimName
			[i][(int)EDirAction::Slug_Ejection]
			= "Slug_Ejection";
	}

	for (int i = 0; i < (int)EDirSpace::MAX; ++i)
	{
		m_vecAnimName
			[i][(int)EDirAction::Death]
			= "Death";
	}


	//테일부스터
	m_vecTailBoosterName[(int)EDirSpace::Left2] = "TailBoostLeft2";
	m_vecTailBoosterName[(int)EDirSpace::Left1] = "TailBooxtLeft1";
	m_vecTailBoosterName[(int)EDirSpace::Mid] = "TailBoostMid";
	m_vecTailBoosterName[(int)EDirSpace::Right1] = "TailBoostRight1";
	m_vecTailBoosterName[(int)EDirSpace::Right2] = "TailBoostRight2";


	//측면부스터 오프셋
	m_vecSideBoosterOffset[(int)EDirSpace::Left2][DirLeft].Set(-10.f, -30.f);
	m_vecSideBoosterOffset[(int)EDirSpace::Left2][DirRight].Set(1.f, -30.f);
	
	m_vecSideBoosterOffset[(int)EDirSpace::Left1][DirLeft].Set(-10.f, -31.f);
	m_vecSideBoosterOffset[(int)EDirSpace::Left1][DirRight].Set(4.f, -31.f);

	m_vecSideBoosterOffset[(int)EDirSpace::Mid][DirLeft].Set(-9.f, -31.f);
	m_vecSideBoosterOffset[(int)EDirSpace::Mid][DirRight].Set(9.f, -31.f);

	m_vecSideBoosterOffset[(int)EDirSpace::Right1][DirLeft].Set(-5.f, -30.f);
	m_vecSideBoosterOffset[(int)EDirSpace::Right1][DirRight].Set(9.f, -30.f);

	m_vecSideBoosterOffset[(int)EDirSpace::Right2][DirLeft].Set(-1.f, -29.f);
	m_vecSideBoosterOffset[(int)EDirSpace::Right2][DirRight].Set(10.f, -30.f);
}

void CPlayerSpace::RegisterInput()
{
	//이펙트 생성
	SetInvincible(3.f);
	CJustPlay* JustPlay = m_Scene->CreateObject<CJustPlay>("P1Indicator", this);
	JustPlay->SetDuration(3.f);
	JustPlay->SetOffset(0.f, -20.f);

	//사이드 충돌 활성화
	m_SideCollOn = true;

	//입력 초기화
	CInput::GetInst()->AddBindFunction<CPlayerSpace>("MoveLeft",
		EInput_Type::Push,
		this, &CPlayerSpace::MoveLeft);

	CInput::GetInst()->AddBindFunction<CPlayerSpace>("MoveLeft",
		EInput_Type::Down,
		this, &CPlayerSpace::LeftKeyDown);

	CInput::GetInst()->AddBindFunction<CPlayerSpace>("MoveRight",
		EInput_Type::Push,
		this, &CPlayerSpace::MoveRight);

	CInput::GetInst()->AddBindFunction<CPlayerSpace>("MoveRight",
		EInput_Type::Down,
		this, &CPlayerSpace::RightKeyDown);

	CInput::GetInst()->AddBindFunction<CPlayerSpace>("MoveUp",
		EInput_Type::Push,
		this, &CPlayerSpace::MoveUp);

	CInput::GetInst()->AddBindFunction<CPlayerSpace>("MoveDown",
		EInput_Type::Push,
		this, &CPlayerSpace::MoveDown);

	//공격키 이때 등록해줌
	CInput::GetInst()->AddBindFunction<CPlayerSpace>("GunFire",
		EInput_Type::Down,
		this, &CPlayerSpace::FireGun);

	CInput::GetInst()->AddBindFunction<CPlayerSpace>("Bomb",
		EInput_Type::Push,
		this, &CPlayerSpace::FireBomb);

	CInput::GetInst()->AddBindFunction<CPlayerSpace>("Debug3",
		EInput_Type::Down,
		this, &CPlayerSpace::ChangeRifleHMG);

	CInput::GetInst()->AddBindFunction<CPlayerSpace>("Debug4",
		EInput_Type::Down,
		this, &CPlayerSpace::ChangeRifleShotgun);

	CInput::GetInst()->AddBindFunction<CPlayerSpace>("Debug5",
		EInput_Type::Down,
		this, &CPlayerSpace::ChangeRifleRocket);

	CInput::GetInst()->AddBindFunction<CPlayerSpace>("Debug6",
		EInput_Type::Down,
		this, &CPlayerSpace::ChangeRifleLaser);

	CInput::GetInst()->AddBindFunction<CPlayerSpace>("Debug7",
		EInput_Type::Down,
		this, &CPlayerSpace::Death);


}
