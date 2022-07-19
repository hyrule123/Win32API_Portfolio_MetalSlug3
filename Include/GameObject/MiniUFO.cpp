#include "MiniUFO.h"

#include "Player.h"

#include "../Scene/MainScene.h"

//�߻�ü
#include "ProjectileEnemy.h"

//����Ʈ
#include "Effect.h"

//�浹ü
#include "../Collision/ColliderBox.h"

//��ġ ��� ī�޶�


CMiniUFO::CMiniUFO():
	m_HitTimer(),
	m_MainScene()
{
	SetTypeID<CMiniUFO>();
}

CMiniUFO::CMiniUFO(const CMiniUFO& Obj) :
	CEnemy(Obj),
	m_HitTimer(Obj.m_HitTimer),
	m_MainScene(Obj.m_MainScene)
{
}

CMiniUFO::~CMiniUFO()
{
	//�ڽ��� ���ŵǾ����� ����
	CMainScene* Scene = dynamic_cast<CMainScene*>(m_Scene);
	if (Scene)
		Scene->AddMiniUFOCounter(-1);
}

bool CMiniUFO::LoadResource()
{
	CreateAnimation();

	if (!LoadCSVPreset(TEXT("Enemy/MiniUFONormal.csv")))
		return false;

	return true;
}

bool CMiniUFO::Init(CGameObject* Obj)
{
	CEnemy::Init(Obj);

	SetCullingDelete(ECullingDelete::All, false);
	SetCullingDelete(ECullingDelete::CullingOn, false);

	m_MainScene = dynamic_cast<CMainScene*>(m_Scene);
	if (!m_MainScene)
		return false;
	

	SetAnimation("MiniUFONormal");

	m_HP = 100;
	m_MaxSpeed = 200.f;
	SetDeAccel(0.8f);
	m_Size.Set(30.f, 30.f);

	CColliderBox* Coll = AddCollider<CColliderBox>("MiniUFO");
	Coll->SetCollisionProfile(ECollisionChannel::Enemy);
	Coll->SetPivot(m_Pivot);
	Coll->SetSize(m_Size);

	//CColliderCircle* Coll = AddCollider<CColliderCircle>("MiniUFO");
	//Coll->SetRadius(50.f);

	//End Function
	//���������� ����
	SetEndFunction<CMiniUFO>("MiniUFODeath", this, &CMiniUFO::MiniUFODeathEndFunc);
	//����
	SetEndFunction<CMiniUFO>("AttackSFX", this, &CMiniUFO::AttackEndFunc);


	//���������� �����Ǹ� �ڽ��� ��ϵǾ����� �˸�
	CMainScene* Scene = dynamic_cast<CMainScene*>(m_Scene);
	if(Scene)
		Scene->AddMiniUFOCounter(1);
		

	// 1/4 Ȯ���� ���� ���
	if (rand() % 3 == 0)
	{
		int RandWeapon = rand() % ((int)EItemList::Bomb + 1);
		SetItemDrop((EItemList)RandWeapon);
	}


	return true;
}


void CMiniUFO::Update(float DeltaTime)
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
				ChangeAnimContinue("MiniUFONormal");
			}
		}
	}

	//������ ������ Ȯ�εǸ� ��� ����
	if (m_MainScene->CheckCutscenePlay())
	{
		if(m_HP > 0)
			InflictDamage(10000);
	}
		
}

void CMiniUFO::PostUpdate(float DeltaTime)
{
	CEnemy::PostUpdate(DeltaTime);


}


int CMiniUFO::InflictDamage(int Damage)
{
	//������ ����� CEnemy���� ó��.
	int DMG = CEnemy::InflictDamage(Damage);

	m_HP -= DMG;

	//0.1�ʰ� �ǰ� �ִϸ��̼��� ���
	m_HitTimer = 0.1f;
	ChangeAnimContinue("MiniUFOHit");

	if (m_HP <= 0)
	{
		SoundPlay("MiniUFODestroy");

		//�̵� ����
		m_Speed = 0;
		m_MoveToOn = false;


		ItemDrop();
		ClearCollider();

		//���⼭�� ���� ��� �ִϸ��̼��� ���.
		StopAnimation("AttackSFX");	//���� �ִϸ��̼� ���̾�
		ChangeAnimation("MiniUFODeath");
	}

	return DMG;
}

void CMiniUFO::SetEssential(float DestPosX)
{
	m_MoveToDest.x = DestPosX;
}

void CMiniUFO::Start()
{
	m_Start = true;


}


void CMiniUFO::Routine(float DeltaTime)
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

		//�̵� ����Ǹ� ���� ��ƾ����
		if (!m_MoveToOn)
			GoNextRoutine();
	}
	break;

	case (UINT8)EMiniUFORoutine::Wait:
	{
		if (CheckFirstEnter())
		{
			//0~1�ʰ� ���
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
			//�ִϸ��̼��� ����ϸ� �˾Ƽ� �� �߻���
			ChangeAnimation("AttackSFX", true);
		}

		//�ٽ� ó������ ���ư���.
		m_Routine = 0;
		m_FirstEnter = true;
	}
	break;
	}
}


void CMiniUFO::AttackEndFunc()
{
	//���� �߻�
	CPlayer* Player = m_Scene->GetPlayer();
	if (Player)
	{
		SoundPlay("MiniUFOLaser");
		Vector2 Dir = (Player->GetPos() - m_Pos).Normalize();
		CProjectileEnemy* Proj = m_Scene->CreateObject< CProjectileEnemy>("MiniUFOProj");
		Proj->SetEssential(EProjectileEnemy::MiniUFOAttack, Dir, m_Pos);
	}
}

void CMiniUFO::MiniUFODeathEndFunc()
{
	SetActive(false);
}
