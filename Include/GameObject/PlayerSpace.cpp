#include "PlayerSpace.h"
#include "../GameManager.h"
#include "../input.h"
#include "../Scene/Scene.h"

#include "../Scene/SceneResource.h"
#include "../Scene/SceneManager.h"

//�¿� �ִϸ��̼� ��� ���� �Է� Ȯ�ο�
#include "../Input.h"

//�浹ü
#include "../Collision/ColliderBox.h"

#include "Pistol.h"

//����
#include "PlayerHMG.h"
#include "Laser.h"
#include "Shotgun.h"
#include "RocketLauncher.h"
#include "AstroSlugSFX.h"	//���� �߻� ����Ʈ�� ������
#include "AstroBomb.h"

#include "../UI/WindowSpace.h"

//�ִϸ��̼� ���
#include "JustPlay.h"

//���̵��ξƿ�
#include "../UI/WidgetFadeInOut.h"

//���� ���� ���
#include "../UI/WidgetScore.h"

//���ӿ���
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
	m_HMGDelay(0.06f),	//���ӽŰ��� �߻� ������
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
	/////////////////// �ִϸ��̼� ���� ///////////////////
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

	


	//��ġ ����
	SetPos(50.f, 50.f);
	SetSize(22.f, 50.f);
	SetDir(1.f, 0.f);
	SetPivot(0.5f, 1.f);
	SetScale(1.f);
	SetMaxSpeed(150.f);
	SetDeAccel(1.f);


	//��ų ����
	//��ų ����
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


	//���� ���� EndFunction ���
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

	//HMG ��Ƽ���� ���
	for (int i = (int)EDirSpace::Left2; i < (int)EDirSpace::MAX; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			AddNotify<CPlayerSpace>(m_vecAnimName[i][(int)EDirAction::Attack_HMG], j, this, &CPlayerSpace::FireHMG);
		}

	}
	
	////�浹ü ����
	CColliderBox* Collider = AddCollider<CColliderBox>("PlayerBox");

	Collider->SetSize(m_Size);
	Collider->SetCollisionProfile(ECollisionChannel::Player);
	Collider->SetPivot(0.5f, 1.f);





	//���� �Ѿ� �� ����
	m_WindowSpace->SetBulletLeft(m_Bullet);
	m_WindowSpace->SetBombLeft(m_Bomb);



	return true;
}



void CPlayerSpace::ComputeCurrentDir(float DeltaTime)
{
	//�������� ���� ��� �������� ���ƿ�
	if (fabs(m_MoveDist.x) <= FLT_EPSILON)
	{
		if (m_DirTime > m_Tolerance)
			m_DirTime -= DeltaTime;
		else if (m_DirTime < -m_Tolerance)
			m_DirTime += DeltaTime;
	};


	//������� ������ �� �������� ���� ����� ��������. ���� Ȯ��
	//10�� ���ѵ� ������ �ٲپ �Ҽ����� �� �ٲ��ص�
	//����ü�� �°� ������
	//ex)-0.25f(�� ����) -> -2.5f -> -2 + 2(�ִ񰪸�ŭ �����ذ�)
	//->�ε����� �´� ���� ����
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
	//�� �Ƚ����� ���̵� ���(�� ������ �� �߻� �޼ҵ忡�� �׼� �����)
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

	if (m_DirTime <= -1.f * m_DirTimeMax)//�ִ��� ����(+������)
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


	if (m_DirTime >= m_DirTimeMax)//�ִ��� ����
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
			if (m_FiredRocket >= 2)	//���� �ʿ� 2�� �̻� ������ �߻������ return
				break;
			//��Ÿ���� �ƴϸ� ��Ÿ������ �ٲٴ� �޼ҵ带 ȣ���ϰ� ���� �ۼ�
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

			//���Է� ���� �ʱ�ȭ
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

	//��Ÿ���� �ƴϸ� ��Ÿ������ �ٲٴ� �޼ҵ带 ȣ���ϰ� ���� �ۼ�
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
	////��ų ���� ����
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

	//�� �߻� ���¸� true�� �����Ͽ� �ѹ߻� �Ұ����ϰ� �ϰ� ���� ����
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
	//�ѹ��� ����
	if (m_HP <= -10)
		return;
	m_HP = -10;
	
	m_SideCollOn = false;

	//��� Ű���ε��� ����
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
	//�ٷ� �����ϱ� �׷��������� ����..
	//���� Ÿ�̸Ӱ����� �ϳ� ���� �� �� �ð��� �����ؾ� �ҵ�
	//�ø����� �ذ�
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
	//���ӿ��� ������ ��ȯ�ϰ�
	CSceneManager::GetInst()->ChangeGameOver();

	//���̳��� ĳ��Ʈ�� �� ��
	CGameOver* GameOver = dynamic_cast<CGameOver*>(
		CSceneManager::GetInst()->GetNextScene());
	if (!GameOver)
		return;

	//�׾ ���ӿ����ƴٰ� �˷���.
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
		//�ڽ��� Ű���ε��� ��� �����ϰ� ���� �������� ĳ���͸� �̵���Ų��.
		CInput::GetInst()->DeleteBindClass<CPlayerSpace>(this);
		MoveToDest(true, EMoveToMethod::MoveDir, Vector2(50.f, 200.f));
	}
	else
	{
		//�ٽ� �Է��� ����Ѵ�.
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


	//���� ��� �������� �̾ ����ϰ�
	//�ٸ� ��� ���� ���
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



	//���� �׼� ������Ʈ
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


		//�Ѿ� ������Ʈ ����
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


	//�Ѿ� ������Ʈ ����
	CRocketLauncher* Rocket = m_Scene->CreateObject<CRocketLauncher>("Rocket");
	Rocket->SetEssential(Vector2(0.f, -1.f), m_Pos + m_Offset);

	--m_Bullet;
	m_WindowSpace->SetBulletLeft(m_Bullet);
	++m_FiredRocket;	//���� �߻� �� ����. �� ���� CRocketLauncher�� �Ҹ��ڿ��� ������
}

void CPlayerSpace::FireShotgun()
{
	if (m_Bullet <= 0)
		return;

	m_CurrentAct = EDirAction::Attack_Shotgun;

	//�Ѿ� ������Ʈ ����
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


	//���Ϻν���
	m_vecTailBoosterName[(int)EDirSpace::Left2] = "TailBoostLeft2";
	m_vecTailBoosterName[(int)EDirSpace::Left1] = "TailBooxtLeft1";
	m_vecTailBoosterName[(int)EDirSpace::Mid] = "TailBoostMid";
	m_vecTailBoosterName[(int)EDirSpace::Right1] = "TailBoostRight1";
	m_vecTailBoosterName[(int)EDirSpace::Right2] = "TailBoostRight2";


	//����ν��� ������
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
	//����Ʈ ����
	SetInvincible(3.f);
	CJustPlay* JustPlay = m_Scene->CreateObject<CJustPlay>("P1Indicator", this);
	JustPlay->SetDuration(3.f);
	JustPlay->SetOffset(0.f, -20.f);

	//���̵� �浹 Ȱ��ȭ
	m_SideCollOn = true;

	//�Է� �ʱ�ȭ
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

	//����Ű �̶� �������
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
