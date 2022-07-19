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

//충돌체와의 충돌 시 처리를 위함
#include "ColliderCircle.h"
#include "ColliderLine.h"

//디버그 오브젝트 렌더링을 할지
#include "../GameManager.h"


CColliderBox::CColliderBox():
	m_Info(),
	m_Size(),
	m_Pivot(0.5f, 0.5f)
{
	SetTypeID<CColliderBox>();

	//생성자에서 아예 Box로 설정해준다.
	m_ColliderType = EColliderType::Box;
}

CColliderBox::CColliderBox(const CColliderBox& col):
	CCollider(col),
	m_Info(col.m_Info),
	m_Size(col.m_Size),
	m_Pivot(col.m_Pivot)
{
	m_ColliderType = EColliderType::Box;
}

CColliderBox::~CColliderBox()
{
}

BoxInfo CColliderBox::GetInfo() const
{
	return m_Info;
}

void CColliderBox::SetFixedPos(const Vector2& Pos)
{
	CCollider::SetFixedPos(Pos);

	m_Info.LT = Pos + m_Offset - (m_Size * m_Pivot);
	m_Info.RB = m_Info.LT + m_Size;

	m_Bottom = m_Info.RB.y;
}

void CColliderBox::SetOffset(const Vector2& Offset)
{
	if (m_isFixed)	//혹시 기존에 쓰고있던 오프셋값이 있을수 있으므로 기존 오프셋값을 뺴고 더해준다.
	{
		m_Info.LT -= m_Offset;
		m_Info.LT += Offset;

		m_Info.RB -= m_Offset;
		m_Info.RB += Offset;
	}

	CCollider::SetOffset(Offset);
}

void CColliderBox::SetOffset(float _x, float _y)
{
	if (m_isFixed)	//혹시 기존에 쓰고있던 오프셋값이 있을수 있으므로 기존 오프셋값을 뺴고 더해준다.
	{
		m_Info.LT -= m_Offset;
		m_Info.LT.x += _x;
		m_Info.LT.y += _y;

		m_Info.RB -= m_Offset;
		m_Info.RB.x += _x;
		m_Info.RB.y += _y;
	}

	CCollider::SetOffset(_x, _y);
}


void CColliderBox::SetSize(const Vector2& Size)
{
	m_Size = Size;
}

void CColliderBox::SetSize(float SizeX, float SizeY)
{
	m_Size.x = SizeX;
	m_Size.y = SizeY;
}

void CColliderBox::SetPivot(const Vector2& Pivot)
{
	m_Pivot = Pivot;
}

void CColliderBox::SetPivot(float PivotX, float PivotY)
{
	m_Pivot.x = PivotX;
	m_Pivot.y = PivotY;
}

//void CColliderBox::SetExtent(float Width, float Height)
//{
//	m_Width = Width;
//	m_Height = Height;
//}



bool CColliderBox::Collision(CCollider* Dest)
{
	//부딫힌 상대방의 충돌체 타입에 따라 바뀐다.
	switch (Dest->GetColliderType())
	{
	case EColliderType::Box:
		return CCollisionManager::GetInst()->CollisionBoxToBox(this, (CColliderBox*)Dest);
	case EColliderType::Circle:
		return CCollisionManager::GetInst()->CollisionBoxToCircle(this, (CColliderCircle*)Dest);
	case EColliderType::Line:
		return CCollisionManager::GetInst()->CollisionBoxToLine(this, (CColliderLine*)Dest);

	}



	return false;
}

bool CColliderBox::CollisionPoint(const Vector2& Point)
{
	return CCollisionManager::GetInst()->CollisionPointToBox(Point, this);
}


bool CColliderBox::Init()
{
	if (!CCollider::Init())
		return false;

	return true;
}

void CColliderBox::Start()
{
	m_Start = true;

	if (!m_isFixed)
	{
		Vector2 Pos = m_OwnerObj->GetPos();

		m_Info.LT = Pos + m_Offset - (m_Size * m_Pivot);
		m_Info.RB = m_Info.LT + m_Size;

		m_Bottom = m_Info.RB.y;
	}
}

void CColliderBox::Update(float DeltaTime)
{
	if (!m_Start)
		Start();
}

void CColliderBox::PostUpdate(float DeltaTime)
{
	if (!m_Start)
		Start();
	else 
	{
		if (!m_isFixed)
		{
			Vector2 Pos = m_OwnerObj->GetPos();

			m_Info.LT = Pos + m_Offset - (m_Size * m_Pivot);
			m_Info.RB = m_Info.LT + m_Size;

			m_Bottom = m_Info.RB.y;
		}
	}
}

void CColliderBox::Render(HDC hDC, float DeltaTime)
{
	if (!m_Start)
		Start();

#ifdef _DEBUG
	HBRUSH Brush = CGameManager::GetInst()->GetBrush(EBrushType::Green);
	
	//만약 리스트가 비어있지 않거나
	//isMouseColliding이 true면 충돌중이라는 뜻이므로 
	if (!m_listCollision.empty() || m_isMouseColliding)
	{
		Brush = CGameManager::GetInst()->GetBrush(EBrushType::Red);
	}

	//카메라의 위치를 빼서 실제 그려질 위치를 계산한다.
	Vector2 PosLT = m_Info.LT - m_Scene->GetCamera()->GetPos();
	Vector2 PosRB = m_Info.RB - m_Scene->GetCamera()->GetPos();

	RECT Rect;
	Rect.left = (long)PosLT.x;
	Rect.top = (long)PosLT.y;
	Rect.right = (long)PosRB.x;
	Rect.bottom = (long)PosRB.y;

	//사각형을 그려낸다.
	FrameRect(hDC, &Rect, Brush);

#endif
}
