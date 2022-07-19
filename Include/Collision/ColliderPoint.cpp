#include "ColliderPoint.h"

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

CColliderPoint::CColliderPoint()
{
	SetTypeID<CColliderPoint>();

	//생성자에서 아예 Box로 설정해준다.
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
	//부딫힌 상대방의 충돌체 타입에 따라 바뀐다.
	switch (Dest->GetColliderType())
	{
	case EColliderType::Box:
		return CCollisionManager::GetInst()->CollisionPointToBox(this, (CColliderBox*)Dest);
	case EColliderType::Circle:
		return CCollisionManager::GetInst()->CollisionPointToCircle(this, (CColliderCircle*)Dest);
	//점-점 충돌은 x
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

	//만약 리스트가 비어있지 않다면 충돌중이라는 뜻이므로 
	if (!m_CollidingList.empty())
	{
		Pen = CGameManager::GetInst()->GetPen(EBrushType::Red);
	}

	//펜과 브러시를 투명하게 바꿔 준다.
	HPEN PrevPen = (HPEN)SelectObject(hDC, Pen);
	HBRUSH PrevBrush = (HBRUSH)SelectObject(hDC, GetStockObject(GRAY_BRUSH));



	//카메라의 위치를 빼서 실제 그려질 위치를 계산한다.
	//점 충돌체이지만 가시성을 위해 살짝 큰 원을 그려준다.
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
