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
		//목표값과의 밑변과 높이를 구한다.
		float width = Vec.x - x;
		float height = Vec.y - y;

		//피타고라스 정리를 통해 빗변의 길이를 구한다.
		float c = sqrt(width * width + height * height);

		//구한 길이로 코사인값을 구한다.
		float theta = acosf(width / c);

		//만약 구해야 하는 각도가 180 degree(PI) 를 넘어갈 시 360 degree(PI * 2)에서 빼준다.
		if (height < 0.f)
			theta = 2.f * PI - theta;

		return theta;
	}

	float FastAngle(const Vector2& Vec)
	{
		//목표값과의 밑변과 높이를 구한다.
		float width = Vec.x - x;
		float height = Vec.y - y;

		//피타고라스 정리를 통해 빗변의 길이를 구한다.
		float c = width * width + height * height;

		c = [](float n)->float
		{
			static union { int i; float f; } u;
			u.i = 0x5F375A86 - (*(int*)&n >> 1);
			return ((int(3) - n * u.f * u.f) * n * u.f * 0.5f);
		}(c);
		return c;

		//구한 길이로 코사인값을 구한다.
		float theta = acosf(width / c);

		//만약 구해야 하는 각도가 180 degree(PI) 를 넘어갈 시 360 degree(PI * 2)에서 빼준다.
		if (height < 0.f)
			theta = 2.f * PI - theta;

		return theta;
	}

	//벡터의 크기 혹은 거리
	float Distance(const Vector2& Dest)	const
	{
		float Width = x - Dest.x;
		float Height = y - Dest.y;

		float Result = Width * Width + Height * Height;

		Result = sqrtf(Result);

		return Result;
	}


	//벡터의 길이를 반환한다.
	float Norm() const 
	{ 
		return hypotf(x, y); 
	}

	//방향이 같은 단위 벡터를 반환한다.
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