#include "Collider.h"
#include "CollisionManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneCollision.h"

CCollider::CCollider():
	m_OwnerObj(nullptr),
	m_Scene(nullptr),
	m_Offset(),
	m_Bottom(),
	m_isFixed(),
	m_FixedPos(),
	m_HitPoint(),
	m_ColliderType(),
	m_Profile(),
	m_CollisionBegin(nullptr),
	m_CollisionEnd(nullptr),
	m_listCollision(),
	m_CollisionBeginMouse(nullptr),
	m_CollisionEndMouse(nullptr),
	m_isMouseColliding(false),
	m_Start(),
	m_CheckMouseCollision()
{
}


CCollider::CCollider(const CCollider& col):
	CRef(col),
	m_OwnerObj(col.m_OwnerObj),
	m_Scene(col.m_Scene),
	m_Offset(col.m_Offset),
	m_Bottom(col.m_Bottom),
	m_isFixed(col.m_isFixed),
	m_FixedPos(col.m_FixedPos),
	m_HitPoint(),
	m_ColliderType(col.m_ColliderType),
	m_Profile(col.m_Profile),
	m_CollisionBegin(nullptr),	//여기부터는 Init 이후에 설정되므로 초기화
	m_CollisionEnd(nullptr),
	m_listCollision(),
	m_CollisionBeginMouse(nullptr),
	m_CollisionEndMouse(nullptr),
	m_isMouseColliding(false),
	m_Start(),
	m_CheckMouseCollision(col.m_CheckMouseCollision)
{
}

CCollider::~CCollider()
{
	auto iter = m_listCollision.begin();
	auto iterEnd = m_listCollision.end();

	while (iter != iterEnd)
	{
		(*iter)->DeleteCollisionList(this);

		++iter;
	}

}

bool CCollider::Init()
{
	SetCollisionProfile(ECollisionChannel::Default);


	return true;
}

float CCollider::GetBottom() const
{
	return m_Bottom;
}

void CCollider::SetCheckMouseCollision(bool On)
{
	m_CheckMouseCollision = On;
}

bool CCollider::GetCheckMouseCollision() const
{
	return m_CheckMouseCollision;
}







std::list<CSharedPtr<CCollider>>* CCollider::GetCollisionList()
{
	return &m_listCollision;
}

bool CCollider::CheckColliding(CGameObject* Obj)
{
	if (m_listCollision.empty())
		return false;

	auto iter = m_listCollision.begin();
	auto iterEnd = m_listCollision.end();
	while (iter != iterEnd)
	{
		if ((*iter)->GetOwnerObj() == Obj)
			return true;

		++iter;
	}

	return false;
}

bool CCollider::CheckCollidingAny()
{
	if (m_listCollision.empty())
		return false;

	return true;
}

bool CCollider::CheckMouseColliding() const
{
	return m_isMouseColliding;
}

void CCollider::SetMouseColliding(bool isMouseColliding)
{
	m_isMouseColliding = isMouseColliding;
}

bool CCollider::CollisionPoint(const Vector2& Point)
{
	return false;
}

void CCollider::CallCollisionBeginMouse(const Vector2& Point)
{
	if (m_CollisionBeginMouse)
		m_CollisionBeginMouse(Point, this);
}

void CCollider::CallCollisionEndMouse(const Vector2& Point)
{
	if (m_CollisionEndMouse)
		m_CollisionEndMouse(Point, this);
}

void CCollider::SetFixedPos(const Vector2& Pos)
{
	m_isFixed = true;
	m_FixedPos = Pos;
}

void CCollider::SetOffset(const Vector2& Offset)
{
	m_Offset = Offset;
}

void CCollider::SetOffset(float _x, float _y)
{
	m_Offset.Set(_x, _y);
}

const Vector2& CCollider::GetPos() const
{
	return m_FixedPos;
}



const Vector2& CCollider::GetHitPoint() const
{
	return m_HitPoint;
}

void CCollider::CallCollisionBegin(CCollider* Dest)
{
	//만약 함수 포인터가 등록되어 있다면 자신을 Src, 상대를 Dest에 넣어서 호출한다.
	if(m_CollisionBegin)
		m_CollisionBegin(this, Dest);

}

void CCollider::CallCollisionEnd(CCollider* Dest)
{
	if (m_CollisionEnd)
		m_CollisionEnd(this, Dest);
}

CGameObject* CCollider::GetOwnerObj() const
{
	return m_OwnerObj;
}

CScene* CCollider::GetOwnerScene() const
{
	return m_Scene;
}

EColliderType CCollider::GetColliderType() const
{
	return m_ColliderType;
}

const ECollisionChannel CCollider::GetCollisionProfile() const
{
	return m_Profile;
}

void CCollider::SetCollisionProfile(ECollisionChannel Profile)
{
	m_Profile = Profile;
}



void CCollider::AddCollisionList(CCollider* col)
{
	m_listCollision.push_back(col);
}

bool CCollider::CheckCollisionList(CCollider* col)
{
	auto iter = m_listCollision.begin();
	auto iterEnd = m_listCollision.end();

	while (iter != iterEnd)
	{
		if ((*iter) == col)
			return true;

		++iter;
	}

	return false;
}

void CCollider::DeleteCollisionList(CCollider* col)
{
	auto iter = m_listCollision.begin();
	auto iterEnd = m_listCollision.end();

	while (iter != iterEnd)
	{
		if ((*iter) == col)
		{
			m_listCollision.erase(iter);

			
			return;
		}

		++iter;
	}

}

void CCollider::ClearCollisionList()
{
	auto iter = m_listCollision.begin();
	auto iterEnd = m_listCollision.end();

	while (iter != iterEnd)
	{
		//서로의 충돌 종료 함수를 호출
		CallCollisionEnd(*iter);
		(*iter)->CallCollisionEnd(this);
		(*iter)->DeleteCollisionList(this);

		++iter;
	}

	m_Scene->GetSceneCollision()->DeleteCollider(this);
	m_listCollision.clear();
}

void CCollider::Start()
{
	m_Start = true;
}
