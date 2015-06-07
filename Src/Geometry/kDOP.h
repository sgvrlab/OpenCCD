#pragma once

#include "vec3f.h"
#include <float.h>
#include <stdlib.h>

#define MAX(a,b)	((a) > (b) ? (a) : (b))
#define MIN(a,b)	((a) < (b) ? (a) : (b))

/*
     * k=18 (Aabb + 12 diagonal planes that "cut off" some space of the edges):
     * \li (-1,0,0) and (1,0,0)  -> indices 0 and 9
     * \li (0,-1,0) and (0,1,0)  -> indices 1 and 10
     * \li (0,0,-1) and (0,0,1)  -> indices 2 and 11

     * \li (-1,-1,0) and (1,1,0) -> indices 3 and 12
     * \li (-1,0,-1) and (1,0,1) -> indices 4 and 13
     * \li (0,-1,-1) and (0,1,1) -> indices 5 and 14
     * \li (-1,1,0) and (1,-1,0) -> indices 6 and 15
     * \li (-1,0,1) and (1,0,-1) -> indices 7 and 16
     * \li (0,-1,1) and (0,1,-1) -> indices 8 and 17
*/

class kDOP18 {
public:
	FORCEINLINE static void getDistances(const Vec3f& p,
		float &d3, float &d4, float &d5, float &d6, float &d7, float &d8)
	{
		d3 = p[0] + p[1];
		d4 = p[0] + p[2];
		d5 = p[1] + p[2];
		d6 = p[0] - p[1];
		d7 = p[0] - p[2];
		d8 = p[1] - p[2];
	}

	FORCEINLINE static void getDistances(const Vec3f& p, float d[])
	{
		d[0] = p[0] + p[1];
		d[1] = p[0] + p[2];
		d[2] = p[1] + p[2];
		d[3] = p[0] - p[1];
		d[4] = p[0] - p[2];
		d[5] = p[1] - p[2];
	}

	FORCEINLINE static float getDistances(const Vec3f &p, int i)
	{
		if (i == 0) return p[0]+p[1];
		if (i == 1) return p[0]+p[2];
		if (i == 2) return p[1] + p[2];
		if (i == 3) return p[0] - p[1];
		if (i == 4) return p[0] - p[2];
		if (i == 5) return p[1] - p[2];
		return 0;
	}

public:
	float _dist[18];

	FORCEINLINE kDOP18() {
		empty();
	}

	FORCEINLINE kDOP18(const Vec3f &v) {
		_dist[0] = _dist[9]  = v[0];
		_dist[1] = _dist[10] = v[1];
		_dist[2] = _dist[11] = v[2];

		float d3, d4, d5, d6, d7, d8;
		getDistances(v, d3, d4, d5, d6, d7, d8);
		_dist[3] = _dist[12] = d3;
		_dist[4] = _dist[13] = d4;
		_dist[5] = _dist[14] = d5;
		_dist[6] = _dist[15] = d6;
		_dist[7] = _dist[16] = d7;
		_dist[8] = _dist[17] = d8;
 	}

	FORCEINLINE kDOP18(const Vec3f &a, const Vec3f &b) {
		_dist[0]  = MIN(a[0], b[0]);
		_dist[9]  = MAX(a[0], b[0]);
		_dist[1]  = MIN(a[1], b[1]);
		_dist[10] = MAX(a[1], b[1]);
		_dist[2]  = MIN(a[2], b[2]);
		_dist[11] = MAX(a[2], b[2]);

		float ad3, ad4, ad5, ad6, ad7, ad8;
		getDistances(a, ad3, ad4, ad5, ad6, ad7, ad8);
		float bd3, bd4, bd5, bd6, bd7, bd8;
		getDistances(b, bd3, bd4, bd5, bd6, bd7, bd8);
		_dist[3]  = MIN(ad3, bd3);
		_dist[12] = MAX(ad3, bd3);
		_dist[4]  = MIN(ad4, bd4);
		_dist[13] = MAX(ad4, bd4);
		_dist[5]  = MIN(ad5, bd5);
		_dist[14] = MAX(ad5, bd5);
		_dist[6]  = MIN(ad6, bd6);
		_dist[15] = MAX(ad6, bd6);
		_dist[7]  = MIN(ad7, bd7);
		_dist[16] = MAX(ad7, bd7);
		_dist[8]  = MIN(ad8, bd8);
		_dist[17] = MAX(ad8, bd8);
	}

	FORCEINLINE bool overlaps(const kDOP18& b) const
	{
		for (int i=0; i<9; i++) {
			if (_dist[i] > b._dist[i+9]) return false;
			if (_dist[i+9] < b._dist[i]) return false;
		}

		return true;
	}

	FORCEINLINE bool overlaps(const kDOP18 &b, kDOP18 &ret) const
	{
		if (!overlaps(b))
			return false;

		for (int i=0; i<9; i++) {
			ret._dist[i] = MAX(_dist[i],  b._dist[i]);
			ret._dist[i+9] = MIN(_dist[i+9], b._dist[i+9]);
		}
		return true;
	}

	FORCEINLINE bool inside(const Vec3f &p) const
	{
		for (int i=0; i<3; i++) {
			if (p[i] < _dist[i] || p[i] > _dist[i+9])
				return false;
		}

		float d[6];
		getDistances(p, d);
		for (int i=3; i<9; i++) {
			if (d[i-3] < _dist[i] || d[i-3] > _dist[i+9])
				return false;
		}

		return true;
	}

	FORCEINLINE kDOP18 &operator += (const Vec3f &p)
	{
		_dist[0]  = MIN(p[0], _dist[0]);
		_dist[9]  = MAX(p[0], _dist[9]);
		_dist[1]  = MIN(p[1], _dist[1]);
		_dist[10] = MAX(p[1], _dist[10]);
		_dist[2]  = MIN(p[2], _dist[2]);
		_dist[11] = MAX(p[2], _dist[11]);

		float d3, d4, d5, d6, d7, d8;
		getDistances(p, d3, d4, d5, d6, d7, d8);
		_dist[3]  = MIN(d3, _dist[3]);
		_dist[12] = MAX(d3, _dist[12]);
		_dist[4]  = MIN(d4, _dist[4]);
		_dist[13] = MAX(d4, _dist[13]);
		_dist[5]  = MIN(d5, _dist[5]);
		_dist[14] = MAX(d5, _dist[14]);
		_dist[6]  = MIN(d6, _dist[6]);
		_dist[15] = MAX(d6, _dist[15]);
		_dist[7]  = MIN(d7, _dist[7]);
		_dist[16] = MAX(d7, _dist[16]);
		_dist[8]  = MIN(d8, _dist[8]);
		_dist[17] = MAX(d8, _dist[17]);

		return *this;
	}

	FORCEINLINE kDOP18 &operator += (const kDOP18 &b)
	{
		_dist[0]  = MIN(b._dist[0], _dist[0]);
		_dist[9]  = MAX(b._dist[9], _dist[9]);
		_dist[1]  = MIN(b._dist[1], _dist[1]);
		_dist[10] = MAX(b._dist[10], _dist[10]);
		_dist[2]  = MIN(b._dist[2], _dist[2]);
		_dist[11] = MAX(b._dist[11], _dist[11]);
		_dist[3]  = MIN(b._dist[3], _dist[3]);
		_dist[12] = MAX(b._dist[12], _dist[12]);
		_dist[4]  = MIN(b._dist[4], _dist[4]);
		_dist[13] = MAX(b._dist[13], _dist[13]);
		_dist[5]  = MIN(b._dist[5], _dist[5]);
		_dist[14] = MAX(b._dist[14], _dist[14]);
		_dist[6]  = MIN(b._dist[6], _dist[6]);
		_dist[15] = MAX(b._dist[15], _dist[15]);
		_dist[7]  = MIN(b._dist[7], _dist[7]);
		_dist[16] = MAX(b._dist[16], _dist[16]);
		_dist[8]  = MIN(b._dist[8], _dist[8]);
		_dist[17] = MAX(b._dist[17], _dist[17]);
		return *this;
	}

	FORCEINLINE kDOP18 operator + ( const kDOP18 &v) const
	{ kDOP18 rt(*this); return rt += v; }

	FORCEINLINE float length(int i) const {
		return _dist[i+9]-_dist[i];
	}

	FORCEINLINE float width()  const { return _dist[9] - _dist[0]; }
	FORCEINLINE float height() const { return _dist[10] - _dist[1]; }
	FORCEINLINE float depth()  const { return _dist[11] - _dist[2]; }
	FORCEINLINE float volume() const { return width()*height()*depth(); }

	FORCEINLINE Vec3f center() const { 
		return Vec3f(_dist[0]+_dist[9], _dist[1]+_dist[10], _dist[2]+_dist[11])*0.5f;
	}

	FORCEINLINE float center(int i) const {
		return (_dist[i+9]+_dist[i])*0.5f;
	}

	FORCEINLINE void empty() {
		for (int i=0; i<9; i++) {
			_dist[i] = FLT_MAX;
			_dist[i+9] = -FLT_MAX;
		}
	}

	void visulization();
};