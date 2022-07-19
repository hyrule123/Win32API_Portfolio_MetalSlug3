#include "GameInfo.h"

//180.f degree = PI radian

float DegreeToRadian(float Degree)
{
	return Degree * PI / 180.f;
}

float RadianToDegree(float Radian)
{
	return Radian * 180.f / PI;
}

float FastSqrt(const float& n)
{
	static union { int i; float f; } u;
	u.i = 0x5F375A86 - (*(int*)&n >> 1);
	return (int(3) - n * u.f * u.f) * n * u.f * 0.5f;
}
