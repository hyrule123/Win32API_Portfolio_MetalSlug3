#include "Splash.h"

//�浹ü
#include "../Collision/ColliderCircle.h"
#include "../../Include/Scene/Scene.h"
#include "../ScoreManager.h"


CSplash::CSplash():
	m_Coll(),
	m_SetActive(true)
{
	SetTypeID<CSplash>();
}

CSplash::CSplash(const CSplash& Obj) :
	CProjectile(Obj),
	m_Coll(),
	m_SetActive(true)
{
}

CSplash::~CSplash()
{
}

bool CSplash::Init(CGameObject* Obj)
{

	SetPivot(0.5f, 0.5f);

	//�浹ü ����
	m_Coll = AddCollider<CColliderCircle>("Splash");

	//ȣ�� �Լ� ����
	m_Coll->SetCollisionBeginFunc(this, &CSplash::CollisionBegin);

	m_SetActive = true;

	return true;
}

void CSplash::PostUpdate(float DeltaTime)
{
	CProjectile::PostUpdate(DeltaTime);

	//�� �������Ӹ� �浹 üũ��
	SetActive(m_SetActive);
	if (m_SetActive)
		m_SetActive = false;
}

void CSplash::SetEssential(const Vector2& Pos, const Vector2& Offset, float Radius, ECollisionChannel Channel, int Damage)
{
	m_Coll->SetFixedPos(Pos);
	m_Coll->SetRadius(Radius);
	m_Coll->SetOffset(Offset);
	m_Coll->SetCollisionProfile(Channel);
	m_Damage = Damage;

	m_isReady = true;
}




void CSplash::CollisionBegin(CCollider* Src, CCollider* Dest)
{
	//�ش� �ݰ濡 �������� �ְ� �������.
	Dest->GetOwnerObj()->InflictDamage(m_Damage);
	CScoreManager::GetInst()->AddScore(100);

	SetActive(false);
}

