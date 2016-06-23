#pragma once

#include <math.h>

#ifdef _DEBUG
#include <stdio.h>
//#include <glh/glh_linear.h>
#endif

#include "forceline.h"
#define     GLH_ZERO            float(0.0)
#define     GLH_EPSILON         float(10e-6)
#define		GLH_EPSILON_2		float(10e-12)
//#define     equivalent(a,b)     (((a < b + GLH_EPSILON) && (a > b - GLH_EPSILON)) ? true : false)

class Vec3f {
public :
	union {
		struct {
		float x, y, z;
		};
		struct {
		float v[3];
		};
	};
public:

	FORCEINLINE Vec3f ()
	{x=0; y=0; z=0;}

	FORCEINLINE Vec3f(const Vec3f &v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	FORCEINLINE Vec3f(const float *v)
	{
		x = v[0];
		y = v[1];
		z = v[2];
	}

	FORCEINLINE Vec3f(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	FORCEINLINE float operator [] ( int i ) const {return v[i];}

	FORCEINLINE Vec3f &operator += (const Vec3f &v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	FORCEINLINE Vec3f &operator -= (const Vec3f &v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	FORCEINLINE void negate() {
		x = -x;
		y = -y;
		z = -z;
	}

	FORCEINLINE Vec3f operator - () const {
		return Vec3f(-x, -y, -z);
	}

	FORCEINLINE Vec3f operator+ (const Vec3f &v) const
	{
		return Vec3f(x+v.x, y+v.y, z+v.z);
	}

	FORCEINLINE Vec3f operator- (const Vec3f &v) const
	{
		return Vec3f(x-v.x, y-v.y, z-v.z);
	}

	FORCEINLINE Vec3f operator *(float t) const
	{
		return Vec3f(x*t, y*t, z*t);
	}

     // cross product
     FORCEINLINE const Vec3f cross(const Vec3f &vec) const
     {
          return Vec3f(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x);
     }

	 FORCEINLINE float dot(const Vec3f &vec) const {
		 return x*vec.x+y*vec.y+z*vec.z;
	 }

	 FORCEINLINE void normalize() 
	 { 
		 float sum = x*x+y*y+z*z;
		 if (sum > GLH_EPSILON_2) {
			 float base = float(1.0/sqrt(sum));
			 x *= base;
			 y *= base;
			 z *= base;
		 }
	 }

	 FORCEINLINE float length() {
		 return float(sqrt(x*x + y*y + z*z));
	 }

	FORCEINLINE Vec3f & set_value( const float &vx, const float &vy, const float &vz)
	{ x = vx; y = vy; z = vz; return *this; }

	FORCEINLINE bool equal_abs(const Vec3f &other) {
		return x == other.x && y == other.y && z == other.z;
	}

	FORCEINLINE float square_norm() const {
		return x*x+y*y+z*z;
	}

#ifdef _DEBUG
	FORCEINLINE void output() {
		printf("x=%f, y=%f, z=%f\n", x, y, z);
	}
#endif
};
