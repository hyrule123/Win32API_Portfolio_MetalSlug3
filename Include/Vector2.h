//Vector2.h


#pragma once

const float PI = 3.1415f;

struct Vector2
{
	float x;
	float y;

	Vector2() :
		x(0.f),
		y(0.f)
	{}

	Vector2(float _x, float _y) :
		x(_x),
		y(_y)
	{}

	Vector2(int _x, int _y) :
		x((float)_x),
		y((float)_y)
	{}

	Vector2(const Vector2& v) :
		x(v.x),
		y(v.y)
	{}

	void Set(float _x, float _y)
	{
		x = _x;
		y = _y;
	}

	void operator = (float f)
	{
		x = f;
		y = f;
	}

	void operator = (const Vector2& v)
	{
		x = v.x;
		y = v.y;
	}

	bool operator == (const Vector2& v)	const
	{
		return (x == v.x && y == v.y);
	}

	bool operator != (const Vector2& v)	const
	{
		return (x != v.x || y != v.y);
	}

	bool operator == (float f)	const
	{
		return (x == f && y == f);
	}

	bool operator != (float f) const
	{
		return (x != f || y != f);
	}


		//<=, >=
	bool operator <= (const Vector2& Vec)
	{
		bool a = x <= Vec.x ? true : false;
		bool b = y <= Vec.y ? true : false;
		return (a && b);
	}
	bool operator >= (const Vector2& Vec)
	{
		bool a = x >= Vec.x ? true : false;
		bool b = y >= Vec.y ? true : false;

		return (a && b);
	}

	bool operator <= (float f)
	{
		bool a = x <= f ? true : false;
		bool b = y <= f ? true : false;
		return (a && b);
	}
	bool operator >= (float f)
	{
		bool a = x >= f ? true : false;
		bool b = y >= f ? true : false;

		return (a && b);
	}




	//+
	Vector2 operator + (const Vector2& v)	const
	{
		Vector2 Result;
		Result.x = x + v.x;
		Result.y = y + v.y;
		return Result;
	}
	Vector2 operator + (float f)	const
	{
		Vector2 Result;
		Result.x = x + f;
		Result.y = y + f;
		return Result;
	}
	Vector2 operator + (int i)	const
	{
		Vector2 Result;
		Result.x = x + (float)i;
		Result.y = y + (float)i;
		return Result;
	}
	void operator += (const Vector2& v)
	{
		x += v.x;
		y += v.y;
	}
	void operator += (float f)
	{
		x += f;
		y += f;
	}
	void operator += (int i)
	{
		x += (float)i;
		y += (float)i;
	}




	//-
	Vector2 operator - (const Vector2& v)	const
	{
		Vector2 Result;
		Result.x = x - v.x;
		Result.y = y - v.y;
		return Result;
	}
	Vector2 operator - (float f)	const
	{
		Vector2 Result;
		Result.x = x - f;
		Result.y = y - f;
		return Result;
	}
	Vector2 operator - (int i)	const
	{
		Vector2 Result;
		Result.x = x - (float)i;
		Result.y = y - (float)i;
		return Result;
	}
	void operator -= (const Vector2& v)
	{
		x -= v.x;
		y -= v.y;
	}
	void operator -= (float f)
	{
		x -= f;
		y -= f;
	}
	void operator -= (int i)
	{
		x -= (float)i;
		y -= (float)i;
	}





	//*
	Vector2 operator * (const Vector2& v)	const
	{
		Vector2 Result;
		Result.x = x * v.x;
		Result.y = y * v.y;
		return Result;
	}
	Vector2 operator * (float f)	const
	{
		Vector2 Result;
		Result.x = x * f;
		Result.y = y * f;
		return Result;
	}
	Vector2 operator * (int i)	const
	{
		Vector2 Result;
		Result.x = x * (float)i;
		Result.y = y * (float)i;
		return Result;
	}
	void operator *= (const Vector2& v)
	{
		x *= v.x;
		y *= v.y;
	}
	void operator *= (float f)
	{
		x *= f;
		y *= f;
	}
	void operator *= (int i)
	{
		x *= (float)i;
		y *= (float)i;
	}



	///
	Vector2 operator / (const Vector2& v)	const
	{
		Vector2 Result;
		Result.x = x / v.x;
		Result.y = y / v.y;
		return Result;
	}
	Vector2 operator / (float f)	const
	{
		Vector2 Result;
		Result.x = x / f;
		Result.y = y / f;
		return Result;
	}
	Vector2 operator / (int i)	const
	{
		Vector2 Result;
		Result.x = x / (float)i;
		Result.y = y / (float)i;
		return Result;
	}
	void operator /= (const Vector2& v)
	{
		x /= v.x;
		y /= v.y;
	}
	void operator /= (float f)
	{
		x /= f;
		y /= f;
	}
	void operator /= (int i)
	{
		x /= (float)i;
		y /= (float)i;
	}


	Vector2 Round()
	{
		x = roundf(x);
		y = roundf(y);

		return *this;
	}



	float Angle(const Vector2& Vec)
	{
		//��ǥ������ �غ��� ���̸� ���Ѵ�.
		float width = Vec.x - x;
		float height = Vec.y - y;

		//��Ÿ��� ������ ���� ������ ���̸� ���Ѵ�.
		float c = sqrt(width * width + height * height);

		//���� ���̷� �ڻ��ΰ��� ���Ѵ�.
		float theta = acosf(width / c);

		//���� ���ؾ� �ϴ� ������ 180 degree(PI) �� �Ѿ �� 360 degree(PI * 2)���� ���ش�.
		if (height < 0.f)
			theta = 2.f * PI - theta;

		return theta;
	}

	float FastAngle(const Vector2& Vec)
	{
		//��ǥ������ �غ��� ���̸� ���Ѵ�.
		float width = Vec.x - x;
		float height = Vec.y - y;

		//��Ÿ��� ������ ���� ������ ���̸� ���Ѵ�.
		float c = width * width + height * height;

		c = [](float n)->float
		{
			static union { int i; float f; } u;
			u.i = 0x5F375A86 - (*(int*)&n >> 1);
			return ((int(3) - n * u.f * u.f) * n * u.f * 0.5f);
		}(c);
		return c;

		//���� ���̷� �ڻ��ΰ��� ���Ѵ�.
		float theta = acosf(width / c);

		//���� ���ؾ� �ϴ� ������ 180 degree(PI) �� �Ѿ �� 360 degree(PI * 2)���� ���ش�.
		if (height < 0.f)
			theta = 2.f * PI - theta;

		return theta;
	}

	//������ ũ�� Ȥ�� �Ÿ�
	float Distance(const Vector2& Dest)	const
	{
		float Width = x - Dest.x;
		float Height = y - Dest.y;

		float Result = Width * Width + Height * Height;

		Result = sqrtf(Result);

		return Result;
	}


	//������ ���̸� ��ȯ�Ѵ�.
	float Norm() const 
	{ 
		return hypotf(x, y); 
	}

	//������ ���� ���� ���͸� ��ȯ�Ѵ�.
	Vector2 Normalize() const 
	{
		if (x == 0.f && y == 0.f)
			return Vector2(0.f, 0.f);

		float norm = Norm();

		return Vector2(x / norm, y / norm);
	}


	float Dot(const Vector2& v)
	{
		return x * v.x + y * v.y;
	}

	float Cross(const Vector2& v)
	{
		return x * v.y - v.x * y;
	}

	Vector2 fabs()
	{
		return Vector2(::fabs(x), ::fabs(y));
	}
	
	float Sum()
	{
		return x + y;
	}

};