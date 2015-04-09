#pragma once

#include <iostream>
#include <math.h>

class Vec3
{
public:
	Vec3()
	{

	}

	Vec3(float x, float y, float z) : x(x), y(y), z(z)
	{

	}
	
	float x,y,z;

	float getLength()
	{
		Vec3 temp;
		temp.x = pow(x, 2);
		temp.y = pow(y, 2);
		temp.z = pow(z, 2);
		float total = temp.x + temp.y + temp.z;

		return sqrt(total);
	}

	Vec3 normalize()
	{
		float length = getLength();
		return Vec3(x / length, y / length, z / length);
	}

	Vec3* operator += (Vec3 b)
	{
		x += b.x;
		y += b.y;
		z += b.z;
		return this;
	}
	Vec3* operator -= (Vec3 b)
	{
		x -= b.x;
		y -= b.y;
		z -= b.z;
		return this;
	}

};

inline Vec3 operator + (Vec3 a, Vec3 b)
{
	Vec3 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}

inline Vec3 operator - (Vec3 a, Vec3 b)
{
	Vec3 result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
	return result;
}

inline Vec3 operator * (Vec3 a, Vec3 b)
{
	Vec3 result;
	result.x = a.x * b.x;
	result.y = a.y * b.y;
	result.z = a.z * b.z;
	return result;
}

inline Vec3 operator / (Vec3 a, Vec3 b)
{
	Vec3 result;
	result.x = a.x / b.x;
	result.y = a.y / b.y;
	result.z = a.z / b.z;
	return result;
}


inline std::ostream& operator << (std::ostream& os, const Vec3& a)
{
	os << "Vec3(" << a.x << ", " << a.y << ", " << a.z << ") ";
	return os;
}
inline bool operator == (const Vec3& a, const Vec3& b)
{
	return (a.x == b.x && a.y == b.y && a.z == b.z) ? true : false;
}
inline bool operator != (const Vec3& a, const Vec3& b)
{
	return (a.x != b.x || a.y != b.y || a.z != b.z) ? true : false;
}