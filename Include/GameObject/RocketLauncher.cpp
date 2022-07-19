#include "RocketLauncher.h"

//�浹ü
#include "../Collision/ColliderBox.h"
#include "../../Include/Scene/Scene.h"

//���� ����Ʈ ������
#include "RocketLauncherExplosion.h"

//���� ������
#include "JustPlay.h"

//���� ���Ѽ� ���ٿ�
#include "Player.h"

//�浹�Լ����� DELTA_TIME ����
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
	
	//���� �ִϸ��̼� ����
	SetAnimation("Rocket_Start");

	SetRenderLayer(ERenderLayer::EffectHigh);
	m_MaxSpeed = 50.f;
	//m_Damage = 0;	��ü �������� ����. ���� ���÷��� �������� ����
	SetDir(0.f, -1.f);	//���� ����

	SetSize(10.f, 25.f);
	SetPivot(0.5f, 1.f);

	//�浹ü 1 ���� -> ����� �� �����ϴ� �뵵
	CColliderBox* Coll = AddCollider<CColliderBox>("RocketColl");
	Coll->SetCollisionProfile(ECollisionChannel::PlayerAttack);
	Coll->SetSize(m_Size);
	Coll->SetPivot(0.5f, 1.f);
	Coll->SetOffset(0.f, -25.f);
	Coll->SetCollisionBeginFunc(this, &CRocketLauncher::CollisionBegin);


	Coll = nullptr;
	//�浹ü 2 ���� -> ����� �� �����Ǵ� �뵵
	Coll = AddCollider<CColliderBox>("Homming");
	Coll->SetCollisionProfile(ECollisionChannel::PlayerAttack);
	
	//���� ȭ�� ũ���� �浹ü�� ����
	Coll->SetSize(100.f, (float)ORIGINAL_GAME_RES_HEIGHT);
	Coll->SetPivot(0.5f, 1.f);
	Coll->SetOffset(0.f, -25.f);
	Coll->SetCollisionBeginFunc(this, &CRocketLauncher::HommingBegin);

	//endfunction
	SetEndFunction< CRocketLauncher>("Rocket_Start", this, &CRocketLauncher::RocketStartEnd);


	//�ø� ����
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

	//���� ���
	m_TrailTimer -= DeltaTime;
	if (m_TrailTimer < 0.f && m_TrailOn)
	{
		m_TrailTimer = 0.05f;

		CJustPlay* Trail = m_Scene->CreateObject<CJustPlay>("RocketTrail");

		Trail->SetEssential(m_Pos);
	}



	


	if (CheckColliding(m_Target))
	{
		//x�࿡�� �󸶳� �������ִ����� üũ
		float PosX = m_Pos.x - m_Target->GetPos().x;

		if (PosX > 0.f)	//����̸� ���ʿ� �ִٴ� ���̹Ƿ�
			m_Dir.x -= m_MaxTurnDirX * DeltaTime;
		else if (PosX < 0.f)
			m_Dir.x += m_MaxTurnDirX * DeltaTime;

		SetDir(m_Dir);	//����ȭ
	}
	else
	{
		m_Target = nullptr;
	}



	//�־��� ���� ���� �������� �̵�
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

	//�ǰ�����Ʈ ǥ��
	SFX->SetEssential(m_Dir, Src->GetHitPoint());

	//Dest->GetOwnerObj()->InflictDamage(m_Damage);

	SetActive(false);
}

void CRocketLauncher::HommingBegin(CCollider* Src, CCollider* Dest)
{
	//Ÿ���� �����Ǿ����� ������ ���� ���� �浹���� �� ����� Ÿ������ ����
	if (!m_Target)
		m_Target = Dest->GetOwnerObj();
}
