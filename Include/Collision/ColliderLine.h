#pragma once
#include "Collider.h"
class CColliderLine :
	public CCollider
{
	friend class CGameObject;
	friend class CCollisionManager;

protected:
	CColliderLine();
	CColliderLine(const CColliderLine& col);
	virtual ~CColliderLine();

protected:
	LineInfo m_Info;
	bool m_isFixedEnd;
	Vector2 m_FixedEnd;
	Vector2 m_Dir;
	float m_Distance;


public:
	//이 함수를 통해 Start Point를 지정
	virtual void SetFixedPos(const Vector2& Start);

	//두 가지 함수 중 하나를 통해 End Point를 지정
	void SetFixedEnd(const Vector2& End);   //고정 End지점을 설정
	void SetDir(const Vector2& Dir, float Distance);    //유동 End지점을 설정.
	void SetDir(float Degree, float Distance);
	void SetDistance(float Distance);

	virtual void SetOffset(const Vector2& Offset);
	virtual void SetOffset(float _x, float _y);

	virtual bool Collision(CCollider* Dest);
	virtual bool CollisionPoint(const Vector2& Point);


	LineInfo GetInfo() const;



	//레이저용 함수
	float GetLineInfoEndY() const;


protected:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);
};

