#include "Projectile.h"

#include "../GameManager.h"

#include "../ScoreManager.h"

CProjectile::CProjectile() :
	m_isReady(false),
	m_Damage(10)
{
	SetTypeID<CProjectile>();
}

CProjectile::CProjectile(const CProjectile& Obj) :
	CGameObject(Obj),
	m_isReady(Obj.m_isReady),
	m_Damage(Obj.m_Damage)
{
}

CProjectile::~CProjectile()
{
}

bool CProjectile::Init(CGameObject* Obj)
{
	CGameObject::Init(Obj);

	SetCullingDelete(ECullingDelete::All, true);

	return true;
}

void CProjectile::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	//m_Distance += m_MaxSpeed * DeltaTime;

	MoveDir(m_Dir);
}

void CProjectile::PostUpdate(float DeltaTime)
{
	//�ı� ������ �����Ǹ� �Ѿ� ����
	//�Ÿ��� ����ų�, �Ѿ� �⺻������ ���� �ʾҴٸ� �׳� ����
	if (!m_isReady)
	{
		SetActive(false);
	}


	CGameObject::PostUpdate(DeltaTime);
}

void CProjectile::Render(HDC hDC, float DeltaTime)
{
	CGameObject::Render(hDC, DeltaTime);
}

void CProjectile::SetDamage(int Damage)
{
	m_Damage = Damage;
}


void CProjectile::SetEssential(Vector2 Dir, Vector2 Pos)
{
	m_isReady = true;

	m_Dir = Dir;
	m_Pos = Pos;
}

void CProjectile::SetEssential(float DirX, float DirY, float PosX, float PosY)
{
	m_isReady = true;

	m_Dir.x = DirX;
	m_Dir.y = DirY;
	m_Pos.x = PosX;
	m_Pos.y = PosY;
}

void CProjectile::SetOffset(const Vector2& Offset)
{
	m_Pos += Offset;
}

void CProjectile::SetOffset(float x, float y)
{
	m_Pos.x += x;
	m_Pos.y += y;
}

void CProjectile::SetOffsetX(float x)
{
	m_Pos.x += x;
}

void CProjectile::SetOffsetY(float y)
{
	m_Pos.y += y;
}

void CProjectile::CollisionBegin(CCollider* Src, CCollider* Dest)
{

}
