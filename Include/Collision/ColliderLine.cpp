#include "ColliderLine.h"


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

//�ڽ� �浹ü���� �浹 �� ó����
#include "ColliderBox.h"
#include "ColliderCircle.h"


CColliderLine::CColliderLine() :
	m_Info{},
	m_isFixedEnd(),
	m_FixedEnd(),
	m_Distance()
{
	SetTypeID<CColliderLine>();

	//�����ڿ��� �ƿ� Box�� �������ش�.
	m_ColliderType = EColliderType::Line;
}

CColliderLine::CColliderLine(const CColliderLine& col) :
	CCollider(col),
	m_Info(col.m_Info),
	m_isFixedEnd(col.m_isFixedEnd),
	m_FixedEnd(col.m_FixedEnd),
	m_Dir(),
	m_Distance()
{
	m_ColliderType = EColliderType::Line;
}

CColliderLine::~CColliderLine()
{
}

void CColliderLine::SetFixedPos(const Vector2& Start)
{
	m_Info.Start = Start + m_Offset;
	m_Distance = m_Info.Start.Distance(m_Info.End);
}

void CColliderLine::SetOffset(const Vector2& Offset)
{
	if (m_isFixed)	//Ȥ�� ������ �����ִ� �����°��� ������ �����Ƿ� ���� �����°��� ���� �����ش�.
	{
		m_Info.Start -= m_Offset;
		m_Info.Start += Offset;

		m_Info.End -= m_Offset;
		m_Info.End += Offset;
	}

	CCollider::SetOffset(Offset);
}

void CColliderLine::SetOffset(float _x, float _y)
{
	if (m_isFixed)	//Ȥ�� ������ �����ִ� �����°��� ������ �����Ƿ� ���� �����°��� ���� �����ش�.
	{
		m_Info.Start -= m_Offset;
		m_Info.Start.x += _x;
		m_Info.Start.y += _y;

		m_Info.End -= m_Offset;
		m_Info.End.x += _x;
		m_Info.End.y += _y;
	}

	CCollider::SetOffset(_x, _y);
}

LineInfo CColliderLine::GetInfo() const
{
	return m_Info;
}

void CColliderLine::SetFixedEnd(const Vector2& End)
{
	m_isFixedEnd = true;
	m_FixedEnd = End;
	m_Info.End = End + m_Offset;
	m_Distance = m_Info.Start.Distance(m_Info.End);
}

void CColliderLine::SetDir(const Vector2& Dir, float Distance)
{
	//���� ������ ����
	m_isFixedEnd = false;
	m_Dir = Dir.Normalize();
	m_Distance = Distance;
}

void CColliderLine::SetDir(float Degree, float Distance)
{
	m_isFixedEnd = false;
	float Rad = DegreeToRadian(Degree);

	m_Dir.x = cosf(Rad);
	m_Dir.y = sinf(Rad);
	m_Distance = Distance;
}

void CColliderLine::SetDistance(float Distance)
{
	m_Distance = Distance;
}


bool CColliderLine::Collision(CCollider* Dest)
{
	//�΋H�� ������ �浹ü Ÿ�Կ� ���� �ٲ��.
	switch (Dest->GetColliderType())
	{
	case EColliderType::Box:
		return CCollisionManager::GetInst()->CollisionLineToBox(this, (CColliderBox*)Dest);
	case EColliderType::Circle:
		return CCollisionManager::GetInst()->CollisionLineToCircle(this, (CColliderCircle*)Dest);
	case EColliderType::Line:
		return CCollisionManager::GetInst()->CollisionLineToLine(this, (CColliderLine*)Dest);
	}

	return false;
}

bool CColliderLine::CollisionPoint(const Vector2& Point)
{
	//return CCollisionManager::GetInst()->CollisionPointToCircle(Point, this);

	return false;
}



float CColliderLine::GetLineInfoEndY() const
{
	return m_Info.End.y;
}

bool CColliderLine::Init()
{
	if (!CCollider::Init())
		return false;

	

	return true;
}

void CColliderLine::Start()
{
	m_Start = true;

	if (!m_isFixed)
	{
		m_Info.Start = m_OwnerObj->GetPos() + m_Offset;
	}
	

	if (!m_isFixedEnd)
	{
		m_Info.End = m_Info.Start + m_Dir * m_Distance;
	}
}

void CColliderLine::Update(float DeltaTime)
{
	if (!m_Start)
		Start();

}

void CColliderLine::PostUpdate(float DeltaTime)
{

	if (!m_Start)
		Start();
	else
	{
		if (!m_isFixed)
		{
			m_Info.Start = m_OwnerObj->GetPos() + m_Offset;
		}

		if (!m_isFixedEnd)
		{
			m_Info.End = m_Info.Start + m_Dir * m_Distance;
		}
	}
	m_Bottom = m_Info.Start.y > m_Info.End.y ? m_Info.Start.y : m_Info.End.y;
}

void CColliderLine::Render(HDC hDC, float DeltaTime)
{
	if (!m_Start)
		Start();

#ifdef _DEBUG

	HPEN Pen = CGameManager::GetInst()->GetPen(EBrushType::Green);
	//��� �귯�ø� �����ϰ� �ٲ� �ش�.

	HBRUSH PrevBrush = (HBRUSH)SelectObject(hDC, GetStockObject(HOLLOW_BRUSH));
	bool isCollide = false;

	//���� ����Ʈ�� ������� �ʴٸ� �浹���̶�� ���̹Ƿ� 
	if (!m_listCollision.empty() || m_isMouseColliding)
	{
		isCollide = true;
		Pen = CGameManager::GetInst()->GetPen(EBrushType::Red);
	}
	
	HPEN PrevPen = (HPEN)SelectObject(hDC, Pen);

	if (isCollide)
	{
		//�浹 ���� ��� ��Ʈ����Ʈ�� ���� ������ �׷��ش�.
		//ī�޶��� ��ġ�� ���� ���� �׷��� ��ġ�� ����Ѵ�.
		Vector2 PosLT = m_HitPoint - m_Scene->GetCamera()->GetPos() - 10.f;
		Vector2 PosRB = m_HitPoint - m_Scene->GetCamera()->GetPos() + 10.f;
		Ellipse(hDC,
			(int)PosLT.x,
			(int)PosLT.y,
			(int)PosRB.x,
			(int)PosRB.y);
	}

	//ī�޶��� ��ġ�� ���� ���� �׷��� ��ġ�� ����Ѵ�.
	Vector2 PosStart = m_Info.Start - m_Scene->GetCamera()->GetPos();
	Vector2 PosEnd = m_Info.End - m_Scene->GetCamera()->GetPos();

	//���� �׸���
	MoveToEx(hDC, (int)PosStart.x, (int)PosStart.y, NULL);
	LineTo(hDC, (int)PosEnd.x, (int)PosEnd.y);

	




	SelectObject(hDC, PrevPen);
	SelectObject(hDC, PrevBrush);












#endif


};
