#pragma once

#include "../GameInfo.h"
#include "../Singleton.h"

class CCollisionManager
{

private:
	//�� ä�κ��� �ϳ��� �������� ����. �ʱ�ȭ �� �� ���������� ����
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

	//��-��
	bool CollisionLineToCircle(class CColliderLine* Src, class CColliderCircle* Dest);
	bool CollisionCircleToLine(class CColliderCircle* Src, class CColliderLine* Dest);

	//��-�簢��
	bool CollisionLineToBox(class CColliderLine* Src, class CColliderBox* Dest);
	bool CollisionBoxToLine(class CColliderBox* Src, class CColliderLine* Dest);

	//��-��
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

	//��ȣ�� üũ��.
	float PointLineSignCheck(const Vector2& Point, const Vector2& LineStart, const Vector2& LineEnd);

	//�Ÿ��� Ȯ���Ͽ� ����� �Ÿ��� �� �Ÿ��� ���͸� ����
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

	//Profile�� ����ϴ� �浹ü�� Channel�� � ��ȣ�ۿ��� �Ұ����� ����
	//(�⺻��: true)
	void SetProfile(ECollisionChannel Profile, 
		ECollisionChannel Channel, 
		bool isCollide = true);

	//Profile�� ����ϴ� �浹ü�� ��� ��ȣ�ۿ��� ���ڷ� ���� Interaction���� �����Ѵ�.
	//(�⺻��: true)
	void SetProfileAll(ECollisionChannel Profile, 
		bool isCollide = true);




	DECLARE_SINGLETON(CCollisionManager)
};

