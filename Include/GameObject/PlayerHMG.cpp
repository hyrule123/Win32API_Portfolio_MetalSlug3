#include "PlayerHMG.h"

//�浹ü
#include "../Collision/ColliderBox.h"
#include "../../Include/Scene/Scene.h"

//�Ѿ� ����Ʈ ������
#include "PistolHit.h"

#include "../Scene/SceneResource.h"

#include "../ScoreManager.h"

CPlayerHMG::CPlayerHMG()
{
	SetTypeID<CPlayerHMG>();
}

CPlayerHMG::CPlayerHMG(const CPlayerHMG& Obj) :
	CProjectile(Obj)
{
}

CPlayerHMG::~CPlayerHMG()
{
}

bool CPlayerHMG::Init(CGameObject* Obj)
{
	CProjectile::Init(Obj);

	m_MaxSpeed = 600.f;
	m_Damage = 10;

	SetSize(5.f, 30.f);
	SetPivot(0.5f, 0.5f);

	SetRenderLayer(ERenderLayer::EffectLow);

	//�浹ü ����
	CColliderBox* Coll = AddCollider<CColliderBox>("Bullet");

	Coll->SetCollisionProfile(ECollisionChannel::PlayerAttack);
	Coll->SetSize(m_Size);

	//ȣ�� �Լ� ����
	Coll->SetCollisionBeginFunc(this, &CPlayerHMG::CollisionBegin);

	m_isReady = true;

	return true;
}

bool CPlayerHMG::LoadResource()
{
	SetTexture("HMG", TEXT("SFX/HMGSpace.bmp"));
	SetColorKey(0, 248, 0);

	return true;
}

void CPlayerHMG::SetEssential(Vector2 Dir, Vector2 Pos)
{
	m_isReady = true;

	m_Dir = Dir;
	m_Pos.x = Pos.x;
	m_Pos.y = Pos.y - 100.f;
}



void CPlayerHMG::CollisionBegin(CCollider* Src, CCollider* Dest)
{
	m_Scene->GetSceneResource()->SoundPlay("BulletHitSound");

	CPistolHit* Effect = m_Scene->CreateObject<CPistolHit>("BulletSFX");

	//�ణ �����ϰ� �ǰ�����Ʈ ǥ��
	if (Effect)
	{
		Vector2 Pos = Src->GetHitPoint();
		Pos.x += (float)(rand() % 10);
		Pos.y += (float)(rand() % 10);
		Effect->SetPos(Pos);
	}


	Dest->GetOwnerObj()->InflictDamage(m_Damage);
	CScoreManager::GetInst()->AddScore(100);

	SetActive(false);
}