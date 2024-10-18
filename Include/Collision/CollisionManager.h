#pragma once

#include "../GameInfo.h"
#include "../Singleton.h"

class CCollisionManager
{

private:
	//각 채널별로 하나의 프로파일 보유. 초기화 때 각 프로파일을 설정
	CollisionProfile* m_vecProfile[(int)ECollisionChannel::Max];

public:
	bool Init();


public:
	bool CollisionBoxToBox(
		class CColliderBox* Src, 
		class CColliderBox* Dest);

	bool CollisionCircleToCircle(
		class CColliderCircle* Src,
		class CColliderCircle* Dest
	);

	bool CollisionBoxToCircle(
		class CColliderBox* Src,
		class CColliderCircle* Dest
	);

	bool CollisionCircleToBox(
		class CColliderCircle* Src,
		class CColliderBox* Dest
	);

	//선-원
	bool CollisionLineToCircle(class CColliderLine* Src, class CColliderCircle* Dest);
	bool CollisionCircleToLine(class CColliderCircle* Src, class CColliderLine* Dest);

	//선-사각형
	bool CollisionLineToBox(class CColliderLine* Src, class CColliderBox* Dest);
	bool CollisionBoxToLine(class CColliderBox* Src, class CColliderLine* Dest);

	//선-선
	bool CollisionLineToLine(class CColliderLine* Src, class CColliderLine* Dest);


protected:
	bool CollisionBoxCircle(
		const BoxInfo& Box,
		const CircleInfo& Circle,
		Vector2& HitPoint
	);

	bool CollisionLineBox(
		const LineInfo& Line, 
		const BoxInfo& Box, 
		Vector2& HitPointNear,
		Vector2& HitPointFar
	);

	bool CollisionLineCircle(
		const LineInfo& Line,
		const CircleInfo& Circle,
		Vector2& HitPointNear,
		Vector2& HitPointFar
	);

	bool CollisionLineLine(
		const LineInfo& SrcLine,
		const LineInfo& DestLine,
		Vector2& HitPoint
	);

private:
	float PointLineDistance(const Vector2& Point, const Vector2& LineStart, const Vector2& LineEnd);

	//부호만 체크함.
	float PointLineSignCheck(const Vector2& Point, const Vector2& LineStart, const Vector2& LineEnd);

	//거리를 확인하여 가까운 거리와 먼 거리의 벡터를 삽입
	void ComputeDistance(const Vector2& Start, Vector2& Near, Vector2& Far);

public:
	bool CollisionPointToCircle(
		const Vector2& Point,
		class CColliderCircle* Circle
	);

	bool CollisionPointToBox(
		const Vector2& Point,
		class CColliderBox* Box
	);


public:
	CollisionProfile* GetCollisionProfile(ECollisionChannel Channel);

	//Profile을 사용하는 충돌체가 Channel과 어떤 상호작용을 할것인지 설정
	//(기본값: true)
	void SetProfile(ECollisionChannel Profile, 
		ECollisionChannel Channel, 
		bool isCollide = true);

	//Profile을 사용하는 충돌체의 모든 상호작용을 인자로 들어온 Interaction으로 설정한다.
	//(기본값: true)
	void SetProfileAll(ECollisionChannel Profile, 
		bool isCollide = true);




	DECLARE_SINGLETON(CCollisionManager)
};

