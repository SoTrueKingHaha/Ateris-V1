#pragma once
#include <math.h>

#define PI 3.14159265358979323846
#define M_RADPI 57.295779513082
#define DEG2RAD(x) ((float)(x) * (float)((float)(PI) / 180.0f))
#define RAD2DEG(x) ((float)(x) * (float)(180.0f / (float)(PI)))

class Vec3
{
public:
	float x, y, z;

public:
	Vec3(void)
	{
		x = y = z = 0.0f;
	}

	void Zero()
	{
		x = y = z = 0.f;
	}

	Vec3(float X, float Y, float Z)
	{
		x = X; y = Y; z = Z;
	}

	__forceinline void Clear() {
		x = y = z = 0.f;
	}

	Vec3(float *v)
	{
		x = v[0]; y = v[1]; z = v[2];
	}

	Vec3(const float *v)
	{
		x = v[0]; y = v[1]; z = v[2];
	}

	Vec3(const Vec3 &v)
	{
		x = v.x; y = v.y; z = v.z;
	}

	Vec3 &operator=(const Vec3 &v)
	{
		x = v.x; y = v.y; z = v.z; return *this;
	}

	float &operator[](int i)
	{
		return ((float *)this)[i];
	}

	float operator[](int i) const
	{
		return ((float *)this)[i];
	}

	Vec3 &operator+=(const Vec3 &v)
	{
		x += v.x; y += v.y; z += v.z; return *this;
	}

	Vec3 &operator-=(const Vec3 &v)
	{
		x -= v.x; y -= v.y; z -= v.z; return *this;
	}

	Vec3 &operator*=(const Vec3 &v)
	{
		x *= v.x; y *= v.y; z *= v.z; return *this;
	}

	Vec3 &operator/=(const Vec3 &v)
	{
		x /= v.x; y /= v.y; z /= v.z; return *this;
	}

	Vec3 &operator+=(float v)
	{
		x += v; y += v; z += v; return *this;
	}

	Vec3 &operator-=(float v)
	{
		x -= v; y -= v; z -= v; return *this;
	}

	Vec3 &operator*=(float v)
	{
		x *= v; y *= v; z *= v; return *this;
	}

	Vec3 &operator/=(float v)
	{
		x /= v; y /= v; z /= v; return *this;
	}

	Vec3 operator+(const Vec3 &v) const
	{
		return Vec3(x + v.x, y + v.y, z + v.z);
	}

	Vec3 operator-(const Vec3 &v) const
	{
		return Vec3(x - v.x, y - v.y, z - v.z);
	}

	Vec3 operator*(const Vec3 &v) const
	{
		return Vec3(x * v.x, y * v.y, z * v.z);
	}

	Vec3 operator/(const Vec3 &v) const
	{
		return Vec3(x / v.x, y / v.y, z / v.z);
	}

	Vec3 operator+(float v) const
	{
		return Vec3(x + v, y + v, z + v);
	}

	Vec3 operator-(float v) const
	{
		return Vec3(x - v, y - v, z - v);
	}

	Vec3 operator*(float v) const
	{
		return Vec3(x * v, y * v, z * v);
	}

	Vec3 operator/(float v) const
	{
		return Vec3(x / v, y / v, z / v);
	}

	void Set(float X = 0.0f, float Y = 0.0f, float Z = 0.0f)
	{
		x = X; y = Y; z = Z;
	}

	float Length(void) const
	{
		return sqrtf(x * x + y * y + z * z);
	}

	float LengthSqr(void) const
	{
		return (x * x + y * y + z * z);
	}

	float Normalize()
	{
		float fl_length = Length();
		float fl_length_normal = 1.f / (1.192092896e-07F + fl_length);

		x = x * fl_length_normal;
		y = y * fl_length_normal;
		z = z * fl_length_normal;

		return fl_length;
	}

	Vec3 toAngle( ) const noexcept
	{
		return Vec3{ RAD2DEG( atan2( -z, hypot( x, y ) ) ),
					 RAD2DEG( atan2( y, x ) ),
					 0.0f };
	}

	float const *Base( ) const
	{
		return ( float const * )this;
	}

	//-----------------------------------------------------------------------------
	inline float *Base( )
	{
		return ( float * )this;
	}


	Vec3 fromAngle( ) const noexcept
	{
		return Vec3{ float(cos( DEG2RAD( x ) ) * cos( DEG2RAD( y ) ) ),
					 float( cos( DEG2RAD( x ) ) * sin( DEG2RAD( y ) ) ),
					 float( -sin( DEG2RAD( x ) ) ) };
	}

	void Rotate2D( const float &flYaw )
	{
		float s, c, r = DEG2RAD( flYaw );
		s = float( sin( r ) ); c = float( cos( r ) );

		float _x = x, _y = y;

		x = ( _x * c ) - ( _y * s );
		y = ( _x * s ) + ( _y * c );
	}

	float NormalizeInPlace()
	{
		return Normalize();
	}

	float Length2D(void) const
	{
		return sqrtf(x * x + y * y);
	}

	float Length2DSqr(void) const
	{
		return (x * x + y * y);
	}

	float DistTo(const Vec3 &v) const
	{
		return (*this - v).Length();
	}

	float DistToSqr(const Vec3 &v) const
	{
		return (*this - v).LengthSqr();
	}

	float Dist2D(const Vec3& v) const
	{
		return (*this - v).Length2D();
	}

	float Dot(const Vec3 &v) const
	{
		return (x * v.x + y * v.y + z * v.z);
	}

	Vec3 Cross(const Vec3 &v) const
	{
		return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}

	bool IsZero(void) const
	{
		return (x > -0.01f && x < 0.01f &&
			y > -0.01f && y < 0.01f &&
			z > -0.01f && z < 0.01f);
	}

	Vec3 Scale(float fl) {
		return Vec3(x * fl, y * fl, z * fl);
	}

	void Init(float ix, float iy, float iz)
	{
		x = ix; y = iy; z = iz;
	}
};

class Vec2
{
public:
	float x, y;

public:
	Vec2(void)
	{
		x = y = 0.0f;
	}

	Vec2(float X, float Y)
	{
		x = X; y = Y;
	}

	Vec2(float *v)
	{
		x = v[0]; y = v[1];
	}

	Vec2(const float *v)
	{
		x = v[0]; y = v[1];
	}

	Vec2(const Vec2 &v)
	{
		x = v.x; y = v.y;
	}

	Vec2 &operator=(const Vec2 &v)
	{
		x = v.x; y = v.y; return *this;
	}

	float &operator[](int i)
	{
		return ((float *)this)[i];
	}

	float operator[](int i) const
	{
		return ((float *)this)[i];
	}

	Vec2 &operator+=(const Vec2 &v)
	{
		x += v.x; y += v.y; return *this;
	}

	Vec2 &operator-=(const Vec2 &v)
	{
		x -= v.x; y -= v.y; return *this;
	}

	Vec2 &operator*=(const Vec2 &v)
	{
		x *= v.x; y *= v.y; return *this;
	}

	Vec2 &operator/=(const Vec2 &v)
	{
		x /= v.x; y /= v.y; return *this;
	}

	Vec2 &operator+=(float v)
	{
		x += v; y += v; return *this;
	}

	Vec2 &operator-=(float v)
	{
		x -= v; y -= v; return *this;
	}

	Vec2 &operator*=(float v)
	{
		x *= v; y *= v; return *this;
	}

	Vec2 &operator/=(float v)
	{
		x /= v; y /= v; return *this;
	}

	Vec2 operator+(const Vec2 &v) const
	{
		return Vec2(x + v.x, y + v.y);
	}

	Vec2 operator-(const Vec2 &v) const
	{
		return Vec2(x - v.x, y - v.y);
	}

	Vec2 operator*(const Vec2 &v) const
	{
		return Vec2(x * v.x, y * v.y);
	}

	Vec2 operator/(const Vec2 &v) const
	{
		return Vec2(x / v.x, y / v.y);
	}

	Vec2 operator+(float v) const
	{
		return Vec2(x + v, y + v);
	}

	Vec2 operator-(float v) const
	{
		return Vec2(x - v, y - v);
	}

	Vec2 operator*(float v) const
	{
		return Vec2(x * v, y * v);
	}

	Vec2 operator/(float v) const
	{
		return Vec2(x / v, y / v);
	}

	void Set(float X = 0.0f, float Y = 0.0f)
	{
		x = X; y = Y;
	}

	float Length(void) const
	{
		return sqrtf(x * x + y * y);
	}

	float LengthSqr(void) const
	{
		return (x * x + y * y);
	}

	float DistTo(const Vec2 &v) const
	{
		return (*this - v).Length();
	}

	float DistToSqr(const Vec2 &v) const
	{
		return (*this - v).LengthSqr();
	}

	float Dot(const Vec2 &v) const
	{
		return (x * v.x + y * v.y);
	}

	bool IsZero(void) const
	{
		return (x > -0.01f && x < 0.01f &&
			y > -0.01f && y < 0.01f);
	}
};

using Vector = Vec3;
using Vector2D = Vec2;

class Vector4D {
	float x, y, z, w;
};