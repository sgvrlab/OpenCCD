#pragma once

#include "vec3f.h"
#include <float.h>
#include <stdlib.h>

#define ERROR_BOUND		0

#define MAX(a,b)	((a) - (b) > 0 ? (a) : (b))
#define MIN(a,b)	(0 < (b) - (a) ? (a) : (b))

inline void
vmin(Vec3f &a, const Vec3f &b)
{
	a.set_value(
		MIN(a[0], b[0]),
		MIN(a[1], b[1]),
		MIN(a[2], b[2]));
}

inline void
vmax(Vec3f &a, const Vec3f &b)
{
	a.set_value(
		MAX(a[0], b[0]),
		MAX(a[1], b[1]),
		MAX(a[2], b[2]));
}


class aabb {
public :
	Vec3f _min;
	Vec3f _max;

	FORCEINLINE aabb() {
		empty();
	}

	FORCEINLINE aabb(const Vec3f &v) {
		_min = _max = v;
	}

	FORCEINLINE aabb(const Vec3f &a, const Vec3f &b) {
		_min = a;
		_max = a;
		vmin(_min, b);
		vmax(_max, b);
	}

	FORCEINLINE bool overlaps(const aabb& b) const
	{
		if (_min[0] - b._max[0] > ERROR_BOUND) return false;	// x
		if (_min[1] - b._max[1] > ERROR_BOUND) return false;	// y
		if (_min[2] - b._max[2] > ERROR_BOUND) return false;	// z

		if (ERROR_BOUND < b._min[0] - _max[0]) return false;
		if (ERROR_BOUND < b._min[1] - _max[1]) return false;
		if (ERROR_BOUND < b._min[2] - _max[2]) return false;

		return true;
	}

	/*
	FORCEINLINE bool overlaps2( const aabb& b ) const {
		aabb a1, a2, b1, b2 ;
		a1.empty() ; a2.empty() ; b1.empty() ; b2.empty() ;

		float caX = (_max[0] + _min[0]) / 2 ;
		float cbX = (b._max[0] + b._min[0]) / 2 ;

		a1._max		= _max ;
		a1._min		= _min ;
		a1._max.x	= caX ;

		a2._max		= _max ;
		a2._min		= _min ;
		a2._min.x	= caX ;

		b1._max		= b._max ;
		b1._min		= b._min ;
		b1._max.x	= cbX ;

		b2._max		= b._max ;
		b2._min		= b._min ;
		b2._min.x	= cbX ;

		if ( a1.overlaps(b1) || a2.overlaps(b2) )
			return true ;

		return false ;
	}
	*/

	FORCEINLINE bool overlaps(const aabb &b, aabb &ret) const
	{
		if (!overlaps(b))
			return false;

		ret._min.set_value(
			MAX(_min[0],  b._min[0]),
			MAX(_min[1],  b._min[1]),
			MAX(_min[2],  b._min[2]));

		ret._max.set_value(
			MIN(_max[0], b._max[0]),
			MIN(_max[1], b._max[1]),
			MIN(_max[2], b._max[2]));

		return true;
	}

	FORCEINLINE bool inside(const Vec3f &p) const
	{
		if (p[0] < _min[0] || p[0] > _max[0]) return false;
		if (p[1] < _min[1] || p[1] > _max[1]) return false;
		if (p[2] < _min[2] || p[2] > _max[2]) return false;

		return true;
	}

	FORCEINLINE aabb &operator += (const Vec3f &p)
	{
		vmin(_min, p);
		vmax(_max, p);

		return *this;
	}

	FORCEINLINE aabb &operator += (const aabb &b)
	{
		vmin(_min, b._min);
		vmax(_max, b._max);

		return *this;
	}

	FORCEINLINE aabb operator + ( const aabb &v) const
		{ aabb rt(*this); return rt += v; }

	FORCEINLINE float width()  const { return _max[0] - _min[0]; }
	FORCEINLINE float height() const { return _max[1] - _min[1]; }
	FORCEINLINE float depth()  const { return _max[2] - _min[2]; }
	FORCEINLINE Vec3f center() const { return (_min+_max)*0.5; }
	FORCEINLINE float volume() const { return width()*height()*depth(); }

	FORCEINLINE void empty() {
		_max = Vec3f(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		_min = Vec3f(FLT_MAX, FLT_MAX, FLT_MAX);
	}

	//FORCEINLINE void enlarge() {
	//	_max.set_value( _max[0] + 0.0001 , _max[1] + 0.0001, _max[2] + 0.0001 ) ;
	//	_min.set_value( _min[0] - 0.0001 , _min[1] - 0.0001, _min[2] - 0.0001 ) ;
	//}

	void visulization();
};