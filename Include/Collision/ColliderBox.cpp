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

//�浹ü���� �浹 �� ó���� ����
#include "ColliderCircle.h"
#include "ColliderLine.h"

//����� ������Ʈ �������� ����
#include "../GameManager.h"


CColliderBox::CColliderBox():
	m_Info(),
	m_Size(),
	m_Pivot(0.5f, 0.5f)
{
	SetTypeID<CColliderBox>();

	//�����ڿ��� �ƿ� Box�� �������ش�.
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
	if (m_isFixed)	//Ȥ�� ������ �����ִ� �����°��� ������ �����Ƿ� ���� �����°��� ���� �����ش�.
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
	if (m_isFixed)	//Ȥ�� ������ �����ִ� �����°��� ������ �����Ƿ� ���� �����°��� ���� �����ش�.
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
	//�΋H�� ������ �浹ü Ÿ�Կ� ���� �ٲ��.
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
	
	//���� ����Ʈ�� ������� �ʰų�
	//isMouseColliding�� true�� �浹���̶�� ���̹Ƿ� 
	if (!m_listCollision.empty() || m_isMouseColliding)
	{
		Brush = CGameManager::GetInst()->GetBrush(EBrushType::Red);
	}

	//ī�޶��� ��ġ�� ���� ���� �׷��� ��ġ�� ����Ѵ�.
	Vector2 PosLT = m_Info.LT - m_Scene->GetCamera()->GetPos();
	Vector2 PosRB = m_Info.RB - m_Scene->GetCamera()->GetPos();

	RECT Rect;
	Rect.left = (long)PosLT.x;
	Rect.top = (long)PosLT.y;
	Rect.right = (long)PosRB.x;
	Rect.bottom = (long)PosRB.y;

	//�簢���� �׷�����.
	FrameRect(hDC, &Rect, Brush);

#endif
}
