#include "UFO.h"

#include "../Scene/SceneManager.h"
#include "../GameManager.h"
#include "Player.h"

//발사체
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

//위치 계산 카메라
#include "../Scene/Camera.h"


CUFO::CUFO():
	m_Timer()
{
	SetTypeID<CUFO>();
}

CUFO::CUFO(const CUFO& Obj) :
	CEnemy(Obj),
	m_Timer()
{
}

CUFO::~CUFO()
{

}

bool CUFO::LoadResource()
{
	CreateAnimation();

	if (!LoadCSVPreset(TEXT("Enemy/UFO.csv")))
		return false;

	return true;
}

bool CUFO::Init(CGameObject* Obj)
{
	CCharacter::Init(Obj);

	SetCullingDelete(ECullingDelete::All, false);
	SetCullingDelete(ECullingDelete::CullingOn, false);

	SetAnimation("UFO_Main");

	m_HP = 100;
	m_MaxSpeed = 600.f;
	SetDeAccel(0.3f);

	CColliderBox* Coll = AddCollider<CColliderBox>("UFO");
	Coll->SetPivot(0.5f, 1.f);
	Coll->SetSize(53.f, 23.f);
	Coll->SetOffset(0.f, -10.f);
	Coll->SetCollisionProfile(ECollisionChannel::Enemy);

	//CColliderCircle* Coll = AddCollider<CColliderCircle>("UFO");
	//Coll->SetRadius(50.f);

	//랜덤한 x축의 화면 밖에서 생성
	m_Pos.x = (float)(rand() % ORIGINAL_GAME_RES_WIDTH);
	m_Pos.y = -50.f;


	m_Animation->AddNotify("UFO_Attack", 7, this, &CUFO::AttackFrame7Notify);




	return true;
}


void CUFO::Update(float DeltaTime)
{
	CCharacter::Update(DeltaTime);


	Routine(DeltaTime);
}

void CUFO::PostUpdate(float DeltaTime)
{
	CCharacter::PostUpdate(DeltaTime);

	////체력 확인
	//if (m_HP <= 0.f)
	//	SetActive(false);
}

void CUFO::Render(HDC hDC, float DeltaTime)
{
	CCharacter::Render(hDC, DeltaTime);
}


int CUFO::InflictDamage(int Damage)
{
	//아이템 드랍은 CEnemy에서 처리.
	int DMG = CEnemy::InflictDamage(Damage);
	
	m_HP -= DMG;
	StartReaction(0.1f, false, EReactionChannel::HitReaction);

	if (m_HP <= 0)
	{
		SetActive(false);
		ItemDrop();
		SoundPlay("MidExplosion");
		CExplosion* Explosion = m_Scene->CreateObject<CExplosion>("UFOExplosion");
		Explosion->SetEssential(EExplosionSize::Big, m_Pos);
	}

	return DMG;
}

void CUFO::Start()
{
	m_Start = true;


}

void CUFO::SetEssential(bool isKey)
{
	m_isKey = isKey;
}

void CUFO::AttackFrame7Notify()
{
	SoundPlay("UFOLaser");
	CProjectileEnemy* Proj = m_Scene->CreateObject< CProjectileEnemy>("UFOProj");
	Proj->SetEssential(EProjectileEnemy::UFOLaser, Vector2(0.f, 1.f), m_Pos);
}



void CUFO::Routine(float DeltaTime)
{
	switch (m_Routine)
	{
	case (UINT8)EUFORoutine::Moveto:
	{
		//10~100 사이의 위치에 랜덤으로 이동한다.
		int RandPosX = rand() % ORIGINAL_GAME_RES_WIDTH + 15;
		if (RandPosX > ORIGINAL_GAME_RES_WIDTH)
			RandPosX -= 100;

		int RandPosY = rand() % 51 + 40;

		MoveToDest(true, EMoveToMethod::AddImpact, Vector2((float)RandPosX, (float)RandPosY));

		//1초~2초 대기
		float rnd = (float)(rand() % 11 + 20);
		rnd /= 10.f;
		//타이머 on
		m_Timer.InitCooltime(rnd);
		m_Timer.EnterCooltime();

		++m_Routine;
	}
		break;
	case (UINT8)EUFORoutine::Wait:
	{
		if (m_Timer.UpdateCooltime(DeltaTime))
			++m_Routine;
	}
		break;
	case (UINT8)EUFORoutine::MovetoPlayer:
	{
		CPlayer* Player = m_Scene->GetPlayer();
		Vector2 Pos;
		if (Player)
			Pos = Player->GetPos();

		//x축 랜덤
		int rnd = rand() % 6;
		rnd *= rnd % 2 == 0 ? -1 : 1;
		Pos.x += (float)rnd;

		Pos.y = 50.f + (float)(rand() % 80);


		MoveToDest(true, EMoveToMethod::AddImpact, Pos);

		++m_Routine;
	}
		break;
	case (UINT8)EUFORoutine::AttackDelay:
	{
		if (!m_MoveToOn)
		{
			m_Timer.InitCooltime(0.3f);
			m_Timer.EnterCooltime();
			++m_Routine;
		}
	}
	break;
	case (UINT8)EUFORoutine::Attack:
	{
		//이동이 끝나면 레이저를 발사하고 다음 페이즈에서 잠시 대기시킨다.
		if (!m_Timer.UpdateCooltime(DeltaTime))
		{
			m_Animation->SetAnimation("UFO_Attack", true);
			m_Timer.InitCooltime((float)(rand() % 16) / 10.f + 0.5f);
			m_Timer.EnterCooltime();
			++m_Routine;
		}
		
	}
		break;
	case (UINT8)EUFORoutine::KeyCheck:
		if (m_Timer.UpdateCooltime(DeltaTime))
		{

			++m_Routine;

			//키몬스터일 경우 죽을떄까지 처음부터 다시 반복한다.
			if (m_isKey)
			{
				m_Routine = 0;
			}
			else
			{
				//키몬스터가 아닐 경우 바깥으로 도망간 후 제거
				MoveToDest(true, EMoveToMethod::AddImpact, Vector2(m_Pos.x, m_Pos.y - 500.f));
			}
		}
		break;
	case (UINT8)EUFORoutine::Flee:
	{
		//이동이 끝나면 제거된다
		if (!m_MoveToOn)
		{
			SetActive(false);
		}
	}
		break;
	default:
		break;
	}
}

//void CUFO::CollBegin(class CCollider* Src, class CCollider* Dest)
//{
//
//}