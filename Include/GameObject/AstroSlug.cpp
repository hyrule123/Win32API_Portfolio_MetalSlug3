#include "AstroSlug.h"

//플레이어 주소 지정
#include "../GameManager.h"

//플레이어 생성
#include "PlayerSpace.h"


//SFX 호출용
#include "../Scene/Scene.h"
#include "../Scene/MainScene.h"
#include "../Scene/SceneResource.h"
#include "AstroSlugSFX.h"
#include "Explosion.h"
#include "Trail.h"
#include "JustPlay.h"

#include "../Input.h"

#include "../Collision/ColliderBox.h"

//무기
#include "Pistol.h"
#include "AstroHMG.h"
#include "RocketLauncher.h"
#include "Shotgun.h"
#include "AstroBomb.h"
#include "Laser.h"

//스플래시 데미지
#include "Splash.h"

//위젯 접근
#include "../UI/WindowSpace.h"
#include "../UI/WidgetScore.h"

CAstroSlug::CAstroSlug() :
	m_isSeparate(true),
	m_PartState{ EPartState::Normal },
	m_SyncTimer(),
	m_SyncTimerMax(),
	m_CurrentWeapon(EWeapon::Pistol),
	m_Busy(false),
	m_FireBuffer(),
	m_ExplodeRemainTime(3.f),
	m_FireTurn(),
	m_PistolFireAngle(1),
	m_FireHMG(),
	m_SelfDestruct(),
	m_KnockBack(),
	m_BombTurn(),
	m_BoostOn(),
	m_BoostSFXSide(),
	m_BoostSFXTail(),
	m_BoosterDirSideName{},
	m_BoostDuration(0.5f),
	m_isKeyDown(),
	m_isLaserFire{},
	m_CutSceneTimer(1.f),
	m_isAttack(),
	m_isAfterCutscene(),
	m_SideGunOffset(24.f, 55.f),
	m_CutSceneMode(),
	m_TailBoosterOn()
{
	SetTypeID<CAstroSlug>();
	m_RenderLayer = ERenderLayer::Slug;
	m_HP = 3;
}

CAstroSlug::CAstroSlug(const CAstroSlug& Obj):
	CPlayer(Obj),
	m_isSeparate(Obj.m_isSeparate),
	m_SyncTimer(),
	m_SyncTimerMax(),
	m_CurrentWeapon(EWeapon::Pistol),
	m_Busy(false),
	m_FireBuffer(),
	m_ExplodeRemainTime(Obj.m_ExplodeRemainTime),
	m_FireTurn(Obj.m_FireTurn),
	m_PistolFireAngle(1),
	m_FireHMG(),
	m_SelfDestruct(),
	m_KnockBack(Obj.m_KnockBack),
	m_BombTurn(Obj.m_BombTurn),
	m_BoostOn(Obj.m_BoostOn),
	m_BoostSFXSide(),
	m_BoostSFXTail(),
	m_BoostDuration(Obj.m_BoostDuration),
	m_isKeyDown(Obj.m_isKeyDown),
	m_isLaserFire{},
	m_CutSceneTimer(Obj.m_CutSceneTimer),
	m_isAttack(Obj.m_isAttack),
	m_isAfterCutscene(Obj.m_isAfterCutscene),
	m_SideGunOffset(Obj.m_SideGunOffset),
	m_CutSceneMode(Obj.m_CutSceneMode),
	m_TailBoosterOn(Obj.m_TailBoosterOn)
{
	m_PartState[0] = Obj.m_PartState[0];
	m_PartState[1] = Obj.m_PartState[1];




	for (int i = 0; i < EBoosterDirSideMax; ++i)
	{
		m_BoosterDirSideName[i] = Obj.m_BoosterDirSideName[i];
	}

	for (int i = 0; i < EBoosterDirTailMax; ++i)
	{
		m_BoosterDirTailName[i] = Obj.m_BoosterDirTailName[i];
	}
}

CAstroSlug::~CAstroSlug()
{
	CInput::GetInst()->DeleteBindClass<CAstroSlug>(this);
}

bool CAstroSlug::LoadResource()
{
	CreateAnimation();
	if(!LoadCSVPreset(TEXT("Player/AstroSlug.csv")))
		return false;

	RegisterAnimName();

	return true;
}

void CAstroSlug::Start()
{
	m_Start = true;

	CPlayer::Start();


}

bool CAstroSlug::Init(CGameObject* Obj)
{
	CPlayer::Init(Obj);

	//Player의 주소를 CGameManager에 등록한다.
	CGameManager::GetInst()->SetPlayer(this);

	//인풋은 따로 등록해줘야 한다. 원하는타이밍에 할것.

	ChangeAnimation("PlayerRight");

	m_Animation->SetEndFunction<CAstroSlug>
		("LeftPart_Rifle_Get", this, &CAstroSlug::ChangeRifleEnd);
	m_Animation->SetEndFunction<CAstroSlug>
		("RightPart_Rifle_Get", this, &CAstroSlug::ChangeRifleEnd);
	m_Animation->SetEndFunction<CAstroSlug>
		("PlayerTurnLeftToRight", this, &CAstroSlug::PlayerTurnLeftToRightEnd);

	m_Animation->SetEndFunction<CAstroSlug>
		("EjectModeDoor", this, &CAstroSlug::EjectModeDoorEnd);


	//노티파이 등록
	//문 2프레임때 SFX 효과 재생
	m_Animation->AddNotify<CAstroSlug>("EjectModeDoor", 2, this, &CAstroSlug::EjectDoorFrame2Notify);




	//위치 설정
	SetPos(50.f, 40.f);
	SetSize(36.f, 50.f);
	SetDir(1.f, 0.f);
	SetPivot(0.5f, 1.f);
	SetMaxSpeed(150.f);
	//감속도
	SetDeAccel(0.85f);
	


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
			m_vecCooltime[(int)EWeapon::Shotgun].Cooltime = 0.3f;
			break;
		case EWeapon::Rocket:
			m_vecCooltime[(int)EWeapon::Rocket].Cooltime = 0.2f;
			break;
		case EWeapon::Laser:
			m_vecCooltime[(int)EWeapon::Laser].Cooltime = 0.2f;
			break;
		case EWeapon::Bomb:
			m_vecCooltime[(int)EWeapon::Bomb].Cooltime = 0.3f;
			break;
		default:
			break;
		}
	}
	

	//충돌체
	CColliderBox* Coll = AddCollider<CColliderBox>("AstroColl");
	Coll->SetSize(m_Size);
	Coll->SetPivot(0.5f, 1.f);
	Coll->SetCollisionProfile(ECollisionChannel::Player);


	m_BoostSFXSide = m_Scene->CreateObject<CAstroSlugSFX>("BoostSFXSide", this);
	m_BoostSFXSide->SetEffect(EEffectType::Loop);

	m_BoostSFXTail = m_Scene->CreateObject<CAstroSlugSFX>("BoostSFXTail", this);
	m_BoostSFXTail->SetEffect(EEffectType::Loop);

	//각 부스터별 이름을 문자열 배열에 등록
	m_BoosterDirSideName[SmallBoosterNE] = "SmallBoosterNE";
	m_BoosterDirSideName[SmallBoosterE] = "SmallBoosterE";
	m_BoosterDirSideName[SmallBoosterSE] = "SmallBoosterSE";
	m_BoosterDirSideName[SmallBoosterSW] = "SmallBoosterSW";
	m_BoosterDirSideName[SmallBoosterW] = "SmallBoosterW";
	m_BoosterDirSideName[SmallBoosterNW] = "SmallBoosterNW";
	
	m_BoosterDirTailName[TailBoost1] = "TailBoost1";
	m_BoosterDirTailName[TailBoost2] = "TailBoost2";
	m_BoosterDirTailName[TailSFX] = "TailSFX";
	m_BoosterDirTailName[TailLeft] = "TailLeft";
	m_BoosterDirTailName[TailRight] = "TailRight";
	m_BoosterDirTailName[TailStraight] = "TailStraight";
	m_BoosterDirTailName[TailIdle] = "TailIdle";



	return true;
}

void CAstroSlug::Update(float DeltaTime)
{
	if (!m_Start)
		Start();

	CPlayer::Update(DeltaTime);




	if (CGameManager::GetInst()->GetDebugRender())
		m_Invincible = 1.f;	

	

	if (m_IntroCutSceneFlags)
		m_isAfterCutscene = true;

	//이벤트 플래그 처리
	if (m_IntroCutSceneFlags & (UINT8)EIntroCutSceneFlags::IntroCutSceneOn)
	{
		EventCutsceneProcess();
	}


	if (m_MasterObject)
		m_Pos = m_MasterObject->GetPos() + m_CutSceneOffset;


	CGameManager::GetInst()->DebugTextOut("PlayerPos", m_Pos);
		


	m_isAttack = false;
	if (m_FireBuffer >= 1)
		FireGun();
	
	if (m_Bullet <= 0)
	{
		if (m_PartState[0] == EPartState::NormalRifle || m_PartState[1] == EPartState::NormalRifle)
		{
			m_Bullet = 0;
			RifleDrop();
			m_WindowSpace->SetBulletLeft(m_Bullet);

			m_isLaserFire[0] = nullptr;
			m_isLaserFire[1] = nullptr;
		}
	}

	if(m_isLaserFire[0] == nullptr && m_isLaserFire[1] == nullptr)
		SoundStop("LaserFire");

	//CGameManager::GetInst()->DebugTextOut("isCoolTime", (int)m_vecCooltime[(int)EWeapon::Laser].isCooltime);
	CGameManager::GetInst()->DebugTextOut("m_CurrentWeapon", (int)m_CurrentWeapon);


	if (m_isAttack && m_CurrentWeapon == EWeapon::Laser)
	{	
		if (m_isLaserFire[0] && m_PartState[0] == EPartState::NormalRifle)
		{
			m_isLaserFire[0]->SetKeepFire(true);
		}
		if (m_isLaserFire[1] && m_PartState[1] == EPartState::NormalRifle)
		{
			m_isLaserFire[1]->SetKeepFire(true);
		}
	}
	else
	{
		if (m_isLaserFire[0])
		{
			if (m_isLaserFire[0]->GetKeepFire() < 0.05f)
				m_isLaserFire[0] = nullptr;
		}
		if (m_isLaserFire[1])
		{
			if (m_isLaserFire[1]->GetKeepFire() < 0.05f)
				m_isLaserFire[1] = nullptr;
		}
	}


	if (m_FireHMG > 0)
	{
		if (m_FireHMG % 5 == 0)
			FireHMG();

		--m_FireHMG;
	}

	if (m_isSeparate)
		CheckParts();
	else
		m_Animation->StopAnimation((int)ELayerInfo::Right);

	if (m_HP <= 0)
		Destroy();

	if (m_SelfDestruct)
	{
		SetDir(0.f, -1.f);
		MoveToDest(true, EMoveToMethod::MoveDir, Vector2(m_Pos.x, m_Pos.y - 500.f));

	}

	if (m_KnockBack > 0.f)
		m_KnockBack -= DeltaTime;
		

	//부스터: 전부 켰다 껐다 해줌
	if (m_BoostOn)
	{
		for (int i = 0; i < EBoosterDirSideMax; ++i)
		{
			m_BoostSFXSide->ChangeAnimationDuration(m_BoosterDirSideName[i], m_BoostDuration);
		}
	}


	if (!m_isKeyDown && 
		!m_IntroCutSceneFlags && 
		m_isAfterCutscene && 
		!m_CutSceneMode)
	{
		m_BoostSFXTail->ChangeAnimation(m_BoosterDirTailName[TailIdle]);
	}
	
	if (m_TailBoosterOn)
		m_BoostSFXTail->ChangeAnimation(m_BoosterDirTailName[TailBoost1]);



	CGameManager::GetInst()->DebugTextOut("m_isKeyDown", (int)m_isKeyDown);
	m_isKeyDown = false;

}

void CAstroSlug::PostUpdate(float DeltaTime)
{
	if (!m_Start)
		Start();

	CPlayer::PostUpdate(DeltaTime);

	
}

void CAstroSlug::Render(HDC hDC, float DeltaTime)
{
	if (!m_Start)
		Start();

	CPlayer::Render(hDC, DeltaTime);
}


void CAstroSlug::SetCutsceneMode(bool On)
{
	m_CutSceneMode = On;

	if (m_CutSceneMode)
	{
		//자신의 키바인딩을 모두 제거하고 좌측 구석으로 캐릭터를 이동시킨다.
		CInput::GetInst()->DeleteBindClass<CAstroSlug>(this);
		MoveToDest(true, EMoveToMethod::MoveDir, Vector2(50.f, 200.f));
	}
	else
	{
		//다시 입력을 등록하고, Start 함수를 호출하여 이펙트를 생성한다.
		Start();
		RegisterInput();
	}
}

void CAstroSlug::SetIntroCutsceneMode()
{
	m_IntroCutSceneFlags |= 
		(UINT8)EIntroCutSceneFlags::IntroCutSceneOn;
	m_IntroCutSceneFlags |=
		(UINT8)EIntroCutSceneFlags::Action1;
}

void CAstroSlug::EventCutsceneProcess()
{
	if (m_IntroCutSceneFlags & (UINT8)EIntroCutSceneFlags::Action1)
	{
		//메인씬도 페이즈 시작
		CMainScene* Scene = dynamic_cast<CMainScene*>(m_Scene);
		if (Scene)
			Scene->GoNextPhase();


		m_MaxSpeed = 60.f;

		MoveToDest(true, EMoveToMethod::AddImpact, Vector2(90.f, 100.f));
		m_BoostSFXTail->
			ChangeAnimation(m_BoosterDirTailName[TailBoost1]);

		m_CutSceneTimer = 2.5f;

		m_IntroCutSceneFlags &= ~(UINT8)EIntroCutSceneFlags::Action1;
		m_IntroCutSceneFlags |= (UINT8)EIntroCutSceneFlags::Action2;
	}
	else if
		(
			m_IntroCutSceneFlags
			& (UINT8)EIntroCutSceneFlags::Action2
			)
	{
		m_CutSceneTimer -= DELTA_TIME;
		if (m_CutSceneTimer < 0.f)
		{
			m_IntroCutSceneFlags &= ~(UINT8)EIntroCutSceneFlags::Action2;
			m_IntroCutSceneFlags |= (UINT8)EIntroCutSceneFlags::Action3;
		}
	}
	else if (
		m_IntroCutSceneFlags
		& (UINT8)EIntroCutSceneFlags::Action3
		)
	{
		MoveToDest(true, EMoveToMethod::AddImpact, Vector2(100.f, 200.f));

		CMainScene* Scene = dynamic_cast<CMainScene*>(m_Scene);
		if (Scene)
			Scene->SetScrollMapSpeedSoft(300.f, 100.f);

		m_CutSceneTimer = 1.5f;

		m_IntroCutSceneFlags &= ~(UINT8)EIntroCutSceneFlags::Action3;
		m_IntroCutSceneFlags |= (UINT8)EIntroCutSceneFlags::Action4;
	}
	else if (
		m_IntroCutSceneFlags
		& (UINT8)EIntroCutSceneFlags::Action4
		)
	{
		m_CutSceneTimer -= DELTA_TIME;
		
		if (m_CutSceneTimer < 0.f)
		{
			m_IntroCutSceneFlags &= ~(UINT8)EIntroCutSceneFlags::Action4;
			m_IntroCutSceneFlags |= (UINT8)EIntroCutSceneFlags::Action5;
		}
	}
	else if (
		m_IntroCutSceneFlags
		& (UINT8)EIntroCutSceneFlags::Action5
		)
	{
		if (!m_MoveToOn)	//이동이 끝나면 게임 시작
		{
			SetInvincible(3.f);
			CJustPlay* JustPlay = m_Scene->CreateObject<CJustPlay>("P1Indicator", this);
			JustPlay->SetDuration(3.f);
			JustPlay->SetOffset(0.f, -20.f);

			RegisterInput();

			m_BoostSFXTail->TailBoost1ReverseSet();


			if (m_WindowSpace)
				m_WindowSpace->DisplayHUD();

			m_CutSceneTimer = 0.5f;

			m_IntroCutSceneFlags &= ~(UINT8)EIntroCutSceneFlags::Action5;
			m_IntroCutSceneFlags |= (UINT8)EIntroCutSceneFlags::Action6;
		}


		
	}
	else if (
		m_IntroCutSceneFlags
		& (UINT8)EIntroCutSceneFlags::Action6
		)
	{
		m_CutSceneTimer -= DELTA_TIME;
		if (m_CutSceneTimer < 0.f)
		{
			m_IntroCutSceneFlags = 0;
			m_MaxSpeed = 150.f;

			m_SideCollOn = true;

			//2페이즈 시작
			CMainScene* Scene = dynamic_cast<CMainScene*>(m_Scene);
			if (Scene)
				Scene->GoNextPhase();

		}
	}
}

void CAstroSlug::SetCutSceneOffset(const Vector2& Offset)
{
	m_CutSceneOffset = Offset;
}

void CAstroSlug::SetTailBooster(bool TailBoosterOn)
{
	m_TailBoosterOn = true;
}

void CAstroSlug::DebugDamage()
{
	InflictDamage(1);
}

int CAstroSlug::InflictDamage(int Damage)
{
	int DMG = CPlayer::InflictDamage(Damage);

	m_HP -= DMG;

	if (DMG >= 1)
	{
		//히트포인트에 따라서 약간의 넉백 및 총 하나 드롭
		KnockBack();
		RifleDrop();
		SetInvincible();
		SoundPlay("AstroHit");

		m_WindowSpace->SetHPBar(m_HP);
	}

	
	return DMG;
}

void CAstroSlug::GetGas()
{


	if(m_HP >= 3)
	{
		SoundPlayOneChan("MiscItemGet");

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
	else
	{
		SoundPlayOneChan("OK");
		m_HP += 1;
		m_WindowSpace->SetHPBar(m_HP);
	}


}





void CAstroSlug::CheckParts()
{
	switch (m_PartState[0])
	{
	case EPartState::Normal:
		ChangeAnimContinue("LeftPart_Pistol");
		break;
	case EPartState::NormalRifle:
		ChangeAnimContinue("LeftPart_Rifle");
		break;
	case EPartState::GunGet:
		ChangeAnimation("LeftPart_Rifle_Get");
		break;
	case EPartState::GunDrop:
		SetPlayReverse("LeftPart_Rifle_Get", true, true);
		ChangeAnimation("LeftPart_Rifle_Get");
		break;
	default:
		break;
	}

	switch (m_PartState[1])
	{
	case EPartState::Normal:
		ChangeAnimation("RightPart_Pistol");
		break;
	case EPartState::NormalRifle:
		ChangeAnimation("RightPart_Rifle");
		break;
	case EPartState::GunGet:
		ChangeAnimation("RightPart_Rifle_Get");
		break;
	case EPartState::GunDrop:
		SetPlayReverse("RightPart_Rifle_Get", true, true);
		ChangeAnimation("RightPart_Rifle_Get");
		break;
	default:
		break;
	}

	if (m_PartState[0] <= EPartState::NormalRifle
		&& m_PartState[1] <= EPartState::NormalRifle)
	{
		m_Animation->SyncPlayTime(
			(int)ELayerInfo::LeftorOne,
			(int)ELayerInfo::Right);
	}
}

void CAstroSlug::MoveLeft()
{
	if (m_KnockBack > 0.f)
		return;

	m_isKeyDown = true;

	SetDir(-1.f, 0.f);
	MoveDir(m_Dir);

	if (m_PartState[1] == EPartState::Normal)
	{
		m_BoostSFXSide->ChangeAnimationDuration(m_BoosterDirSideName[SmallBoosterE], m_BoostDuration);
		m_BoostSFXSide->SetAnimOffset(m_BoosterDirSideName[SmallBoosterE], 16.f, -22.f);
	}
	else if (m_PartState[1] == EPartState::NormalRifle)
	{
		m_BoostSFXSide->ChangeAnimationDuration(m_BoosterDirSideName[SmallBoosterE], m_BoostDuration);
		m_BoostSFXSide->SetAnimOffset(m_BoosterDirSideName[SmallBoosterE], 36.f, -22.f);
	}

}

void CAstroSlug::MoveRight()
{
	if (m_KnockBack > 0.f)
		return;

	m_isKeyDown = true;


	SetDir(1.f, 0.f);
	MoveDir(m_Dir);

	if (m_PartState[0] == EPartState::Normal)
	{
		m_BoostSFXSide->ChangeAnimationDuration(m_BoosterDirSideName[SmallBoosterW], m_BoostDuration);
		m_BoostSFXSide->SetAnimOffset(m_BoosterDirSideName[SmallBoosterW], -16.f, -22.f);
	}
	else if (m_PartState[0] == EPartState::NormalRifle)
	{
		m_BoostSFXSide->ChangeAnimationDuration(m_BoosterDirSideName[SmallBoosterW], m_BoostDuration);
		m_BoostSFXSide->SetAnimOffset(m_BoosterDirSideName[SmallBoosterW], -36.f, -22.f);
	}

}




void CAstroSlug::MoveUp()
{
	if (m_KnockBack > 0.f)
		return;

	m_isKeyDown = true;

	SetDir(0.f, -1.f);
	MoveDir(m_Dir);


	m_BoostSFXSide->ChangeAnimationDuration(m_BoosterDirSideName[SmallBoosterSW], m_BoostDuration);
	m_BoostSFXSide->ChangeAnimationDuration(m_BoosterDirSideName[SmallBoosterSE], m_BoostDuration);
}

void CAstroSlug::MoveDown()
{

	if (m_KnockBack > 0.f)
		return;


	m_isKeyDown = true;

	SetDir(0.f, 1.f);
	MoveDir(m_Dir);

	m_BoostSFXSide->ChangeAnimationDuration(m_BoosterDirSideName[SmallBoosterNW], m_BoostDuration);
	m_BoostSFXSide->ChangeAnimationDuration(m_BoosterDirSideName[SmallBoosterNE], m_BoostDuration);

	m_BoostSFXTail->ChangeAnimContinue(m_BoosterDirTailName[TailIdle]);
}

void CAstroSlug::KeyDownLeft()
{
	if (m_KnockBack > 0.f)
		return;
	
	m_isKeyDown = true;

	if (CheckAnimationPlaying("PlayerRight"))
	{
		//고개돌림
		SetPlayReverse("PlayerTurnLeftToRight", true, true);
		ChangeAnimation("PlayerTurnLeftToRight");
	}

	m_BoostSFXTail->ChangeAnimContinue(m_BoosterDirTailName[TailLeft]);
}

void CAstroSlug::KeyDownRight()
{
	if (m_KnockBack > 0.f)
		return;

	m_isKeyDown = true;

	if (CheckAnimationPlaying("PlayerLeft"))
	{
		ChangeAnimation("PlayerTurnLeftToRight");
	}

	m_BoostSFXTail->ChangeAnimContinue(m_BoosterDirTailName[TailRight]);
}

void CAstroSlug::KeyDownUp()
{
	if (m_KnockBack > 0.f)
		return;


	m_isKeyDown = true;


	m_BoostSFXTail->ChangeAnimContinue(m_BoosterDirTailName[TailStraight]);
}

void CAstroSlug::KeyDownDown()
{
	if (m_KnockBack > 0.f)
		return;

	m_isKeyDown = true;




	m_BoostSFXTail->ChangeAnimContinue(m_BoosterDirTailName[TailIdle]);
}

void CAstroSlug::FireGunBuffer()
{
	++m_FireBuffer;
}

void CAstroSlug::FireGun()
{
	m_isAttack = true;


	//아스트로 슬러그에서 피스톨은 어떤 무기를 쓰던 발사됨.
	if (!m_vecCooltime[(int)EWeapon::Pistol].isCooltime)
	{
		//쿨타임이 아니면 쿨타임으로 바꾸는 메소드를 호출하고 로직 작성
		EnterSkillCoolTIme((int)EWeapon::Pistol);

		SoundPlay("AstroPistol");

		CAstroSlugSFX* SFX = m_Scene->CreateObject<CAstroSlugSFX>("GunFireSFX", this);



		//총알 오브젝트 생성
		CAstroHMG* HMG = m_Scene->CreateObject<CAstroHMG>("HMG");
		HMG->SetEssential(Vector2((float)m_PistolFireAngle * 0.1f, -1.f), Vector2(m_Pos.x, m_Pos.y - 80.f));

		--m_PistolFireAngle;
		if (m_PistolFireAngle < -1)
			m_PistolFireAngle = 1;

		//레이저 확인
		if (m_CurrentWeapon == EWeapon::Laser)
		{
			if (m_isLaserFire[0])
				m_isLaserFire[0]->SetKeepFire(true);

		}

		//선입력 버퍼 초기화
		m_FireBuffer = 0;

		
	}

	//총구:  m_Pos로부터 좌측 -25, -65/우측 +25,-65
	switch (m_CurrentWeapon)
	{
	case EWeapon::HMG:
		if (!m_vecCooltime[(int)EWeapon::HMG].isCooltime)
		{
			//쿨타임이 아니면 쿨타임으로 바꾸는 메소드를 호출하고 로직 작성
			EnterSkillCoolTIme((int)EWeapon::HMG);

			
			//해당 로직은 해당 변수를 확인하여 Updat에서 작성함.
			m_FireHMG = 16;


			//선입력 버퍼 초기화
			m_FireBuffer = 0;
		}


		break;
	case EWeapon::Shotgun:
		if (!m_vecCooltime[(int)EWeapon::Shotgun].isCooltime)
		{
			//쿨타임이 아니면 쿨타임으로 바꾸는 메소드를 호출하고 로직 작성
			EnterSkillCoolTIme((int)EWeapon::Shotgun);

			if (m_PartState[m_FireTurn] == EPartState::NormalRifle)
			{
				FireShotgun(m_FireTurn);
			}
			else if (m_PartState[!m_FireTurn] == EPartState::NormalRifle)
			{
				FireShotgun(!m_FireTurn);
			}


			//선입력 버퍼 초기화
			m_FireBuffer = 0;
		}
		break;
	case EWeapon::Rocket:
		if (!m_vecCooltime[(int)EWeapon::Rocket].isCooltime)
		{
			if (m_FiredRocket >= 2)	//만약 맵에 2발 이상 로켓이 발사됐으면 return
				break;
			//쿨타임이 아니면 쿨타임으로 바꾸는 메소드를 호출하고 로직 작성
			EnterSkillCoolTIme((int)EWeapon::Rocket);

			if (m_PartState[m_FireTurn] == EPartState::NormalRifle)
			{
				FireRocket(m_FireTurn);
			}
			else if (m_PartState[!m_FireTurn] == EPartState::NormalRifle)
			{
				FireRocket(!m_FireTurn);
			}


			//선입력 버퍼 초기화
			m_FireBuffer = 0;
		}
		break;
	case EWeapon::Laser:
		if (m_vecCooltime[(int)EWeapon::Laser].EnterCooltime())
		{

			if (m_PartState[m_FireTurn] == EPartState::NormalRifle)
			{
				FireLaser(m_FireTurn);
			}
			if (m_PartState[!m_FireTurn] == EPartState::NormalRifle)
			{
				FireLaser(!m_FireTurn);
			}

			//선입력 버퍼 초기화
			m_FireBuffer = 0;
		}
		break;
	default:
		break;
	}

}


void CAstroSlug::FireHMG()
{


	//해당 메소드는 2프레임에 걸쳐 발사된다.(약간의 텀)

	if (m_PartState[m_FireTurn] == EPartState::NormalRifle && m_Bullet > 0)
	{
		SoundPlayOneChan("AstroHMG");

		CAstroSlugSFX* SFX = m_Scene->CreateObject<CAstroSlugSFX>("SideGunFireSFX");

		//좌측 총구 m_Pos로부터 -25, -65/+25,-65
		Vector2 Offset = m_Pos;
		if (!m_FireTurn) //좌측
			Offset.x -= m_SideGunOffset.x;
		else
			Offset.x += m_SideGunOffset.x;
		Offset.y -= m_SideGunOffset.y;

		SFX->SetPos(Offset);


		float RandDir = rand() % 2 == 0 ? -1.f : 1.f;	//랜덤한 방향 지정

		//총알 오브젝트 생성
		CAstroHMG* HMG = m_Scene->CreateObject<CAstroHMG>("HMG");
		//HMG->SetEssential(Vector2(0.f, -1.f), Offset);
		HMG->SetEssential(Vector2(RandDir * 0.03f, -1.f), Offset);
	
		--m_Bullet;
		m_WindowSpace->SetBulletLeft(m_Bullet);
	}



	//반대 방향으로 전환.
	m_FireTurn = !m_FireTurn;
}

void CAstroSlug::FireShotgun(bool isRight)
{
	if (m_Bullet <= 0)
		return;

	//좌측 총구 m_Pos로부터 -25, -65/+25,-65
	Vector2 Offset;
	if (!isRight) //좌측
		Offset.x -= m_SideGunOffset.x;
	else
		Offset.x += m_SideGunOffset.x;
	Offset.y -= m_SideGunOffset.y;



	//총알 오브젝트 생성
	CShotgun* Shotgun = m_Scene->CreateObject<CShotgun>("Shotgun");
	Shotgun->SetEssential(this, Offset);

	--m_Bullet;
	m_WindowSpace->SetBulletLeft(m_Bullet);
	m_FireTurn = !m_FireTurn;
}

void CAstroSlug::FireRocket(bool isRight)
{
	if (m_Bullet <= 0)
		return;

	CAstroSlugSFX* SFX = m_Scene->CreateObject<CAstroSlugSFX>("RocketLaunch");

	//좌측 총구 m_Pos로부터 -25, -65/+25,-65
	Vector2 Offset = m_Pos;
	if (!isRight) //좌측
		Offset.x -= m_SideGunOffset.x;
	else
		Offset.x += m_SideGunOffset.x;
	Offset.y -= m_SideGunOffset.y - 10.f;

	SFX->SetPos(Offset);

	

	//총알 오브젝트 생성
	CRocketLauncher* Rocket = m_Scene->CreateObject<CRocketLauncher>("Rocket");
	Rocket->SetEssential(Vector2(0.f, -1.f), Offset);

	

	--m_Bullet;
	m_WindowSpace->SetBulletLeft(m_Bullet);
	++m_FiredRocket;	//로켓 발사 수 증가. 이 값은 CRocketLauncher의 소멸자에서 감소함
	m_FireTurn = !m_FireTurn;
}

void CAstroSlug::FireLaser(bool isRight)
{
	if (m_Bullet <= 0)
		return;

	if (m_isLaserFire[isRight])
	{
		m_isLaserFire[isRight]->SetKeepFire(true);
		m_Bullet -= 4;
		m_WindowSpace->SetBulletLeft(m_Bullet);
	}
	else
	{
		m_isLaserFire[isRight] = m_Scene->CreateObject<CLaser>("Laser", this);

		Vector2 Offset;
		if (!isRight) //좌측
			Offset.x -= m_SideGunOffset.x;
		else
			Offset.x += m_SideGunOffset.x;
		Offset.y -= m_SideGunOffset.y;

		m_isLaserFire[isRight]->SetEssential(0.f, 1.f, m_Pos.x, m_Pos.y, Offset.x, Offset.y);

		m_Bullet -= 4;
		m_WindowSpace->SetBulletLeft(m_Bullet);
	}
}







void CAstroSlug::FireBomb()
{
	if (m_Bomb <= 0)
		return;

	if (!m_vecCooltime[(int)EWeapon::Bomb].isCooltime)
	{
		//쿨타임이 아니면 쿨타임으로 바꾸는 메소드를 호출하고 로직 작성
		EnterSkillCoolTIme((int)EWeapon::Bomb);


		CAstroBomb* Bomb = m_Scene->CreateObject<CAstroBomb>("BombRight");
		if (Bomb)
			Bomb->SetEssential(
				m_Pos.x + 10.f, m_Pos.y + 10.f, m_BombTurn);
		Bomb = nullptr;
		Bomb = m_Scene->CreateObject<CAstroBomb>("BombLeft");
		if (Bomb)
			Bomb->SetEssential(
				m_Pos.x - 10.f, m_Pos.y + 10.f, !m_BombTurn);

		--m_Bomb;
		m_WindowSpace->SetBombLeft(m_Bomb);
		m_BombTurn = !m_BombTurn;
	}
}

void CAstroSlug::Boost()
{
	m_MaxSpeed = 250.f;
	m_BoostOn = true;
}

void CAstroSlug::BoostEnd()
{
	m_MaxSpeed = 150.f;
	m_BoostOn = false;
}

void CAstroSlug::SelfDestruct()
{
	m_Scene->GetSceneResource()->SoundPlayOneChan("AstroSlugOut");

	m_WindowSpace->SetHPBar(0);

	m_SelfDestruct = true;
	m_SideCollOn = false;

	//우선 속도를 0으로(플레이어 탈출 이후 급가속)
	SetMaxSpeed(0.f);

	//충돌체 프로필을 PlayerAttack으로 교체
	auto iter = m_listCollider.begin();
	auto iterEnd = m_listCollider.end();
	while (iter != iterEnd)
	{
		(*iter)->SetCollisionProfile(ECollisionChannel::PlayerAttack);

		++iter;
	}

	Eject();
}

void CAstroSlug::FireNotify()
{
}

void CAstroSlug::RifleGet()
{
	m_Busy = true;

	//우측과 좌측 부스터 재생을 정지시킨다.
	m_BoostSFXSide->StopAnimation(1);
	m_BoostSFXSide->StopAnimation(4);

	for (int i = 0; i <= 1; ++i)
	{
		if (m_PartState[i] != EPartState::NormalRifle)
			m_PartState[i] = EPartState::GunGet;
	}

}


void CAstroSlug::RifleDrop()
{
	m_BoostSFXSide->StopAnimation(1);
	m_BoostSFXSide->StopAnimation(4);

	//오른쪽->왼쪽 순으로 총 떨어짐 -> 히트포인트로 수정할것
	if (m_PartState[Right] == EPartState::NormalRifle)
	{
		m_Busy = true;
		m_PartState[Right] = EPartState::GunDrop;
		m_isLaserFire[Right] = nullptr;
	}
	else if (m_PartState[Left] == EPartState::NormalRifle)
	{
		m_Busy = true;
		m_PartState[Left] = EPartState::GunDrop;
		m_isLaserFire[Left] = nullptr;
	}

	if (m_HP <= 0 || m_Bullet <= 0) //체력 다 되면 둘다 떨어짐
	{
		if (m_PartState[Right] == EPartState::NormalRifle)
		{
			m_Busy = true;
			m_PartState[Right] = EPartState::GunDrop;
			m_isLaserFire[Right] = nullptr;
		}
		if (m_PartState[Left] == EPartState::NormalRifle)
		{
			m_Busy = true;
			m_PartState[Left] = EPartState::GunDrop;
			m_isLaserFire[Left] = nullptr;
		}
	}
}


void CAstroSlug::ChangeRifleEnd()
{
	m_Busy = false;
	
	for (int i = 0; i <= 1; ++i)
	{
		if (m_PartState[i] == EPartState::GunGet)
			m_PartState[i] = EPartState::NormalRifle;
		else if (m_PartState[i] == EPartState::GunDrop)
			m_PartState[i] = EPartState::Normal;
	}
}

void CAstroSlug::PlayerTurnLeftToRightEnd()
{
	CAnimationInfo* Info = m_Animation->FindAnimInfo("PlayerTurnLeftToRight");

	if (!Info)
		return;

	if(Info->GetReverse())
		ChangeAnimation("PlayerLeft");
	else
		ChangeAnimation("PlayerRight");
		
}

void CAstroSlug::Destroy()
{
	
	if(m_ExplodeRemainTime > 0.f)
		m_ExplodeRemainTime -= DELTA_TIME;
	if (m_ExplodeRemainTime < 0.f)
	{
		//이떄까지도 플레이어가 바뀌지 않았으면 플레이어 생성하고 즉시 사망
		if (m_Scene->GetPlayer() == this)
		{
			CPlayerSpace* Player = m_Scene->CreateObject<CPlayerSpace>("PlayerSpace");

			Player->SetPos(m_Pos.x, m_Pos.y - 10.f);

			Player->InflictDamage(1);	
		}

		m_Scene->GetSceneResource()->SoundPlay("AstroExplosion");
		m_Scene->GetSceneResource()->SoundStop("AstroSlugOut");

		CExplosion* SFX = m_Scene->CreateObject<CExplosion>("BigExplosion");
		SFX->SetEssential(EExplosionSize::Big, m_Pos);

		CTrail* Trail = m_Scene->CreateObject<CTrail>("Trail");
		Trail->SetPos(m_Pos.x, m_Pos.y + 5.f);
		Trail->SetDir(1.f, 0.f);
		Trail->Trajectory(150.f);
		Trail->SetDuration(0.7f);

		Trail = m_Scene->CreateObject<CTrail>("Trail");
		Trail->SetPos(m_Pos.x, m_Pos.y);
		Trail->SetDir(-1.f, 0.f);
		Trail->Trajectory(150.f);
		Trail->SetDuration(0.7f);

		Trail = m_Scene->CreateObject<CTrail>("Trail");
		Trail->SetDuration(0.7f);
		Trail->SetPos(m_Pos);
		Trail->SetDir(1.f, 1.f);
		Trail->Straight(150.f);

		Trail = m_Scene->CreateObject<CTrail>("Trail");
		Trail->SetDuration(0.7f);
		Trail->SetPos(m_Pos);
		Trail->SetDir(-1.f, 1.f);
		Trail->Straight(150.f);

		//화면을 하얗게 1프레임간 전환
		CGameManager::GetInst()->SetWhiteOut(2);

		SetActive(false);
	}
		
	//HP를 많이 내려놔서 한번만 진입하도록 해준다.
	if (m_HP > -10)
	{
		m_HP = -100;


		m_Scene->GetSceneResource()->SoundPlay("AstroSlugOut");


		
		m_Busy = true;

		ClearCollider();

		//감속도를 크게 낮추고
		SetDeAccel(0.96f);

		//다른 모든 입력(행동)을 제한하고
		CInput::GetInst()->DeleteBindClass<CAstroSlug>(this);

		//비상탈출만 키만 등록한다.
		CInput::GetInst()->AddBindFunction<CAstroSlug>("Jump",
			EInput_Type::Down,
			this, &CAstroSlug::Eject);

		//빨갛게 바꿔주고
		StartReaction(5.f, true, EReactionChannel::HitReaction);

		//반짝이지 않게 무적으로 수동 설정한다.(충돌은 계속 있어야함)
		m_Invincible = 4.f;

		//OUT 메시지 출력.
		CAstroSlugSFX* SFX = m_Scene->CreateObject<CAstroSlugSFX>("SlugOut", this);
		//SFX->SetSFX("SlugOut");
		SFX->SetEffect(EEffectType::Duration);
		SFX->SetDuration(3.f);
		SFX->SetOffset(1.f, -70.f);
	}
}


void CAstroSlug::Eject()
{
	//자폭 입력 또는 체력0상태에서 입력이 들어왔을 경우
	if (m_ExplodeRemainTime < 2.5f || m_SelfDestruct)
	{
		//시간이 너무 적으면 탈출 애니메이션 재생할 시간만 추가해줌
		if (m_ExplodeRemainTime < 0.5f)
			m_ExplodeRemainTime += 0.5f;

		//탈출 안내문 off
		CGameObject* SFX = m_Scene->FindObject("SlugOut");
		if(SFX)
			SFX->SetActive(false);

		//탈출하는 순간 우주선 조작 중지
		CInput::GetInst()->DeleteBindClass<CAstroSlug>(this);

		//감속도 0
		SetDeAccel(0.f);

		//탈출 장면 애니메이션 재생
		m_isSeparate = false;
		ChangeAnimation("EjectMode");
		ChangeAnimation("EjectModeDoor", true);
	}
}

void CAstroSlug::EjectDoorFrame2Notify()
{
	m_Scene->SetCamTarget(nullptr);

	CAstroSlugSFX* SFX = m_Scene->CreateObject<CAstroSlugSFX>("BurningDoorSFX");
	SFX->SetEffect(EEffectType::Once);
	SFX->SetPos(m_Pos);

	SFX = m_Scene->CreateObject<CAstroSlugSFX>("DoorParticle");
	SFX->SetEffect(EEffectType::Once);
	SFX->SetPos(m_Pos);

}

void CAstroSlug::EjectModeDoorEnd()
{
	//플레이어 창의 재생을 false로 전환
	StopAnimation((int)ELayerInfo::PlayerWindow);

	//플레이어 생성은 문 파괴 애니메이션에서 한다.
	CAstroSlugSFX* SFX = m_Scene->CreateObject<CAstroSlugSFX>("DetachedDoor");
	m_Scene->GetSceneResource()->SoundPlay("AstroHatchOpen");
	//SFX->SetSFX("DetachedDoor");
	SFX->SetPos(m_Pos);
	SFX->SetDuration(1.5f);
	SFX->SetPhysicsSimulate(true);
	SFX->SetJumpVelocity(20.f);
	SFX->SetGravityAccel(5.f);
	SFX->Jump();

	CPlayerSpace* Player = m_Scene->CreateObject<CPlayerSpace>("PlayerSpace");

	Player->SetPos(m_Pos.x, m_Pos.y);
	Player->SlugEjection();


	//자폭 모드일경우 앞으로 돌진
	//여기 들어오면 플레이어 탈출이 완료된것이므로 속도제한을 풀어줌
	//이동은 업데이트()에서 처리
	if (m_SelfDestruct)
	{
		SetMaxSpeed(600.f);
		SetDeAccel(1.f);

		//충돌체를 찾아서 자폭 엔드펑션으로 교체
		//일단은 충돌체가 하나뿐이므로 begin을 통해서 교체해줘도 될듯.
		auto iter = m_listCollider.begin();
		if(iter != m_listCollider.end())
			(*iter)->SetCollisionBeginFunc<CAstroSlug>(this, &CAstroSlug::SelfDestructCollBegin);
	}
}



void CAstroSlug::AttackEnd()
{
}

void CAstroSlug::KnockBack()
{
	//히트포인트를 구해서 반대방향으로 넉백
	SetDir(m_Dir * -1);

	
	AddImpact(m_MaxSpeed);
	m_KnockBack = 0.5f;
}

void CAstroSlug::RegisterAnimName()
{
	CAnimationInfo* Info = m_Animation->FindAnimInfo("LeftPart_Pistol");
	if (!Info)
		return;
	m_SyncTimerMax = Info->GetPlayTime();
}

void CAstroSlug::RegisterInput()
{
	//조작 가능해지면 이펙트를 생성한다.
	SetInvincible(3.f);
	CJustPlay* JustPlay = m_Scene->CreateObject<CJustPlay>("P1Indicator", this);
	JustPlay->SetDuration(3.f);
	JustPlay->SetOffset(0.f, -20.f);


	//입력 초기화
	CInput::GetInst()->AddBindFunction<CAstroSlug>("MoveLeft",
		EInput_Type::Down,
		this, &CAstroSlug::KeyDownLeft);
	CInput::GetInst()->AddBindFunction<CAstroSlug>("MoveLeft",
		EInput_Type::Push,
		this, &CAstroSlug::MoveLeft);

	CInput::GetInst()->AddBindFunction<CAstroSlug>("MoveRight",
		EInput_Type::Down,
		this, &CAstroSlug::KeyDownRight);
	CInput::GetInst()->AddBindFunction<CAstroSlug>("MoveRight",
		EInput_Type::Push,
		this, &CAstroSlug::MoveRight);

	CInput::GetInst()->AddBindFunction<CAstroSlug>("MoveUp",
		EInput_Type::Down,
		this, &CAstroSlug::KeyDownUp);
	CInput::GetInst()->AddBindFunction<CAstroSlug>("MoveUp",
		EInput_Type::Push,
		this, &CAstroSlug::MoveUp);

	CInput::GetInst()->AddBindFunction<CAstroSlug>("MoveDown",
		EInput_Type::Down,
		this, &CAstroSlug::KeyDownDown);
	CInput::GetInst()->AddBindFunction<CAstroSlug>("MoveDown",
		EInput_Type::Push,
		this, &CAstroSlug::MoveDown);

	CInput::GetInst()->AddBindFunction<CAstroSlug>("GunFire",
		EInput_Type::Down,
		this, &CAstroSlug::FireGunBuffer);

	CInput::GetInst()->AddBindFunction<CAstroSlug>("Bomb",
		EInput_Type::Down,
		this, &CAstroSlug::FireBomb);

	CInput::GetInst()->AddBindFunction<CAstroSlug>("Jump",
		EInput_Type::Push,
		this, &CAstroSlug::Boost);

	CInput::GetInst()->AddBindFunction<CAstroSlug>("Jump",
		EInput_Type::Up,
		this, &CAstroSlug::BoostEnd);

	CInput::GetInst()->AddBindFunction<CAstroSlug>("SelfDestruct",
		EInput_Type::Down,
		this, &CAstroSlug::SelfDestruct);

	

	CInput::GetInst()->AddBindFunction<CAstroSlug>("Debug3",
		EInput_Type::Down,
		this, &CAstroSlug::ChangeHMG);

	CInput::GetInst()->AddBindFunction<CAstroSlug>("Debug4",
		EInput_Type::Down,
		this, &CAstroSlug::ChangeShotgun);

	CInput::GetInst()->AddBindFunction<CAstroSlug>("Debug5",
		EInput_Type::Down,
		this, &CAstroSlug::ChangeRocketLauncher);


	CInput::GetInst()->AddBindFunction<CAstroSlug>("Debug6",
		EInput_Type::Down,
		this, &CAstroSlug::ChangeLaser);

	CInput::GetInst()->AddBindFunction<CAstroSlug>("Debug7",
		EInput_Type::Down,
		this, &CAstroSlug::DebugDamage);
}


void CAstroSlug::ChangeWeapon(EWeapon Weapon)
{
	//총 획득 애니메이션 재생
	if(Weapon != EWeapon::Bomb)
		RifleGet();

	//무기 변경
	switch (Weapon)
	{
	case EWeapon::HMG:
		if (m_CurrentWeapon == EWeapon::HMG)
		{
			
			m_Bullet += 200;
			m_WindowSpace->SetBulletLeft(m_Bullet);
		}
			
		else
		{
			m_CurrentWeapon = EWeapon::HMG;
			m_Bullet = 200;
			m_WindowSpace->SetBulletLeft(m_Bullet);
		}
		SoundPlayOneChan("GetHMG");
		break;
	case EWeapon::Shotgun:
		if (m_CurrentWeapon == EWeapon::Shotgun)
		{
			m_Bullet += 30;
		}
		else
		{
			m_CurrentWeapon = EWeapon::Shotgun;
			m_Bullet = 30;
			
		}
		SoundPlayOneChan("GetShotgun");
		break;
	case EWeapon::Rocket:
		if (m_CurrentWeapon == EWeapon::Rocket)
		{
			m_Bullet += 30;
		}
			
		else
		{
			m_CurrentWeapon = EWeapon::Rocket;
			m_Bullet = 30;
		}
		SoundPlayOneChan("GetRocket");

		break;
	case EWeapon::Laser:
		if (m_CurrentWeapon == EWeapon::Laser)
		{
			m_Bullet += 200;
		}
		else
		{
			m_CurrentWeapon = EWeapon::Laser;
			m_Bullet = 200;
		}
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

void CAstroSlug::ChangeHMG()
{
	ChangeWeapon(EWeapon::HMG);
}

void CAstroSlug::ChangeShotgun()
{
	ChangeWeapon(EWeapon::Shotgun);
}

void CAstroSlug::ChangeRocketLauncher()
{
	ChangeWeapon(EWeapon::Rocket);
}

void CAstroSlug::ChangeLaser()
{
	ChangeWeapon(EWeapon::Laser);
}

void CAstroSlug::SelfDestructCollBegin(CCollider* Src, CCollider* Dest)
{
	SoundStop("AstroSlugOut");


	m_ExplodeRemainTime = -1.f;

	CSplash* Splash = m_Scene->CreateObject<CSplash>("Splash");
	Splash->SetEssential(m_Pos, Vector2(0.f, -100.f), 50.f, ECollisionChannel::PlayerAttack, 400);

	Destroy();
}
