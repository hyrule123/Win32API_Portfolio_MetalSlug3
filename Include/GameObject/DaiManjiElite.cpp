#include "DaiManjiElite.h"

#include "DaiManji.h"

#include "DaiManji.h"

#include "../Scene/MainScene.h"
#include "../Scene/SceneManager.h"
#include "../GameManager.h"
#include "Player.h"

//발사체(DaimanjiAttack)
#include "ProjectileEnemy.h"

//리소스 로드
#include "../Scene/SceneResource.h"
#include "../Resource/Texture/Texture.h"


//이펙트
#include "Effect.h"
#include "Explosion.h"

//충돌체
#include "../Collision/ColliderBox.h"
#include "../Collision/ColliderCircle.h"

//몬스터 스폰
#include "MiniUFO.h"
#include "MiniUFOElite.h"

//라그하임 -> 이동명령 동시에 내리기위함
#include "Rugname.h"

//위치 계산 카메라
#include "../Scene/Camera.h"

#include "../UI/WidgetComponent.h"

#include "Rugname.h"

CDaiManjiElite::CDaiManjiElite():
	m_ForceMove(),
	m_Phase2(),
	m_Timer(),
	m_Rugname(),
	m_Action(),
	m_SpawnQueue(),
	m_InAction(),
	m_LastCutScene()
{
	SetTypeID<CDaiManjiElite>();
}

CDaiManjiElite::CDaiManjiElite(const CDaiManjiElite& Obj) :
	CDaiManji(Obj),
	m_ForceMove(Obj.m_ForceMove),
	m_Phase2(Obj.m_Phase2),
	m_Timer(Obj.m_Timer),
	m_Rugname(Obj.m_Rugname),
	m_Action(Obj.m_Action),
	m_SpawnQueue(Obj.m_SpawnQueue),
	m_InAction(Obj.m_InAction),
	m_LastCutScene(Obj.m_LastCutScene)
{
}

CDaiManjiElite::~CDaiManjiElite()
{

}


bool CDaiManjiElite::Init(CGameObject* Obj)
{
	//CDaiManji 패스
	CEnemy::Init(Obj);

	SetRenderLayer(ERenderLayer::Default);

	SetCullingDelete(ECullingDelete::All, false);
	SetCullingDelete(ECullingDelete::CullingOn, false);

	SetAnimation("DaiManjiNormal");
	SetAnimation("DaiManjiHatchClosed");

	m_HP = 800;
	m_MaxSpeed = 100.f;
	SetDeAccel(0.8f);
	m_Size.Set(80.f, 80.f);

	m_Coll = AddCollider<CColliderCircle>("DaiManji");
	m_Coll->SetCollisionProfile(ECollisionChannel::Enemy);
	m_Coll->SetRadius(50.f);
	m_Coll->SetOffset(0.f, -60.f);


	//랜덤한 x축의 화면 밖에서 생성
	m_Pos.x = (float)(rand() % ORIGINAL_GAME_RES_WIDTH);
	m_Pos.y = -50.f;
	MoveToDest(true, EMoveToMethod::AddForce, Vector2(m_Pos.x, 100.f));

	//회전반경 설정
	m_RotatingRadius = 12.f;


	//EndFunction 등록.
	SetEndFunction<CDaiManjiElite>("NormalUFOSpawn", this, &CDaiManjiElite::CreateUFO);
	SetEndFunction<CDaiManjiElite>("DaiManjiDeploy", this, &CDaiManjiElite::DaiManjiDeployEnd);
	SetEndFunction<CDaiManjiElite>("DaiManjiReverse", this, &CDaiManjiElite::DaiManjiReverseEnd);
	SetEndFunction<CDaiManjiElite>("DaiManjiCharging", this, &CDaiManjiElite::DaiManjiChargingEnd);
		

	//주인 씬의 주소를 미리 다이나믹 캐스팅을 통해 받아놓는다.
	m_MainScene = dynamic_cast<CMainScene*>(m_Scene);
	if (!m_MainScene)
		return false;


	//사망시 폭발이펙트 시간 측정용 타이머
	m_ExplosionTimer.InitCooltime(0.1f);


	m_isKey = true;
	return true;
}


void CDaiManjiElite::Update(float DeltaTime)
{
	CEnemy::Update(DeltaTime);

	if (m_Phase2 == false)
	{
		if (m_HP > 0)
		{
			//이동 관련 -> 플레이어의 위치에 따라서 수평으로 이동한다.
			CPlayer* Player = m_Scene->GetPlayer();
			if (Player)
			{
				if (!m_ForceMove && !m_MoveToOn)
				{
					//이게 음수면 플레이어는 왼쪽, 양수면 플레이어는 오른쪽
					float RelativePos = Player->GetPos().x - m_Pos.x;

					if (RelativePos < 0.f)
					{
						SetDir(1.f, 0.f);
						AddForce(m_MaxSpeed);
					}
					else
					{
						SetDir(-1.f, 0.f);
						AddForce(m_MaxSpeed);
					}

					//좌측 또는 우측 끝으로 몰리면 강제로 가운데로 이동
					if (m_Pos.x - m_Coll->GetInfo().Radius < 0.f
						|| m_Pos.x + m_Coll->GetInfo().Radius >(float)ORIGINAL_GAME_RES_WIDTH)
					{
						m_ForceMove = true;
						MoveToDest(true, EMoveToMethod::AddForce, Vector2((float)ORIGINAL_GAME_RES_WIDTH / 2.f, m_Pos.y));
					}
				}

				//강제 이동 명령이 종료되면 다시 알아서 이동하도록 설정.
				else if (m_ForceMove && !m_MoveToOn)
				{
					m_ForceMove = false;
				}
			}

			//회전 관련
			//각도가 360도를 넘어가면, 360도를 뺴줘서 숫자가 너무 커지지 않도록 해준다.
			m_RotatingAngle += 360.f * DeltaTime;
			if (m_RotatingAngle >= 1440.f)
				m_RotatingAngle -= 1440.f;

			//라디안값을 구한다.
			float RotAngleRad = DegreeToRadian(m_RotatingAngle);

			//라디안값을 통해 토네이도 총알의 중심점을 구해낸다.
			m_RenderPos.x = m_Pos.x + (cosf(RotAngleRad) * m_RotatingRadius);
			m_RenderPos.y = m_Pos.y + (sinf(RotAngleRad) * m_RotatingRadius);
			if (m_Coll)
				m_Coll->SetFixedPos(m_RenderPos);


			//이동하는 동안에도 루틴 처리
			Routine(DeltaTime);
		}
		else//2페이즈 넘어가기 전 처리
		{
			
			m_ExplosionTimer.UpdateCooltime(DeltaTime);
			if (m_ExplosionTimer.EnterCooltime())
			{
				Vector2 LT = m_Pos - (m_Size * m_Pivot);
				LT.x += (float)(rand() % (int)m_Size.x);
				LT.y += (float)(rand() % (int)m_Size.y);

				//지속 폭발함과 동시에
				CExplosion* Explosion = m_Scene->CreateObject<CExplosion>("DaiManjiExplosion");
				Explosion->SetEssential(EExplosionSize::Midium, LT);
				SoundPlay("SmallExplosion");
			}

			//이동이 끝나면 제자리에서 3초간 폭발 모션 재생 후 위로 들어간다.
			if (!m_MoveToOn)
			{
				m_Timer.UpdateCooltime(DeltaTime);
				if (m_Timer.EnterCooltime())
				{
					m_HP = 1000;

					m_Phase2 = true;
					m_Routine = 0;
					m_FirstEnter = true;

					//이전에 사용하던 NormalUFOSpawn 함수는 사용할수없음.
					//새로운 함수를 등록.
					SetEndFunction<CDaiManjiElite>("NormalUFOSpawn", this, &CDaiManjiElite::NormalUFOSpawn);

					SetEndFunction<CDaiManjiElite>("EliteUFOSpawn", this, &CDaiManjiElite::EliteUFOSpawn);
				}
			}
				
		}
	}
	else
	{
		if(m_HP > 0)
		{
			Phase2Routine(DeltaTime);
		}
		
	}

	if (m_LastCutScene)
	{
		m_ExplosionTimer.UpdateCooltime(DeltaTime);
		if (m_ExplosionTimer.EnterCooltime())
		{
			Vector2 LT = m_Pos - (m_Size * m_Pivot);
			LT.x = (float)(rand() % ORIGINAL_GAME_RES_WIDTH);
			LT.y += (float)(rand() % (int)m_Size.y);

			SoundPlay("SmallExplosion");

			//지속 폭발함과 동시에
			CExplosion* Explosion = m_Scene->CreateObject<CExplosion>("DaiManjiExplosion");
			Explosion->SetEssential(EExplosionSize::Midium, LT);
		}

		m_Timer.UpdateCooltime(DeltaTime);
		if (m_Timer.EnterCooltime())
		{
			if (m_Rugname)
			{
				m_Rugname->SetActive(false);
				m_Rugname = nullptr;
				SetActive(false);
			}

			SoundPlay("BigExplosion");
			
			CExplosion* Explosion = m_Scene->CreateObject<CExplosion>("DaiManjiExplosion");
			Explosion->SetEssential(EExplosionSize::Big, Vector2(m_Pos.x - 140.f, m_Pos.y));

			Explosion = m_Scene->CreateObject<CExplosion>("DaiManjiExplosion");
			Explosion->SetEssential(EExplosionSize::Big, Vector2(m_Pos.x - 70.f, m_Pos.y));

			Explosion = m_Scene->CreateObject<CExplosion>("DaiManjiExplosion");
			Explosion->SetEssential(EExplosionSize::Big, m_Pos);

			Explosion = m_Scene->CreateObject<CExplosion>("DaiManjiExplosion");
			Explosion->SetEssential(EExplosionSize::Big, Vector2(m_Pos.x +70.f, m_Pos.y));

			Explosion = m_Scene->CreateObject<CExplosion>("DaiManjiExplosion");
			Explosion->SetEssential(EExplosionSize::Big, Vector2(m_Pos.x + 140.f, m_Pos.y));

			CGameManager::GetInst()->SetWhiteOut(2);
		}
	}



}

void CDaiManjiElite::PostUpdate(float DeltaTime)
{
	CEnemy::PostUpdate(DeltaTime);

}

void CDaiManjiElite::Render(HDC hDC, float DeltaTime)
{
	if (m_HP <= 0 || m_Phase2)
		m_RenderPos = m_Pos;

	CDaiManji::Render(hDC, DeltaTime);
}


int CDaiManjiElite::InflictDamage(int Damage)
{
	//아이템 드랍은 CEnemy에서 처리.
	int DMG = CEnemy::InflictDamage(Damage);

	m_HP -= DMG;
	StartReaction(0.1f, false, EReactionChannel::HitReaction);

	if (m_HP <= 0 && !m_Phase2)
	{
		//해치액션 종료하고
		StopAnimation((int)EDaiManjiLayerInfo::HatchAction);

		//해치 닫고
		if(!CheckAnimationPlaying("DaiManjiHatchClosed"))
			ChangeAnimation("DaiManjiHatchClose");

		//속도 줄이고
		SetMaxSpeed(30.f);

		//타이머를 설정해서 폭발 이펙트를 생성.
		m_Timer.InitCooltime(2.f);
		m_Timer.EnterCooltime();

		//2페이즈 도킹 진입해야함. 잠깐 충돌체만 지웠다 다시 생성
		ClearCollider();
		m_Coll = nullptr;

		//위치를 가운데로 이동하고 2페이즈 돌입
		m_Pos = m_RenderPos;
		MoveToDest(true, EMoveToMethod::MoveDir, Vector2((float)(ORIGINAL_GAME_RES_WIDTH / 2), 200.f));

		//메인씬에 2페이즈 돌입했다고 전달
		if (m_MainScene)
			m_MainScene->SetCutsceneMode(true);

		//플레이어에 2페이즈 돌입했다고 전달
		m_Scene->GetPlayer()->SetCutsceneMode(true);
	}
	else if (m_HP <= 0 && m_Phase2)
	{
		m_LastCutScene = true;

		//액션 관련 애니메이션 레이어들을 모두 중지
		StopAnimation(2);
		StopAnimation(3);
		StopAnimation(4);

		//메인씬에 컷씬 돌입했다고 전달
		if (m_MainScene)
			m_MainScene->SetCutsceneMode(true);

		//플레이어에 컷씬 돌입했다고 전달
		m_Scene->GetPlayer()->SetCutsceneMode(true);

		//타이머를 설정
		m_Timer.InitCooltime(5.f);
		m_Timer.EnterCooltime();
	}



	return DMG;
}

void CDaiManjiElite::SetEssential(float DestPosX)
{
	m_MoveToDest.x = DestPosX;
}

void CDaiManjiElite::Start()
{
	m_Start = true;
}





void CDaiManjiElite::Routine(float DeltaTime)
{
	switch (m_Routine)
	{
	case (UINT8)EDaiManjiRoutine::Moveto:
	{
		//이동 종료되면 다음 루틴으로
		if (!m_MoveToOn)
			GoNextRoutine();
	}
	break;
	case (UINT8)EDaiManjiRoutine::Wait1:
	{
		if (CheckFirstEnter())
		{
			//3~5초마다 UFO 하나 생성
			float WaitTime = (float)(rand() % 10 + 40) / 10.f;
			m_Timer.InitCooltime(WaitTime);
			m_Timer.EnterCooltime();
		}
		if (m_Timer.UpdateCooltime(DeltaTime))
			GoNextRoutine();


	}
	break;
	case (UINT8)EDaiManjiRoutine::Spawn:
	{
		if (CheckFirstEnter())
		{
			SpawnMiniUFO();
		}
		if (m_isBoss)
			GoNextRoutine();
		else
		{
			m_Routine = (UINT)EDaiManjiRoutine::Wait1;
			m_FirstEnter = true;
		}

	}

	}
}

void CDaiManjiElite::Phase2Routine(float DeltaTime)
{
	switch (m_Routine)
	{
	case EDMPhase0_Docking:
	{
		if (CheckFirstEnter())
		{
			SoundPlay("DaiManjiDocking");

			ChangeAnimation("DaiManjiDocking");
			MoveToDest(true, EMoveToMethod::MoveDir, Vector2(m_Pos.x, 110.f));

			//라그네임도 소환
			m_Rugname = m_Scene->CreateObject<CRugname>("Rugname");
			if (!m_Rugname)
				SetActive(false);
		}

		//이동이 종료되면, 라그네임과 같이 위로 이동한다.
		if (GetMoveToDone() && m_Rugname->GetMoveToDone())
		{
			GoNextRoutine();
		}
		
		break;
	}
	case EDMPhase1://2초 대기 후 라그네임과 함께 위로 올라간다.
	{
		if (CheckFirstEnter())
		{
			m_Timer.InitCooltime(2.f);
			m_Timer.EnterCooltime();
		}
		if (m_Timer.UpdateCooltime(DeltaTime))
		{
			SetMaxSpeed(20.f);
			m_Rugname->SetMaxSpeed(20.f);

			float MoveDist = -40.f;

			MoveToDest(true, EMoveToMethod::MoveDir, Vector2(m_Pos.x, m_Pos.y + MoveDist));

			Vector2 RugnamePos = m_Rugname->GetPos();
			RugnamePos.y += MoveDist;
			m_Rugname->MoveToDest(true, EMoveToMethod::MoveDir, RugnamePos);

			GoNextRoutine();
		}

		break;
	}
	case EDMPhase2://잠깐 대기
	{
		if (CheckFirstEnter())
		{
			m_Timer.InitCooltime(2.f);
			m_Timer.EnterCooltime();

			//이제 해치를 연상태로 냅둔다.
			ChangeAnimation("DaiManjiNormal");
			ChangeAnimation("DaiManjiHatchOpen");
		}

		if (m_Timer.UpdateCooltime(DeltaTime))
			GoNextRoutine();


		break;
	}
	case EDMPhase3://여기서 도착하면 전투루틴 시작
	{
		if (!m_MoveToOn)
		{
			if (CheckFirstEnter())
			{
				m_Scene->GetPlayer()->SetCutsceneMode(false);
				m_MainScene->SetCutsceneMode(false);
				m_Timer.InitCooltime(0.1f);
				m_Timer.EnterCooltime();

				m_Coll = AddCollider<CColliderCircle>("DaiManji");

				if (!m_Coll)
					return;
				m_Coll->SetCollisionProfile(ECollisionChannel::Enemy);
				m_Coll->SetRadius(50.f);
				m_Coll->SetOffset(0.f, -60.f);
			}
		}

		if (m_Timer.UpdateCooltime(DeltaTime))
			GoNextRoutine();
		break;
	}
	case EDMPhase4_BattleStart:
	{
		if (CheckFirstEnter())
		{
			m_Timer.InitCooltime(2.f);
			m_Timer.EnterCooltime();
		}
		if (m_Timer.UpdateCooltime(DeltaTime))
			GoNextRoutine();

		break;
	}
	case EDMPhase5_RandomAction:
	{
		if (CheckFirstEnter())
		{
			if (m_MainScene->GetMiniUFOCounter())
			{
				//랜덤 액션을 지정해서, 해당 액션을 시행한다.
				m_Action = (EDaiManjiEliteActions)(rand() % ((int)EDaiManjiEliteActions::End - 1) + (int)EDaiManjiEliteActions::SpawnElite);
			}
			else
			{//UFO가 최대로 소환되어있으면 에너지볼만 발사한다.
				m_Action = (EDaiManjiEliteActions::Energyball);
			}

			//지정한 액션을 실행.
			AttackByAction();
		}

		if (!m_InAction)
		{
			//다시 액션 원점으로 돌아간다.
			m_Routine = (int)EDaiManjiElitePhase2Flag::EDMPhase4_BattleStart;
			m_FirstEnter = true;
		}



		break;
	}

	default:
		break;
	}

}

void CDaiManjiElite::AttackByAction()
{
	m_InAction = true;

	switch (m_Action)
	{
	case Idle:
		return;
		break;
	case SpawnElite:
	{
		if (m_MainScene->GetMiniUFOCounter())
		{
			m_SpawnQueue = 2;
			ChangeAnimation("EliteUFOSpawn");
		}
		else
		{
			m_InAction = false;
			StopAnimation("EliteUFOSpawn");
		}
			
		break;
	}
		
	case SpawnNormal:
	{
		if (m_MainScene->GetMiniUFOCounter())
		{
			m_SpawnQueue = 2;
			ChangeAnimation("NormalUFOSpawn");
		}
		else
		{
			m_InAction = false;
			StopAnimation("NormalUFOSpawn");
		}
			
		break;
	}
		
	case Energyball:
	{
		ChangeAnimation("DaiManjiDeploy");
	}
		break;
	default:
		m_InAction = false;
		break;
	}
}

void CDaiManjiElite::EliteUFOSpawn()
{
	SoundPlay("MiniUFOSpawn");
	CMiniUFOElite* UFO = m_Scene->CreateObject<CMiniUFOElite>("MiniUFO");
	UFO->SetPos(m_RenderPos.x - 2.f, m_RenderPos.y + 26.f);

	--m_SpawnQueue;
	if (m_SpawnQueue <= 0 || !m_MainScene->GetMiniUFOCounter())
	{
		m_SpawnQueue = 0;
		m_InAction = false;
		StopAnimation(2);
	}

}

void CDaiManjiElite::NormalUFOSpawn()
{
	SoundPlay("MiniUFOSpawn");
	CMiniUFO* UFO = m_Scene->CreateObject<CMiniUFO>("MiniUFO");
	UFO->SetPos(m_RenderPos.x - 2.f, m_RenderPos.y + 26.f);

	--m_SpawnQueue;
	if (m_SpawnQueue <= 0 || !m_MainScene->GetMiniUFOCounter())
	{
		m_SpawnQueue = 0;
		m_InAction = false;
		StopAnimation(2);
	}
}

void CDaiManjiElite::DaiManjiDeployEnd()
{
	SoundPlay("Charge1");

	ChangeAnimation("DaiManjiDeathRay", true);
	ChangeAnimation("DaiManjiCharging", true);
}

void CDaiManjiElite::DaiManjiChargingEnd()
{
	//에너지볼 생성 및 다시 열재생
	CProjectileEnemy* Proj = m_Scene->CreateObject< CProjectileEnemy>("DaiManjiProj");
	CPlayer* Player = m_Scene->GetPlayer();
	if (!Player)
		return;
	Vector2 Dir = (Player->GetPos() - m_Pos).Normalize();
	Proj->SetEssential(EProjectileEnemy::DaiManjiAttack, Dir, Vector2(m_Pos.x, m_Pos.y + 50.f));
	
	ChangeAnimation("DaiManjiReverse", true);
	m_InAction = false;
}

void CDaiManjiElite::DaiManjiReverseEnd()
{
	m_InAction = false;
}





