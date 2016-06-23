#pragma once

#include "forceline.h"
#include <assert.h>
#include "vec3f.h"

class vec3d {
public:
	union {
		struct {
		double x, y, z;
		};
		struct {
		double v[3];
		};
	};

	FORCEINLINE vec3d ()
	{x=0; y=0; z=0;}

	FORCEINLINE vec3d(const Vec3f &v)
	{
		x = v[0];
		y = v[1];
		z = v[2];
	}

	FORCEINLINE vec3d(const vec3d &v)
	{
		x = v[0];
		y = v[1];
		z = v[2];
	}

	FORCEINLINE const double & operator [] ( int i ) const
	{
		switch (i) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default: assert(0); return x;
		}
	}

	FORCEINLINE vec3d(double x, double y, double z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	FORCEINLINE vec3d operator+ (const vec3d &v) const
	{
		return vec3d(x+v.x, y+v.y, z+v.z);
	}

	FORCEINLINE vec3d operator- (const vec3d &v) const
	{
		return vec3d(x-v.x, y-v.y, z-v.z);
	}

	FORCEINLINE vec3d operator *(float t) const
	{
		return vec3d(x*t, y*t, z*t);
	}

     // cross product
     FORCEINLINE const vec3d cross(const vec3d &vec) const
     {
          return vec3d(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x);
     }

	 FORCEINLINE double dot(const vec3d &vec) const {
		 return x*vec.x+y*vec.y+z*vec.z;
	 }
};