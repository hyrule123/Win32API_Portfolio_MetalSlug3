#include "ColliderCircle.h"
#include "ColliderLine.h"
#include "ColliderBox.h"


//���ӿ�����Ʈ ��ġ �޾ƿ��� ����
#include "../GameObject/GameObject.h"

//CScene �ּҸ� ����ϴ� ������ ����
#include "../Scene/Scene.h"

//HBRUSH �޾ƿ��� ����
#include "../GameManager.h"

//ī�޶� ��ġ�� ���� �������ؾ���
#include "../Scene/Camera.h"

//�� ECollisionType�� ó�� �޼ҵ尡 ���� �������
#include "CollisionManager.h"


CColliderCircle::CColliderCircle():
	m_Info{}
{
	SetTypeID<CColliderCircle>();

	//�����ڿ��� �ƿ� Box�� �������ش�.
	m_ColliderType = EColliderType::Circle;
}

CColliderCircle::CColliderCircle(const CColliderCircle& col) :
	CCollider(col),
	m_Info(col.m_Info)
{
	m_ColliderType = EColliderType::Circle;
}

CColliderCircle::~CColliderCircle()
{
}

void CColliderCircle::SetFixedPos(const Vector2& Pos)
{
	CCollider::SetFixedPos(Pos);

	m_Info.Center = Pos + m_Offset;
}

void CColliderCircle::SetOffset(const Vector2& Offset)
{
	if (m_isFixed)	//Ȥ�� ������ �����ִ� �����°��� ������ �����Ƿ� ���� �����°��� ���� �����ش�.
	{
		m_Info.Center -= m_Offset;
		m_Info.Center += Offset;
	}

	CCollider::SetOffset(Offset);
}

void CColliderCircle::SetOffset(float _x, float _y)
{
	if (m_isFixed)	//Ȥ�� ������ �����ִ� �����°��� ������ �����Ƿ� ���� �����°��� ���� �����ش�.
	{
		m_Info.Center -= m_Offset;
		m_Info.Center.x += _x;
		m_Info.Center.y += _y;
	}

	CCollider::SetOffset(_x, _y);
}


CircleInfo CColliderCircle::GetInfo() const
{
	return m_Info;
}

void CColliderCircle::SetRadius(float Rad)
{
	m_Info.Radius = Rad;
}

bool CColliderCircle::Collision(CCollider* Dest)
{
	//�΋H�� ������ �浹ü Ÿ�Կ� ���� �ٲ��.
	switch (Dest->GetColliderType())
	{
	case EColliderType::Box:
		return CCollisionManager::GetInst()->CollisionCircleToBox(this, (CColliderBox*)Dest);
	case EColliderType::Circle:
		return CCollisionManager::GetInst()->CollisionCircleToCircle(this, (CColliderCircle*)Dest);
	case EColliderType::Line:
		return CCollisionManager::GetInst()->CollisionCircleToLine(this, (CColliderLine*)Dest);
	}

	return false;
}

bool CColliderCircle::CollisionPoint(const Vector2& Point)
{
	return CCollisionManager::GetInst()->CollisionPointToCircle(Point, this);
}


bool CColliderCircle::Init()
{
	if (!CCollider::Init())
		return false;

	return true;
}

void CColliderCircle::Start()
{
	m_Start = true;

	if (m_isFixed)
		m_Info.Center = m_FixedPos + m_Offset;
	else
		m_Info.Center = m_OwnerObj->GetPos() + m_Offset;

	m_Bottom = m_Info.Center.y + m_Info.Radius;
}

void CColliderCircle::Update(float DeltaTime)
{
	if (!m_Start)
		Start();
}

void CColliderCircle::PostUpdate(float DeltaTime)
{
	if (!m_Start)
		Start();
	else
	{
		if (m_isFixed)
			m_Info.Center = m_FixedPos + m_Offset;
		else
			m_Info.Center = m_OwnerObj->GetPos() + m_Offset;

		m_Bottom = m_Info.Center.y + m_Info.Radius;
	}
}

void CColliderCircle::Render(HDC hDC, float DeltaTime)
{
	if (!m_Start)
		Start();

#ifdef _DEBUG

	HPEN Pen = CGameManager::GetInst()->GetPen(EBrushType::Green);

	//���� ����Ʈ�� ������� �ʴٸ� �浹���̶�� ���̹Ƿ� 
	if (!m_listCollision.empty() || m_isMouseColliding)
	{
		Pen = CGameManager::GetInst()->GetPen(EBrushType::Red);
	}

	//��� �귯�ø� �����ϰ� �ٲ� �ش�.
	HPEN PrevPen = (HPEN)SelectObject(hDC, Pen);
	HBRUSH PrevBrush = (HBRUSH)SelectObject(hDC, GetStockObject(HOLLOW_BRUSH));


	

	//ī�޶��� ��ġ�� ���� ���� �׷��� ��ġ�� ����Ѵ�.
	Vector2 PosLT = m_Info.Center- m_Info.Radius - m_Scene->GetCamera()->GetPos();
	Vector2 PosRB = m_Info.Center+ m_Info.Radius - m_Scene->GetCamera()->GetPos();

	

	Ellipse(hDC,
		(int)PosLT.x,
		(int)PosLT.y,
		(int)PosRB.x,
		(int)PosRB.y);

	SelectObject(hDC, PrevPen);
	SelectObject(hDC, PrevBrush);
	//SelectObject(hDC, GetStockObject(WHITE_BRUSH));

#endif


};
