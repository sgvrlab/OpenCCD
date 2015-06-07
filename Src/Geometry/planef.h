#pragma once

#include "vec3f.h"
#include "linef.h"

class planef {
public:
	FORCEINLINE planef(){
		planedistance = 0.0;
		planenormal.set_value( 0.0, 0.0, 1.0 );
	}

	FORCEINLINE planef( const Vec3f &p0, const Vec3f &p1, const Vec3f &p2 )
	{
		Vec3f v0 = p1 - p0;
		Vec3f v1 = p2 - p0;
		planenormal = v0.cross(v1);  
		planenormal.normalize();
		planedistance = p0.dot(planenormal);
	}

	FORCEINLINE planef( const Vec3f &normal, float distance )
	{
		planedistance = distance;
		planenormal = normal;
		planenormal.normalize();
	}

	FORCEINLINE planef( const Vec3f &normal, const Vec3f &point )
	{
		planenormal = normal;
		planenormal.normalize();
		planedistance = point.dot(planenormal);
	}

	FORCEINLINE void offset( float d )
	{
		planedistance += d;
	}

	FORCEINLINE bool intersect( const linef &l, Vec3f &intersection ) const
	{
		Vec3f pos, dir;
		Vec3f pn = planenormal;
		float pd = planedistance;

		pos = l.get_position();
		dir = l.get_direction();

		if(dir.dot(pn) == 0.0) return 0;
		pos -= pn*pd;
		// now we're talking about a planef passing through the origin
		if(pos.dot(pn) < 0.0) pn.negate();
		if(dir.dot(pn) > 0.0) dir.negate();
		Vec3f ppos = pn * pos.dot(pn);
		//pos = (ppos.length()/dir.dot(-pn))*dir;
		pos = dir*(ppos.length()/dir.dot(-pn));
		intersection = l.get_position();
		intersection += pos;
		return 1;
	}

	FORCEINLINE bool is_in_half_space( const Vec3f &point ) const
	{

		if(( point.dot(planenormal) - planedistance) < 0.0)
			return 0;
		return 1;
	}


	FORCEINLINE float distance( const Vec3f & point ) const 
	{
		return planenormal.dot(point - planenormal*planedistance);
	}

	FORCEINLINE const Vec3f &get_normal() const
	{
		return planenormal;
	}


	FORCEINLINE float get_distance_from_origin() const
	{
		return planedistance;
	}

	//protected:
	Vec3f planenormal;
	float planedistance;
};
