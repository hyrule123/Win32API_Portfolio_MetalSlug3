#include "RocketLauncherExplosion.h"

#include "../Collision/ColliderCircle.h"

#include "../Scene/Scene.h"

#include "../ScoreManager.h"

CRocketLauncherExplosion::CRocketLauncherExplosion():
	m_Damage(),
	m_SizeName{},
	m_CurrentSize()
{
	SetTypeID<CRocketLauncherExplosion>();
}

CRocketLauncherExplosion::CRocketLauncherExplosion(const CRocketLauncherExplosion& Obj):
	CEffect(Obj),
	m_Damage(Obj.m_Damage),
	m_CurrentSize(Obj.m_CurrentSize)
{
	for (int i = 0; i < (int)EExplosionSize::Max; ++i)
	{
		m_SizeName[i] = Obj.m_SizeName[i];
	}
}

CRocketLauncherExplosion::~CRocketLauncherExplosion()
{
}

bool CRocketLauncherExplosion::LoadResource()
{
	if (!LoadCSVPreset(TEXT("SFX/RocketLauncherExplosion.csv")))
		return false;

	m_SizeName[(int)EExplosionSize::Small] = "Rocket_SmallExplosion";
	m_SizeName[(int)EExplosionSize::Midium] = "Rocket_MidExplosion";
	m_SizeName[(int)EExplosionSize::Big] = "Rocket_BigExplosion";
	

	return true;
}

bool CRocketLauncherExplosion::Init(CGameObject* Obj)
{
	CEffect::Init(Obj);

	if (!m_Animation->FindAnimInfo(m_Name))
		return false;

	SetAnimation(m_Name);

	//���� ���� ����Ʈ�� ������ ���̾�� default
	m_RenderLayer = ERenderLayer::Default;

	//2������ ��Ƽ���� ����
	AddNotify<CRocketLauncherExplosion>(m_Name, 2, this, &CRocketLauncherExplosion::Frame2Notify);


	return true;
}


void CRocketLauncherExplosion::SetEssential(const Vector2& Dir, const Vector2& Pos)
{
	m_Dir = Dir;
	m_Pos = Pos;

	//�浹ü ����
	CColliderCircle* Coll = AddCollider<CColliderCircle>("Rocket");
	Coll->SetCollisionProfile(ECollisionChannel::PlayerAttack);
	Coll->SetCollisionBeginFunc<CRocketLauncherExplosion>(this, &CRocketLauncherExplosion::CollisionBegin);

	if (m_Name == m_SizeName[(int)EExplosionSize::Small])
	{
		m_CurrentSize = EExplosionSize::Small;

		m_Damage = 30;

		Coll->SetOffset(0.f, -17.f);
		Coll->SetRadius(17.f);
		

	}

	else if (m_Name == m_SizeName[(int)EExplosionSize::Midium])
	{
		m_CurrentSize = EExplosionSize::Midium;

		m_Damage = 20;

		Coll->SetOffset(0.f, -25.f);
		Coll->SetRadius(25.f);

		m_Pos += m_Dir * 5.f;

	}
	else if (m_Name == m_SizeName[(int)EExplosionSize::Big])
	{
		m_CurrentSize = EExplosionSize::Big;

		m_Damage = 10;


		Coll->SetOffset(0.f, -40.f);
		Coll->SetRadius(40.f);

		m_Pos += m_Dir * 10.f;

	}

	Coll->SetFixedPos(m_Pos);
	m_isReady = true;
}

void CRocketLauncherExplosion::CollisionBegin(CCollider* Src, CCollider* Dest)
{
	CScoreManager::GetInst()->AddScore(100);
	Dest->GetOwnerObj()->InflictDamage(m_Damage);
}

void CRocketLauncherExplosion::Frame2Notify()
{
	//�ڽ��� �浹ü ����
	ClearCollider();

	int index = (int)m_CurrentSize + 1;

	//�ִ� ũ���� ��� �ߴ�
	if (index >= (int)EExplosionSize::Max || index < (int)EExplosionSize::Small)
		return;

	//�̿��� ��� �ڽź��� ���� ū ����ü ����
	CRocketLauncherExplosion* Explosion = m_Scene->CreateObject<CRocketLauncherExplosion>(m_SizeName[index]);
	Explosion->SetEssential(m_Dir, m_Pos);
}
