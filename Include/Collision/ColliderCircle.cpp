#include "ColliderCircle.h"
#include "ColliderLine.h"
#include "ColliderBox.h"


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


CColliderCircle::CColliderCircle():
	m_Info{}
{
	SetTypeID<CColliderCircle>();

	//생성자에서 아예 Box로 설정해준다.
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
	if (m_isFixed)	//혹시 기존에 쓰고있던 오프셋값이 있을수 있으므로 기존 오프셋값을 뺴고 더해준다.
	{
		m_Info.Center -= m_Offset;
		m_Info.Center += Offset;
	}

	CCollider::SetOffset(Offset);
}

void CColliderCircle::SetOffset(float _x, float _y)
{
	if (m_isFixed)	//혹시 기존에 쓰고있던 오프셋값이 있을수 있으므로 기존 오프셋값을 뺴고 더해준다.
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
	//부딫힌 상대방의 충돌체 타입에 따라 바뀐다.
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

	//만약 리스트가 비어있지 않다면 충돌중이라는 뜻이므로 
	if (!m_listCollision.empty() || m_isMouseColliding)
	{
		Pen = CGameManager::GetInst()->GetPen(EBrushType::Red);
	}

	//펜과 브러시를 투명하게 바꿔 준다.
	HPEN PrevPen = (HPEN)SelectObject(hDC, Pen);
	HBRUSH PrevBrush = (HBRUSH)SelectObject(hDC, GetStockObject(HOLLOW_BRUSH));


	

	//카메라의 위치를 빼서 실제 그려질 위치를 계산한다.
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
