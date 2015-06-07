#pragma once

#include "vec3f.h"
#include "vec4d.h"
#include "box.h"

#define MAX(a,b)	((a) > (b) ? (a) : (b))
#define MIN(a,b)	((a) < (b) ? (a) : (b))

inline float middle_xyz(char xyz, const vec3d &p1, const vec3d &p2, const vec3d &p3)
{
	float t0, t1;

	t0 = (float)MIN(p1[xyz], p2[xyz]);
	t0 = (float)MIN(t0, p3[xyz]);
	t1 = (float)MAX(p1[xyz], p2[xyz]);
	t1 = (float)MAX(t1, p3[xyz]);
	return (t0+t1)*0.5f;
}

inline float middle_xyz(char xyz, const Vec3f &p1, const Vec3f &p2, const Vec3f &p3)
{
	float t0, t1;

	t0 = MIN(p1[xyz], p2[xyz]);
	t0 = MIN(t0, p3[xyz]);
	t1 = MAX(p1[xyz], p2[xyz]);
	t1 = MAX(t1, p3[xyz]);
	return (t0+t1)*0.5f;
}

class aap {
public:
	char _xyz;
	float _p;

	FORCEINLINE aap(const BOX &total) {
		Vec3f center = total.center();
		char xyz = 2;

		if (total.width() >= total.height() && total.width() >= total.depth()) {
			xyz = 0;
		} else
		if (total.height() >= total.width() && total.height() >= total.depth()) {
			xyz = 1;
		}

		_xyz = xyz;
		_p = center[xyz];
	}

	FORCEINLINE bool inside(const Vec3f &mid) const {
		return mid[_xyz]>_p;
	}

	FORCEINLINE void moveLeft() {
		_p = _p + _p/10 ;
	}

	FORCEINLINE void moveRight() {
		_p = _p - _p/10 ;
	}
};