#include "ColliderPoint.h"

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

CColliderPoint::CColliderPoint()
{
	SetTypeID<CColliderPoint>();

	//�����ڿ��� �ƿ� Box�� �������ش�.
	m_ColliderType = EColliderType::Circle;
}

CColliderPoint::CColliderPoint(const CColliderPoint& col) :
	CCollider(col),
	m_Info(col.m_Info)
{
	m_ColliderType = EColliderType::Circle;
}

CColliderPoint::~CColliderPoint()
{
}

const Vector2& CColliderPoint::GetInfo() const
{
	return m_Info;
}

void CColliderPoint::SetPoint(const Vector2& Point)
{
	m_Info = Point;
}



bool CColliderPoint::Collision(CCollider* Dest)
{
	//�΋H�� ������ �浹ü Ÿ�Կ� ���� �ٲ��.
	switch (Dest->GetColliderType())
	{
	case EColliderType::Box:
		return CCollisionManager::GetInst()->CollisionPointToBox(this, (CColliderBox*)Dest);
	case EColliderType::Circle:
		return CCollisionManager::GetInst()->CollisionPointToCircle(this, (CColliderCircle*)Dest);
	//��-�� �浹�� x
	case EColliderType::Point:
		break;
	}

	return false;
}



bool CColliderPoint::Init()
{
	if (!CCollider::Init())
		return false;

	return true;
}

void CColliderPoint::Update(float DeltaTime)
{
}

void CColliderPoint::PostUpdate(float DeltaTime)
{
	if (m_isFixed)
		m_Info = m_FixedPos;
	else
		m_Info = m_OwnerObj->GetPos() + m_Offset;
}

void CColliderPoint::Render(HDC hDC, float DeltaTime)
{

#ifdef _DEBUG

	HPEN Pen = CGameManager::GetInst()->GetPen(EBrushType::Green);

	//���� ����Ʈ�� ������� �ʴٸ� �浹���̶�� ���̹Ƿ� 
	if (!m_CollidingList.empty())
	{
		Pen = CGameManager::GetInst()->GetPen(EBrushType::Red);
	}

	//��� �귯�ø� �����ϰ� �ٲ� �ش�.
	HPEN PrevPen = (HPEN)SelectObject(hDC, Pen);
	HBRUSH PrevBrush = (HBRUSH)SelectObject(hDC, GetStockObject(GRAY_BRUSH));



	//ī�޶��� ��ġ�� ���� ���� �׷��� ��ġ�� ����Ѵ�.
	//�� �浹ü������ ���ü��� ���� ��¦ ū ���� �׷��ش�.
	Vector2 PosLT = m_Info - 3.f - m_OwnerScene->GetCamera()->GetPos();
	Vector2 PosRB = m_Info + 3.f - m_OwnerScene->GetCamera()->GetPos();


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
