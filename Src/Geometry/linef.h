#pragma once
#include "vec3f.h"
#include <math.h>

class linef
{
public:

	FORCEINLINE linef()
	{ set_value(Vec3f(0,0,0),Vec3f(0,0,1)); }

	FORCEINLINE linef( const Vec3f & p0, const Vec3f &p1)
	{ set_value(p0,p1); }

	FORCEINLINE void set_value( const Vec3f &p0, const Vec3f &p1)
	{
		position = p0;
		direction = p1-p0;
		direction.normalize();
	}

	FORCEINLINE bool get_closest_points(const linef &line2, 
		Vec3f &pointOnThis,
		Vec3f &pointOnThat)
	{

		// quick check to see if parallel -- if so, quit.
		if(fabs(direction.dot(line2.direction)) == 1.0)
			return 0;
		linef l2 = line2;

		// Algorithm: Brian Jean
		// 
		register float u;
		register float v;
		Vec3f Vr = direction;
		Vec3f Vs = l2.direction;
		register float Vr_Dot_Vs = Vr.dot(Vs);
		register float detA = float(1.0 - (Vr_Dot_Vs * Vr_Dot_Vs));
		Vec3f C = l2.position - position;
		register float C_Dot_Vr =  C.dot(Vr);
		register float C_Dot_Vs =  C.dot(Vs);

		u = (C_Dot_Vr - Vr_Dot_Vs * C_Dot_Vs)/detA;
		v = (C_Dot_Vr * Vr_Dot_Vs - C_Dot_Vs)/detA;

		pointOnThis = position;
		pointOnThis += direction * u;
		pointOnThat = l2.position;
		pointOnThat += l2.direction * v;

		return 1;
	}

	FORCEINLINE Vec3f get_closest_point(const Vec3f &point)
	{
		Vec3f np = point - position;
		Vec3f rp = direction*direction.dot(np)+position;
		return rp;
	}

	FORCEINLINE const Vec3f & get_position() const {return position;}

	FORCEINLINE const Vec3f & get_direction() const {return direction;}

	//protected:
	Vec3f position;
	Vec3f direction;
};
