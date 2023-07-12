#pragma once
#include "Color/Color.h"
#include "Hook/Hook.h"
#include "Interface/Interface.h"
#include "Pattern/Pattern.h"
#include "UtlVector/UtlVector.h"
#include "Vector/Vector.h"
#include "WinAPI/WinAPI.h"

#include <algorithm>
#include <stdint.h>
#include <map>
#include <unordered_map>
#include <memory>
#include <string>
#include <fstream>

#undef min
#undef max

//unscoped enum
#pragma warning (disable : 26812)

typedef float matrix3x4_t[3][4];
using QAngle = Vec3;

inline void Q_memcpy(void* dest, const void* src, int count)
{
	int             i;
	if ((((long)dest | (long)src | count) & 3) == 0)
	{
		count >>= 2;
		for (i = 0; i < count; i++)
			((int*)dest)[i] = ((int*)src)[i];
	}
	else
		for (i = 0; i < count; i++)
			((char*)dest)[i] = ((char*)src)[i];
}

class VMatrix
{
private:
	Vector m[4][4];

public:
	inline const matrix3x4_t &As3x4() const {
		return *((const matrix3x4_t *)this);
	}
};

namespace Utils
{
	__inline bool IsGameWindowInFocus()
	{
		static HWND hwGame = 0;

		while (!hwGame) {
			hwGame = WinAPI::FindWindowW(0, (L"Team Fortress 2"));
			return false;
		}

		return (WinAPI::GetForegroundWindow() == hwGame);
	}

	inline std::wstring ConvertUtf8ToWide(const std::string_view &str)
	{
		int count = MultiByteToWideChar(CP_UTF8, 0, str.data(), str.length(), NULL, 0);
		std::wstring wstr(count, 0);
		MultiByteToWideChar(CP_UTF8, 0, str.data(), str.length(), &wstr[0], count);
		return wstr;
	}

	inline float CLR2FLT(unsigned char x) {
		return (static_cast<float>(x) / 255.0f);
	}

	inline unsigned char FLT2CLR( float x )
	{
		return ( unsigned char )( 255.0f * x );
	}
#pragma warning (disable: 4244)
	inline std::vector<unsigned char>HSVTOCLR( float H, float S, float V ) {
		if ( H > 360 || H < 0 || S>100 || S < 0 || V>100 || V < 0 ) {
			return { 0, 0, 0, 0 };
		}
		float s = S / 100;
		float v = V / 100;
		float C = s * v;
		float X = C * ( 1 - ( float )abs( fmod( H / 60.0f, 2 ) - 1 ) );
		float m = v - C;
		float r, g, b;
		if ( H >= 0 && H < 60 ) {
			r = C, g = X, b = 0;
		}
		else if ( H >= 60 && H < 120 ) {
			r = X, g = C, b = 0;
		}
		else if ( H >= 120 && H < 180 ) {
			r = 0, g = C, b = X;
		}
		else if ( H >= 180 && H < 240 ) {
			r = 0, g = X, b = C;
		}
		else if ( H >= 240 && H < 300 ) {
			r = X, g = 0, b = C;
		}
		else {
			r = C, g = 0, b = X;
		}
		unsigned char R = ( r + m ) * 255;
		unsigned char G = ( g + m ) * 255;
		unsigned char B = ( b + m ) * 255;

		return { R, G, B };
	}

	inline void CLR2HSV( Color_t &col, float* hsv )
	{
		float r = col.r / 255.f;
		float g = col.g / 255.f;
		float b = col.b / 255.f;

		float cmax = std::max( r, std::max( g, b ) );
		float cmin = std::min( r, std::min( g, b ) );
		float diff = cmax - cmin;
		float h = -1, s = -1;

		if ( cmax == cmin )
			h = 0;

		// if cmax equal r then compute h
		else if ( cmax == r )
			h = fmod( 60 * ( ( g - b ) / diff ) + 360, 360 );

		// if cmax equal g then compute h
		else if ( cmax == g )
			h = fmod( 60 * ( ( b - r ) / diff ) + 120, 360 );

		// if cmax equal b then compute h
		else if ( cmax == b )
			h = fmod( 60 * ( ( r - g ) / diff ) + 240, 360 );

		// if cmax equal zero
		if ( cmax == 0 )
			s = 0;
		else
			s = ( diff / cmax ) * 100;

		float v = cmax * 100.f;

		hsv[ 0 ] = h;
		hsv[ 1 ] = s;
		hsv[ 2 ] = v;
	}

	inline Vec3 CLR2VEC(Color_t x) {
		return Vec3(CLR2FLT(x.r), CLR2FLT(x.g), CLR2FLT(x.b));
	}

	inline unsigned long CLR2UL(Color_t x) {
		return (static_cast<unsigned long>(((x.r & 0xFF) << 24) | ((x.g & 0xFF) << 16) | ((x.b & 0xFF) << 8) | (x.a & 0xFF)));
	}

	inline void *GetVFuncPtr(void *pBaseClass, unsigned int nIndex) {
		return reinterpret_cast<void *>((*static_cast<int **>(pBaseClass))[nIndex]);
	}

	inline Vec3 GetRotatedPosition( Vec3 vOrigin, const float flRotation, const float flDistance )
	{
		const auto rad = DEG2RAD( flRotation );
		vOrigin.x += cosf( rad ) * flDistance;
		vOrigin.y += sinf( rad ) * flDistance;

		return vOrigin;

	}

	//============math==================

#pragma warning (push)
#pragma warning (disable : 26451)
#pragma warning (disable : 4244)

	inline double __declspec (naked) __fastcall FastSqrt(double n) {
		_asm fld qword ptr[esp + 4]
			_asm fsqrt
		_asm ret 8
	}

	inline float NormalizeAngle(float ang) {
		if (!std::isfinite(ang))
			return 0.0f;

		return std::remainder(ang, 360.0f);
	}

	inline float VectorNormalize(Vec3& vector)
	{
		float length = vector.Length();

		if (!length)
			vector.Set();
		else
			vector /= length;

		return length;
	}

	inline void SinCos(float radians, float *sine, float *cosine)
	{
		_asm
		{
			fld		DWORD PTR[radians]
			fsincos

			mov edx, DWORD PTR[cosine]
			mov eax, DWORD PTR[sine]

			fstp DWORD PTR[edx]
			fstp DWORD PTR[eax]
		}
	}

	inline Vec2 RotateVec2( Vec2 &point, const Vec2 &origin, float radians )
	{
		float s = sin( radians );
		float c = cos( radians );

		//Translate point back to origin
		point.x -= origin.x;
		point.y -= origin.y;

		//Rotate point
		float xnew = point.x * c - point.y * s;
		float ynew = point.x * s + point.y * c;

		//Translate point back
		point.x = xnew + origin.x;
		point.y = ynew + origin.y;
		return point;
	}

	inline void ClampAngles(Vec3 &v) {
		v.x = std::max(-89.0f, std::min(89.0f, NormalizeAngle(v.x)));
		v.y = NormalizeAngle(v.y);
		v.z = 0.0f;
	}

	inline void VectorAngles(const Vec3 &forward, Vec3 &angles)
	{
		float tmp, yaw, pitch;

		if (forward.y == 0 && forward.x == 0)
		{
			yaw = 0;

			if (forward.z > 0)
				pitch = 270;
			else
				pitch = 90;
		}

		else
		{
			yaw = RAD2DEG(atan2f(forward.y, forward.x));

			if (yaw < 0)
				yaw += 360;

			tmp = forward.Length2D();
			pitch = RAD2DEG(atan2f(-forward.z, tmp));

			if (pitch < 0)
				pitch += 360;
		}

		angles[0] = pitch;
		angles[1] = yaw;
		angles[2] = 0;
	}

	inline void VectorCopy( const Vector &src, Vector &dst )
	{
		dst.x = src.x;
		dst.y = src.y;
		dst.z = src.z;
	}

	inline void AngleVectors(const Vec3 &angles, Vec3 *forward)
	{
		float sp, sy, cp, cy;

		SinCos(DEG2RAD(angles.x), &sp, &cp);
		SinCos(DEG2RAD(angles.y), &sy, &cy);

		if (forward)
		{
			forward->x = cp * cy;
			forward->y = cp * sy;
			forward->z = -sp;
		}
	}

	inline void AngleVectors(const Vec3 &angles, Vec3 *forward, Vec3 *right, Vec3 *up)
	{
		float sr, sp, sy, cr, cp, cy;
		SinCos(DEG2RAD(angles.x), &sp, &cp);
		SinCos(DEG2RAD(angles.y), &sy, &cy);
		SinCos(DEG2RAD(angles.z), &sr, &cr);

		if (forward)
		{
			forward->x = cp * cy;
			forward->y = cp * sy;
			forward->z = -sp;
		}

		if (right)
		{
			right->x = (-1 * sr * sp * cy + -1 * cr * -sy);
			right->y = (-1 * sr * sp * sy + -1 * cr * cy);
			right->z = -1 * sr * cp;
		}

		if (up)
		{
			up->x = (cr * sp * cy + -sr * -sy);
			up->y = (cr * sp * sy + -sr * cy);
			up->z = cr * cp;
		}
	}

	inline void VectorTransform(const Vec3& input, const matrix3x4_t& matrix, Vec3& output)
	{
		for (auto i = 0; i < 3; i++)
			output[i] = input.Dot((Vec3&)matrix[i]) + matrix[i][3];
	}

	inline Vec3 CalcAngle(const Vec3 &source, const Vec3 &destination, bool clamp = true)
	{
		Vec3 angles = Vec3(0.0f, 0.0f, 0.0f);
		Vec3 delta = (source - destination);
		float fHyp = FastSqrt((delta.x * delta.x) + (delta.y * delta.y));

		angles.x = (atanf(delta.z / fHyp) * M_RADPI);
		angles.y = (atanf(delta.y / delta.x) * M_RADPI);
		angles.z = 0.0f;

		if (delta.x >= 0.0f)
			angles.y += 180.0f;

		if (clamp)
			ClampAngles(angles);

		return angles;
	}

	inline float CalcFov(const Vec3 &src, const Vec3 &dst)
	{
		Vec3 v_src = Vec3();
		AngleVectors(src, &v_src);

		Vec3 v_dst = Vec3();
		AngleVectors(dst, &v_dst);

		float result = RAD2DEG(acos(v_dst.Dot(v_src) / v_dst.LengthSqr()));

		if (!isfinite(result) || isinf(result) || isnan(result))
			result = 0.0f;

		return result;
	}

	inline float RemapValClamped(float val, float A, float B, float C, float D)
	{
		if (A == B)
			return val >= B ? D : C;
		float cVal = (val - A) / (B - A);
		cVal = std::clamp(cVal, 0.0f, 1.0f);

		return C + (D - C) * cVal;
	}

	inline void MatrixCopy( const matrix3x4_t &source, matrix3x4_t &target )
	{
		for ( int i = 0; i < 3; i++ ) {
			for ( int j = 0; j < 4; j++ ) {
				target[ i ][ j ] = source[ i ][ j ];
			}
		}
	}

	inline void ConcatTransforms( const matrix3x4_t &in1, const matrix3x4_t &in2, matrix3x4_t &out )
	{
		if ( &in1 == &out ) {
			matrix3x4_t in1b;
			MatrixCopy( in1, in1b );
			ConcatTransforms( in1b, in2, out );
			return;
		}

		if ( &in2 == &out ) {
			matrix3x4_t in2b;
			MatrixCopy( in2, in2b );
			ConcatTransforms( in1, in2b, out );
			return;
		}

		out[ 0 ][ 0 ] = in1[ 0 ][ 0 ] * in2[ 0 ][ 0 ] + in1[ 0 ][ 1 ] * in2[ 1 ][ 0 ] +
			in1[ 0 ][ 2 ] * in2[ 2 ][ 0 ];
		out[ 0 ][ 1 ] = in1[ 0 ][ 0 ] * in2[ 0 ][ 1 ] + in1[ 0 ][ 1 ] * in2[ 1 ][ 1 ] +
			in1[ 0 ][ 2 ] * in2[ 2 ][ 1 ];
		out[ 0 ][ 2 ] = in1[ 0 ][ 0 ] * in2[ 0 ][ 2 ] + in1[ 0 ][ 1 ] * in2[ 1 ][ 2 ] +
			in1[ 0 ][ 2 ] * in2[ 2 ][ 2 ];
		out[ 0 ][ 3 ] = in1[ 0 ][ 0 ] * in2[ 0 ][ 3 ] + in1[ 0 ][ 1 ] * in2[ 1 ][ 3 ] +
			in1[ 0 ][ 2 ] * in2[ 2 ][ 3 ] + in1[ 0 ][ 3 ];
		out[ 1 ][ 0 ] = in1[ 1 ][ 0 ] * in2[ 0 ][ 0 ] + in1[ 1 ][ 1 ] * in2[ 1 ][ 0 ] +
			in1[ 1 ][ 2 ] * in2[ 2 ][ 0 ];
		out[ 1 ][ 1 ] = in1[ 1 ][ 0 ] * in2[ 0 ][ 1 ] + in1[ 1 ][ 1 ] * in2[ 1 ][ 1 ] +
			in1[ 1 ][ 2 ] * in2[ 2 ][ 1 ];
		out[ 1 ][ 2 ] = in1[ 1 ][ 0 ] * in2[ 0 ][ 2 ] + in1[ 1 ][ 1 ] * in2[ 1 ][ 2 ] +
			in1[ 1 ][ 2 ] * in2[ 2 ][ 2 ];
		out[ 1 ][ 3 ] = in1[ 1 ][ 0 ] * in2[ 0 ][ 3 ] + in1[ 1 ][ 1 ] * in2[ 1 ][ 3 ] +
			in1[ 1 ][ 2 ] * in2[ 2 ][ 3 ] + in1[ 1 ][ 3 ];
		out[ 2 ][ 0 ] = in1[ 2 ][ 0 ] * in2[ 0 ][ 0 ] + in1[ 2 ][ 1 ] * in2[ 1 ][ 0 ] +
			in1[ 2 ][ 2 ] * in2[ 2 ][ 0 ];
		out[ 2 ][ 1 ] = in1[ 2 ][ 0 ] * in2[ 0 ][ 1 ] + in1[ 2 ][ 1 ] * in2[ 1 ][ 1 ] +
			in1[ 2 ][ 2 ] * in2[ 2 ][ 1 ];
		out[ 2 ][ 2 ] = in1[ 2 ][ 0 ] * in2[ 0 ][ 2 ] + in1[ 2 ][ 1 ] * in2[ 1 ][ 2 ] +
			in1[ 2 ][ 2 ] * in2[ 2 ][ 2 ];
		out[ 2 ][ 3 ] = in1[ 2 ][ 0 ] * in2[ 0 ][ 3 ] + in1[ 2 ][ 1 ] * in2[ 1 ][ 3 ] +
			in1[ 2 ][ 2 ] * in2[ 2 ][ 3 ] + in1[ 2 ][ 3 ];
	}

	inline void AngleMatrix( const Vec3 &angles, matrix3x4_t &matrix )
	{
		float sr, sp, sy, cr, cp, cy;

		SinCos( DEG2RAD( angles.y ), &sy, &cy );
		SinCos( DEG2RAD( angles.x ), &sp, &cp );
		SinCos( DEG2RAD( angles.z ), &sr, &cr );

		// matrix = (YAW * PITCH) * ROLL
		matrix[ 0 ][ 0 ] = cp * cy;
		matrix[ 1 ][ 0 ] = cp * sy;
		matrix[ 2 ][ 0 ] = -sp;

		float crcy = cr * cy;
		float crsy = cr * sy;
		float srcy = sr * cy;
		float srsy = sr * sy;
		matrix[ 0 ][ 1 ] = sp * srcy - crsy;
		matrix[ 1 ][ 1 ] = sp * srsy + crcy;
		matrix[ 2 ][ 1 ] = sr * cp;

		matrix[ 0 ][ 2 ] = sp * crcy + srsy;
		matrix[ 1 ][ 2 ] = sp * crsy - srcy;
		matrix[ 2 ][ 2 ] = cr * cp;

		matrix[ 0 ][ 3 ] = 0.0f;
		matrix[ 1 ][ 3 ] = 0.0f;
		matrix[ 2 ][ 3 ] = 0.0f;
	}

	inline void GetMatrixOrigin( const matrix3x4_t &source, Vec3 &target ) {
		target.x = source[ 0 ][ 3 ];
		target.y = source[ 1 ][ 3 ];
		target.z = source[ 2 ][ 3 ];
	}

	inline void MatrixAngles( const matrix3x4_t &matrix, Vec3 &angles ) {
		//Vec3 forward, left, up;

		// extract the basis vectors from the matrix. since we only need the z
		// component of the up vector, we don't get x and y.
		const Vec3 forward = { matrix[ 0 ][ 0 ], matrix[ 1 ][ 0 ], matrix[ 2 ][ 0 ] };
		const Vec3 left = { matrix[ 0 ][ 1 ], matrix[ 1 ][ 1 ], matrix[ 2 ][ 1 ] };
		const Vec3 up = { 0.f, 0.f, matrix[ 2 ][ 2 ] };

		float len = forward.Length2D( );

		// enough here to get angles?
		if ( len > 0.001f ) {
			angles.x = RAD2DEG( std::atan2( -forward.z, len ) );
			angles.y = RAD2DEG( std::atan2( forward.y, forward.x ) );
			angles.z = RAD2DEG( std::atan2( left.z, up.z ) );
		}

		else {
			angles.x = RAD2DEG( std::atan2( -forward.z, len ) );
			angles.y = RAD2DEG( std::atan2( -left.x, left.y ) );
			angles.z = 0.f;
		}
	}

	template < typename t >
	__forceinline void Clamp(t& n, const t& lower, const t& upper) {
		n = std::max(lower, std::min(n, upper));
	}

#define floatCompare(x, y) \
	(fabsf(x - y) <= FLT_EPSILON * fmaxf(1.0f, fmaxf(fabsf(x), fabsf(y))))

	inline bool RayToOBB(const  Vec3& origin, const  Vec3& direction, const Vec3& position, const Vec3& min, const Vec3& max, const matrix3x4_t orientation) {

		Vec3 p = position - origin;

		Vec3 X(orientation[0][0], orientation[0][1], orientation[0][2]);
		Vec3 Y(orientation[1][0], orientation[1][1], orientation[1][2]);
		Vec3 Z(orientation[2][0], orientation[2][1], orientation[2][2]);

		Vec3 f(
			X.Dot(direction),
			Y.Dot(direction),
			Z.Dot(direction)
		);

		Vec3 e(
			X.Dot(p),
			Y.Dot(p),
			Z.Dot(p)
		);

		float t[6] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
		for (int i = 0; i < 3; ++i) {
			if (floatCompare(f[i], 0)) {
				if (-e[i] + min[i] > 0 || -e[i] + max[i] < 0) {
					return false;
				}
				f[i] = 0.00001f; // Avoid div by 0!
			}

			t[i * 2 + 0] = (e[i] + max[i]) / f[i]; // tmin[x, y, z]
			t[i * 2 + 1] = (e[i] + min[i]) / f[i]; // tmax[x, y, z]
		}

		float tmin = fmaxf(fmaxf(fminf(t[0], t[1]), fminf(t[2], t[3])), fminf(t[4], t[5]));
		float tmax = fminf(fminf(fmaxf(t[0], t[1]), fmaxf(t[2], t[3])), fmaxf(t[4], t[5]));

		// if tmax < 0, ray is intersecting AABB
		// but entire AABB is behing it's origin
		if (tmax < 0) {
			return false;
		}

		// if tmin > tmax, ray doesn't intersect AABB
		if (tmin > tmax) {
			return false;
		}

		return true;
	}

	inline bool CompareFloat( float a, float b, float epsilon = 1.0e-5f )
	{
		return ( fabs( a - b ) <= epsilon * std::max( fabs( a ), fabs( b ) ) );
	}

#pragma warning (pop)
}