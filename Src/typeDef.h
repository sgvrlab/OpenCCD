#pragma once

#include <vector>
#include <iostream>
#include "Geometry/vec3f.h"

//#define SHOW_COLLIDE_TRIANGLES

typedef	unsigned int	UINT ;
/************************************************************************/
/* edge2f                                                               */
/************************************************************************/
class edge2f {
	unsigned int _vids[2];
	unsigned int _fids[2];

	FORCEINLINE void set(unsigned int id0, unsigned int id1) {
		if (id0 > id1) {
			_vids[0] = id0;
			_vids[1] = id1;
		} else {
			_vids[1] = id0;
			_vids[0] = id1;
		}
	}
public:
	FORCEINLINE edge2f() {
		_vids[0] = _vids[1] = UINT_MAX ;
		_fids[0] = _fids[1] = UINT_MAX ;
	}

	FORCEINLINE edge2f(unsigned id0, unsigned int id1, unsigned int fid) {
		set(id0, id1) ;
		_fids[0] = fid ;
		_fids[1] = UINT_MAX ;
	}

	FORCEINLINE void set_fid2(unsigned id) {
		_fids[1] = id ;
	}

	FORCEINLINE unsigned int vid(int i) { return _vids[i] ; }
	FORCEINLINE unsigned int fid(int i) { return _fids[i] ; }

	FORCEINLINE bool operator == (const edge2f &other) const {
		return (_vids[0] == other._vids[0] && _vids[1] == other._vids[1]);
	}

	FORCEINLINE bool operator < (const edge2f &other) const {
		if (_vids[0] == other._vids[0])
			return _vids[1] < other._vids[1] ;
		else
			return _vids[0] < other._vids[0] ;
	}
};

/**
* A structure including VF collision Info.
*/
typedef struct {
	/** The ID of object containing the collided vertex */
	UINT	objVtx			;
	/** The ID of object containing the collided face */
	UINT	objFace			;
	/** The ID of triangle containing the collided vertex */
	UINT	triVtx			;
	/** The ID of triangle containing the collided face */
	UINT	triFace			;
	/** collision time ( 0.0 ~ 1.0) */
	UINT	indexVtx		;
	/** collision time ( 0.0 ~ 1.0) */
	float	collisionTime	;
} intersectingVF ;

/**
* A structure including EE collision Info.<br>
* The edge_i is a membef of the object_i and triangle_i.<br>
* Also, the trianlge_i is a member of the object_i.
*/
typedef struct {
	/** The ID of object containing edge1 */
	UINT	obj1	;
	/** The ID of object containing edge2 */
	UINT	obj2	;
	/** The ID of triangle containing edge1 */
	UINT	tri1	;
	/** The ID of triangle containing edge2 */
	UINT	tri2	;
	/** Two IDs of vertexes composing edge1 */
	UINT	indexEdge1[2] ;
	/** Two IDs of vertexes composing edge2 */
	UINT	indexEdge2[2] ;
	/** collision time ( 0.0 ~ 1.0) */
	float	collisionTime	;
} intersectingEE ;

class mat4f {
public :
	float	element[4][4] ; 

	mat4f() {
		identityMat() ;
	}

	void identityMat (void) {
		for ( int i = 0 ; i < 4 ; i++ ) {
			for ( int j = 0 ; j < 4 ; j++ ) {
				if ( i == j ) {
					element[i][j] = 1 ;
				} else {
					element[i][j] = 0 ;
				}
			}
		}
	}

	void mulVec3f(Vec3f &v) {
		float x, y, z ;

		x = element[0][0]*v.x + element[0][1]*v.y + element[0][2]*v.z + element[0][3] ;
		y = element[1][0]*v.x + element[1][1]*v.y + element[1][2]*v.z + element[1][3] ;
		z = element[2][0]*v.x + element[2][1]*v.y + element[2][2]*v.z + element[2][3] ;

		v.x = x ;
		v.y = y ;
		v.z = z ;
	}
} ;

/************************************************************************/
/* edge2v                                                               */
/************************************************************************/
class edge2v {
	UINT _vids[2] ;

public :
	FORCEINLINE edge2v() {
		_vids[0] = _vids[1] = UINT_MAX ;
	}

	FORCEINLINE void set ( UINT id0, UINT id1 ) {
		_vids[0] = id0 ;
		_vids[1] = id1 ;
	}

	FORCEINLINE UINT id0() {
		return _vids[0] ;
	}

	FORCEINLINE UINT id1() {
		return _vids[1] ;
	}
};

/************************************************************************/
/* tri3e                                                                */
/************************************************************************/
class tri3e {
	unsigned int _ids[3];

public:
	FORCEINLINE tri3e() {
		_ids[0] = _ids[1] = _ids[2] = UINT_MAX;
	}

	FORCEINLINE tri3e(unsigned int id0, unsigned int id1, unsigned id2) {
		set(id0, id1, id2);
	}

	FORCEINLINE void set(unsigned int id0, unsigned int id1, unsigned int id2) {
		_ids[0] = id0;
		_ids[1] = id1;
		_ids[2] = id2;
	}

	FORCEINLINE unsigned int id(int i) { return _ids[i]; }
};

/************************************************************************/
/* tri3f                                                                */
/************************************************************************/
class tri3f {
	unsigned int _ids[3];

public:
	FORCEINLINE tri3f() {
		_ids[0] = _ids[1] = _ids[2] = UINT_MAX;
	}

	FORCEINLINE tri3f(unsigned int id0, unsigned int id1, unsigned id2) {
		set(id0, id1, id2);
	}

	FORCEINLINE void set(unsigned int id0, unsigned int id1, unsigned int id2) {
		_ids[0] = id0;
		_ids[1] = id1;
		_ids[2] = id2;
	}

	FORCEINLINE unsigned int id(int i) { return _ids[i]; }
	FORCEINLINE unsigned int id0() {return _ids[0];}
	FORCEINLINE unsigned int id1() {return _ids[1];}
	FORCEINLINE unsigned int id2() {return _ids[2];}
};

/************************************************************************/
/* Vector operations                                                    */
/************************************************************************/

// swap two vectors
#define swap(a, b) {\
	Vec3f *tmp = a;\
	a = b;\
	b = tmp;\
}

// linear interpolation
inline Vec3f interp(const Vec3f &p1, const Vec3f &p2, float t) {
	return p1*(1-t)+p2*t;
}

// get normal from three vertices by using cross product
inline Vec3f update(Vec3f &v1, Vec3f &v2, Vec3f &v3) {
	Vec3f s = (v2-v1);
	return s.cross(v3-v1);
}

// get normal from three vertices by using cross product
inline Vec3f update(tri3f &tri, Vec3f *vtxs) {
	Vec3f &v1 = vtxs[tri.id0()];
	Vec3f &v2 = vtxs[tri.id1()];
	Vec3f &v3 = vtxs[tri.id2()];

	return update(v1, v2, v3);
}