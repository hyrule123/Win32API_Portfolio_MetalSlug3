#include "ColliderLine.h"


//게임오브젝트 위치 받아오기 위함
#include "../GameObject/GameObject.h"

//CScene 주소를 사용하는 포인터 변수
#include "../Scene/Scene.h"

//HBRUSH 받아오기 위함
#include "../GameManager.h"

//카메라 위치를 통해 렌더링해야함
#include "../Scene/Camera.h"

//각 ECollisionType별 처리 메소드가 여기 들어있음
#include "CollisionManager.h"

//박스 충돌체와의 충돌 시 처리용
#include "ColliderBox.h"
#include "ColliderCircle.h"


CColliderLine::CColliderLine() :
	m_Info{},
	m_isFixedEnd(),
	m_FixedEnd(),
	m_Distance()
{
	SetTypeID<CColliderLine>();

	//생성자에서 아예 Box로 설정해준다.
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
	if (m_isFixed)	//혹시 기존에 쓰고있던 오프셋값이 있을수 있으므로 기존 오프셋값을 뺴고 더해준다.
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
	if (m_isFixed)	//혹시 기존에 쓰고있던 오프셋값이 있을수 있으므로 기존 오프셋값을 뺴고 더해준다.
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
	//끝점 고정을 해제
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
	//부딫힌 상대방의 충돌체 타입에 따라 바뀐다.
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
	//펜과 브러시를 투명하게 바꿔 준다.

	HBRUSH PrevBrush = (HBRUSH)SelectObject(hDC, GetStockObject(HOLLOW_BRUSH));
	bool isCollide = false;

	//만약 리스트가 비어있지 않다면 충돌중이라는 뜻이므로 
	if (!m_listCollision.empty() || m_isMouseColliding)
	{
		isCollide = true;
		Pen = CGameManager::GetInst()->GetPen(EBrushType::Red);
	}
	
	HPEN PrevPen = (HPEN)SelectObject(hDC, Pen);

	if (isCollide)
	{
		//충돌 중일 경우 히트포인트를 작은 원으로 그려준다.
		//카메라의 위치를 빼서 실제 그려질 위치를 계산한다.
		Vector2 PosLT = m_HitPoint - m_Scene->GetCamera()->GetPos() - 10.f;
		Vector2 PosRB = m_HitPoint - m_Scene->GetCamera()->GetPos() + 10.f;
		Ellipse(hDC,
			(int)PosLT.x,
			(int)PosLT.y,
			(int)PosRB.x,
			(int)PosRB.y);
	}

	//카메라의 위치를 빼서 실제 그려질 위치를 계산한다.
	Vector2 PosStart = m_Info.Start - m_Scene->GetCamera()->GetPos();
	Vector2 PosEnd = m_Info.End - m_Scene->GetCamera()->GetPos();

	//선을 그린다
	MoveToEx(hDC, (int)PosStart.x, (int)PosStart.y, NULL);
	LineTo(hDC, (int)PosEnd.x, (int)PosEnd.y);

	




	SelectObject(hDC, PrevPen);
	SelectObject(hDC, PrevBrush);












#endif


};
