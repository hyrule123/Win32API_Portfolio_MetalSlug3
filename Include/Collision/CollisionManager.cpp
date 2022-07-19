#include "CollisionManager.h"

//�ڽ��浹 üũ�� �ʿ�
#include "ColliderBox.h"
#include "ColliderCircle.h"
#include "ColliderLine.h"

DEFINITION_SINGLETON(CCollisionManager)

CCollisionManager::CCollisionManager()
{

}


CCollisionManager::~CCollisionManager()
{

	
	for (int i = 0; i < (int)ECollisionChannel::Max; ++i)
	{
		SAFE_DELETE(m_vecProfile[i]);
	}
}

bool CCollisionManager::Init()
{
	//�浹 ���̾� ��ŭ �������� ����.
	int size = (int)ECollisionChannel::Max;
	for (int i = 0; i < size; ++i)
	{
		m_vecProfile[i] = new CollisionProfile;
	}

	//Default ���� -> ��ο� �浹
	SetProfileAll(ECollisionChannel::Default);

	//ȭ�� ���ʵ� �浹ü�� �����Ѵ�.
	SetProfile(ECollisionChannel::BackAlly, ECollisionChannel::BackEnemy);
	SetProfile(ECollisionChannel::BackEnemy, ECollisionChannel::BackAlly);

	//Player ���� -> Default, Enemy, EnemyAttack�� �浹
	SetProfile(ECollisionChannel::Player, ECollisionChannel::Default);
	SetProfile(ECollisionChannel::Player, ECollisionChannel::Enemy);
	SetProfile(ECollisionChannel::Player, ECollisionChannel::EnemyAttack);

	//Enemy ���� -> Default, Player, PlayerAttack�� �浹
	SetProfile(ECollisionChannel::Enemy, ECollisionChannel::Default);
	SetProfile(ECollisionChannel::Enemy, ECollisionChannel::Player);
	SetProfile(ECollisionChannel::Enemy, ECollisionChannel::PlayerAttack);
	SetProfile(ECollisionChannel::Enemy, ECollisionChannel::Mouse);


	//PlayerAttack ���� -> Default, Enemy�� �浹
	SetProfile(ECollisionChannel::PlayerAttack, ECollisionChannel::Default);
	SetProfile(ECollisionChannel::PlayerAttack, ECollisionChannel::Enemy);

	//EnemyAttack ���� -> Default, Player�� �浹
	SetProfile(ECollisionChannel::EnemyAttack, ECollisionChannel::Default);
	SetProfile(ECollisionChannel::EnemyAttack, ECollisionChannel::Player);


	//Mouse ���� -> �ϴ� Default�� Enemy���� �浹
	SetProfileAll(ECollisionChannel::Mouse);

	return true;
}

bool CCollisionManager::CollisionBoxToBox(CColliderBox* Src, CColliderBox* Dest)
{

		const BoxInfo& SrcInfo = Src->GetInfo();
		const BoxInfo& DestInfo = Dest->GetInfo();


		//�ϴ� �浹�� �� �Ͼ�� ��Ȳ�� ���� �ɷ��ָ�
		if (SrcInfo.LT.x > DestInfo.RB.x)
			return false;
		else if (SrcInfo.RB.x < DestInfo.LT.x)
			return false;
		else if (SrcInfo.LT.y > DestInfo.RB.y)
			return false;
		else if (SrcInfo.RB.y < DestInfo.LT.y)
			return false;



		//�浹�� �Ͼ�� ��Ȳ�̶�� ���� �ȴ�.
		//�浹�� �Ͼ�� ��Ȳ�̶��켱 ��Ʈ����Ʈ�� ����ؾ� �Ѵ�.
		float Left = SrcInfo.LT.x > DestInfo.LT.x ? SrcInfo.LT.x : DestInfo.LT.x;
		float Top = SrcInfo.LT.y > DestInfo.LT.y ? SrcInfo.LT.y : DestInfo.LT.y;
		float Right = SrcInfo.RB.x < DestInfo.RB.x ? SrcInfo.RB.x : DestInfo.RB.x;
		float Bottom = SrcInfo.RB.y < DestInfo.RB.y ? SrcInfo.RB.y : DestInfo.RB.y;


		Vector2 HitPoint;
		HitPoint.x = (Right + Left) / 2.f;
		HitPoint.y = (Bottom + Top) / 2.f;


		Src->m_HitPoint = HitPoint;
		Dest->m_HitPoint = HitPoint;

		return true;

}

bool CCollisionManager::CollisionCircleToCircle(CColliderCircle* Src, CColliderCircle* Dest)
{

	const CircleInfo& SrcInfo = Src->GetInfo();
	const CircleInfo& DestInfo = Dest->GetInfo();
	float Distance = SrcInfo.Center.Distance(DestInfo.Center);

	//���� �� �������� ���̸� ���� �ͺ��� �� �� ������ �Ÿ��� �ִٸ� �浹 �ƴ�
	if (Distance > (SrcInfo.Radius + DestInfo.Radius))
		return false;

	Vector2 HitPoint = (SrcInfo.Center + DestInfo.Center) / 2.f;

	Src->m_HitPoint = HitPoint;
	Dest->m_HitPoint = HitPoint;

	return true;
}


bool CCollisionManager::CollisionBoxToCircle(CColliderBox* Src, CColliderCircle* Dest)
{
	Vector2 HitPoint;

	//�浹�� Ȯ�ε����� ��Ʈ����Ʈ �����ϰ� true ��ȯ
	if (CollisionBoxCircle(Src->GetInfo(), Dest->GetInfo(), HitPoint))
	{
		Src->m_HitPoint = HitPoint;
		Dest->m_HitPoint = HitPoint;

		return true;
	}

	return false;
}

bool CCollisionManager::CollisionCircleToBox(CColliderCircle* Src, CColliderBox* Dest)
{
	Vector2 HitPoint;

	//�浹�� Ȯ�ε����� ��Ʈ����Ʈ �����ϰ� true ��ȯ
	if (CollisionBoxCircle(Dest->GetInfo(), Src->GetInfo(), HitPoint))
	{
		Src->m_HitPoint = HitPoint;
		Dest->m_HitPoint = HitPoint;

		return true;
	}

	return false;
}

bool CCollisionManager::CollisionLineToCircle(CColliderLine* Src, CColliderCircle* Dest)
{
	Vector2 HitPointNear;
	Vector2 HitPointFar;

	//�浹�� Ȯ�ε����� ��Ʈ����Ʈ �����ϰ� true ��ȯ
	if (CollisionLineCircle(Src->GetInfo(), Dest->GetInfo(), HitPointNear, HitPointFar))
	{
		Src->m_HitPoint = HitPointNear;
		Dest->m_HitPoint = HitPointNear;

		return true;
	}

	return false;
}

bool CCollisionManager::CollisionCircleToLine(CColliderCircle* Src, CColliderLine* Dest)
{
	Vector2 HitPoint;

	Vector2 HitPointNear;
	Vector2 HitPointFar;

	//�浹�� Ȯ�ε����� ��Ʈ����Ʈ �����ϰ� true ��ȯ
	if (CollisionLineCircle(Dest->GetInfo(), Src->GetInfo(), HitPointNear, HitPointFar))
	{
		Src->m_HitPoint = HitPointNear;
		Dest->m_HitPoint = HitPointNear;

		return true;
	}

	return false;
}

bool CCollisionManager::CollisionLineToBox(CColliderLine* Src, CColliderBox* Dest)
{
	Vector2 HitPointNear;
	Vector2 HitPointFar;

	//�浹�� Ȯ�ε����� ��Ʈ����Ʈ �����ϰ� true ��ȯ
	if (CollisionLineBox(Src->GetInfo(), Dest->GetInfo(), HitPointNear, HitPointFar))
	{
		Src->m_HitPoint = HitPointNear;
		Dest->m_HitPoint = HitPointNear;

		return true;
	}

	return false;
}

bool CCollisionManager::CollisionBoxToLine(CColliderBox* Src, CColliderLine* Dest)
{
	Vector2 HitPointNear;
	Vector2 HitPointFar;

	//�浹�� Ȯ�ε����� ��Ʈ����Ʈ �����ϰ� true ��ȯ
	if (CollisionLineBox(Dest->GetInfo(), Src->GetInfo(), HitPointNear, HitPointFar))
	{
		Src->m_HitPoint = HitPointNear;
		Dest->m_HitPoint = HitPointNear;

		return true;
	}

	return false;
}

bool CCollisionManager::CollisionLineToLine(CColliderLine* Src, CColliderLine* Dest)
{
	Vector2 HitPoint;

	//�浹�� Ȯ�ε����� ��Ʈ����Ʈ �����ϰ� true ��ȯ
	if (CollisionLineLine(Dest->GetInfo(), Src->GetInfo(), HitPoint))
	{
		Src->m_HitPoint = HitPoint;
		Dest->m_HitPoint = HitPoint;

		return true;
	}

	return false;
}


//Dest�� Src�� ó���ؼ� ���ο��� ����� �޼ҵ�
bool CCollisionManager::CollisionBoxCircle(const BoxInfo& Box, const CircleInfo& Circle, Vector2& HitPoint)
{



	//�ڽ��� �� ������ ���� ��
	if((Circle.Center.x >= Box.LT.x && Circle.Center.x <= Box.RB.x) ||
		(Circle.Center.y >= Box.LT.y && Circle.Center.y <= Box.RB.y))
	{

		BoxInfo ExtendedBox;
		ExtendedBox.LT = Box.LT - Circle.Radius;
		ExtendedBox.RB = Box.RB + Circle.Radius;


		if (ExtendedBox.LT.x > Circle.Center.x)
			return false;

		else if (ExtendedBox.LT.y > Circle.Center.y)
			return false;

		else if (ExtendedBox.RB.x < Circle.Center.x)
			return false;

		else if (ExtendedBox.RB.y < Circle.Center.y)
			return false;



		HitPoint = (Circle.Center + ((Box.LT + Box.RB) / 2.f)) / 2.f;


		return true;


	}

	//�ڽ��� �� ������ ���� ������ �ʴٸ� 
	else
	{

		//�밢���� Ȯ���Ѵ�.
		//�簢���� �� �������� ���� �߽����� �Ÿ���
		//���������� ª�ٸ� �浹�̴�.

		Vector2 Vertex[4] = {
			Box.LT,
			Box.RB,
			Vector2(Box.LT.x, Box.RB.y),
			Vector2(Box.RB.x, Box.LT.y)
		};

		//�ϳ��� �ɸ��� True �̹Ƿ� �ٷ� ����ϰ� ���������� ���������� ������ �� ��.
		for (int i = 0; i < 4; ++i)
		{
			if(Circle.Radius > Circle.Center.Distance(Vertex[i]))
			{
				HitPoint = (Circle.Center + ((Box.LT + Box.RB) / 2.f)) / 2.f;

				return true;
			}
		}

	}

	return false;
}

bool CCollisionManager::CollisionLineBox(
	const LineInfo& Line, const BoxInfo& Box, 
	Vector2& HitPointNear, Vector2& HitPointFar)
{
	LineInfo RectSide;
	Vector2 HitPoint[2];
	UINT8 Count = 0;

	//���� ���� ���������� �簢�� �ȿ� �ִ��� Ȯ���Ѵ�. ���� �ȿ� ������ true �� ��Ʈ����Ʈ ��ȯ
	if (Line.Start.x > Box.LT.x && Line.Start.x < Box.RB.x && Line.Start.y > Box.LT.y && Line.Start.y < Box.RB.y)
	{
		HitPoint[Count] = Line.Start;
		++Count;
	}

	//Left
	RectSide.Start.y = Box.LT.y;
	RectSide.End.y = Box.RB.y;
	RectSide.Start.x = Box.LT.x;
	RectSide.End.x = Box.LT.x;
	bool Left = CollisionLineLine(Line, RectSide, HitPoint[Count]);
	if (Left)
		++Count;

	//Top
	RectSide.Start.x = Box.LT.x;
	RectSide.End.x = Box.RB.x;
	RectSide.Start.y = Box.LT.y;
	RectSide.End.y = Box.LT.y;
	bool Top = CollisionLineLine(Line, RectSide, HitPoint[Count]);
	if (Top)
	{
		++Count;
		if (Count >= 2)
		{
			ComputeDistance(Line.Start, HitPoint[0], HitPoint[1]);
			HitPointNear = HitPoint[0];
			HitPointFar = HitPoint[1];
			return true;
		}
			
	}

	//Right
	RectSide.Start.y = Box.LT.y;
	RectSide.End.y = Box.RB.y;
	RectSide.Start.x = Box.RB.x;
	RectSide.End.x = Box.RB.x;
	bool Right = CollisionLineLine(Line, RectSide, HitPoint[Count]);
	if (Right)
	{
		++Count;
		if (Count >= 2)
		{
			ComputeDistance(Line.Start, HitPoint[0], HitPoint[1]);
			HitPointNear = HitPoint[0];
			HitPointFar = HitPoint[1];
			return true;
		}
	}

	//Bottom
	RectSide.Start.x = Box.LT.x;
	RectSide.End.x = Box.RB.x;
	RectSide.Start.y = Box.RB.y;
	RectSide.End.y = Box.RB.y;
	bool Bottom = CollisionLineLine(Line, RectSide, HitPoint[Count]);
	if (Bottom)
	{
		++Count;
		if (Count >= 2)
		{
			ComputeDistance(Line.Start, HitPoint[0], HitPoint[1]);
			HitPointNear = HitPoint[0];
			HitPointFar = HitPoint[1];
			return true;
		}
	}

	//�� ���� �浹���� ��
	if (Count == 1)
	{
		HitPointNear = HitPoint[0];
		return true;
	}

	return false;
}

bool CCollisionManager::CollisionLineCircle(
	const LineInfo& Line, const CircleInfo& Circle, 
	Vector2& HitPointNear,Vector2& HitPointFar)
{

	if (!(fabs(PointLineDistance(Circle.Center, Line.Start, Line.End)) < Circle.Radius))
		return false;
	//else if (Circle.Center.Distance(Line.Start) < Circle.Radius)
	//{
	//	HitPointNear = Line.Start;
	//	return true;
	//}


	float cx = Circle.Center.x;
	float cy = Circle.Center.y;
	float radius = Circle.Radius;

	float dx, dy, A, B, C, det, t;

	dx = Line.End.x - Line.Start.x;
	dy = Line.End.y - Line.Start.y;
	

	A = dx * dx + dy * dy;
	B = 2 * (dx * (Line.Start.x - cx) + dy * (Line.Start.y - cy));
	C = (Line.Start.x - cx) * (Line.Start.x - cx) + (Line.Start.y - cy) * (Line.Start.y - cy) - radius * radius;

	det = B * B - 4 * A * C;
	if ((A <= 0.0000001) || (det < 0))
	{
		return false;
	}
	else if (det == 0)
	{
		// One solution.
		t = -B / (2 * A);
		HitPointNear.Set(Line.Start.x + t * dx, Line.Start.x + t * dy);
		return true;
	}
	else
	{
		// Two solutions.
		t = (float)((-B + sqrtf(det)) / (2 * A));
		HitPointNear.Set(Line.Start.x + t * dx, Line.Start.y + t * dy);
		t = (float)((-B - sqrtf(det)) / (2 * A));
		HitPointFar.Set(Line.Start.x + t * dx, Line.Start.y + t * dy);

		ComputeDistance(Line.Start, HitPointNear, HitPointFar);

		return true;
	}


	return false;
}

bool CCollisionManager::CollisionLineLine(const LineInfo& SrcLine, const LineInfo& DestLine, Vector2& HitPoint)
{
	float x1 = SrcLine.Start.x;
	float y1 = SrcLine.Start.y;
	float x2 = SrcLine.End.x;
	float y2 = SrcLine.End.y;
	float x3 = DestLine.Start.x;
	float y3 = DestLine.Start.y;
	float x4 = DestLine.End.x;
	float y4 = DestLine.End.y;

	// calculate the direction of the lines
	float uA = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));
	float uB = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / ((y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1));

	if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1)
	{
		// optionally, draw a circle where the lines meet
		HitPoint.x = x1 + (uA * (x2 - x1));
		HitPoint.y = y1 + (uA * (y2 - y1));

		return true;
	}

	return false;
}

float CCollisionManager::PointLineDistance(const Vector2& Point, const Vector2& LineStart, const Vector2& LineEnd)
{
	float LineY = LineEnd.y - LineStart.y;
	float LineX = LineStart.x - LineEnd.x;

	return (LineY * Point.x + LineX * Point.y + (LineStart.y * LineEnd.x - LineStart.x * LineEnd.y)) / sqrtf(LineX * LineX + LineY * LineY);
}

float CCollisionManager::PointLineSignCheck(const Vector2& Point, const Vector2& LineStart, const Vector2& LineEnd)
{
	float LineY = LineEnd.y - LineStart.y;
	float LineX = LineStart.x - LineEnd.x;

	return (LineY * Point.x + LineX * Point.y + (LineStart.y * LineEnd.x - LineStart.x * LineEnd.y));
}

void CCollisionManager::ComputeDistance(const Vector2& Start, Vector2& Near, Vector2& Far)
{
	float DistNear = Start.Distance(Near);
	float DestFar = Start.Distance(Far);

	if (DistNear > DestFar)
	{
		Vector2 Temp(Near);
		Near = Far;
		Far = Temp;
	}
}

bool CCollisionManager::CollisionPointToCircle(const Vector2& Point, CColliderCircle* Circle)
{
	if (Point.Distance(Circle->GetInfo().Center) > Circle->GetInfo().Radius)
		return false;

	Circle->m_HitPoint = Point;

	return true;
}

bool CCollisionManager::CollisionPointToBox(const Vector2& Point, CColliderBox* Box)
{
	const BoxInfo& DestInfo = Box->GetInfo();


	//�ϴ� �浹�� �� �Ͼ�� ��Ȳ�� ���� �ɷ��ָ�
	if (Point.x > DestInfo.RB.x)
		return false;
	else if (Point.x < DestInfo.LT.x)
		return false;
	else if (Point.y > DestInfo.RB.y)
		return false;
	else if (Point.y < DestInfo.LT.y)
		return false;


	//�浹�� �ȴ�. HitPoint�� ���ؼ� �Է����� �� true�� ��ȯ
	Box->m_HitPoint = Point;


	return true;
}






CollisionProfile* CCollisionManager::GetCollisionProfile(ECollisionChannel Profile)
{
	return m_vecProfile[(int)Profile];
}

void CCollisionManager::SetProfile(ECollisionChannel Profile, ECollisionChannel Channel, bool isCollide)
{
	m_vecProfile[(int)Profile]->Interaction[(int)Channel] = isCollide;
}

void CCollisionManager::SetProfileAll(ECollisionChannel Profile, bool isCollide)
{
	for (int i = 0; i < (int)ECollisionChannel::Max; ++i)
	{
		m_vecProfile[(int)Profile]->Interaction[i] = isCollide;
	}

}
