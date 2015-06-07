#pragma once

#include "Geometry/vec3f.h"

//#define ccdTimeResolution GLH_EPSILON
#define     ccdTimeResolution         double(10e-8)

#define zeroRes double(10e-8)
#define IsZero(x) ((x) < zeroRes && (x) > -zeroRes ? true : false)

typedef struct {
	Vec3f ad, bd, cd, pd;
	Vec3f a0, b0, c0, p0;
} NewtonCheckData;

/*
* Ordinary inside-triangle test for p. The triangle normal is computed from the vertices.
*/
static inline bool _insideTriangle(const Vec3f &a, const Vec3f &b, const Vec3f &c, const Vec3f &p, Vec3f &baryc)
{
	Vec3f n, da, db, dc;
	double wa, wb, wc;

	Vec3f ba = b-a;
	Vec3f ca = c-a;
	n = ba.cross(ca);

	da = a - p, db = b - p, dc = c - p;
	if ((wa = (db.cross(dc)).dot(n)) < 0.0f) return false;
	if ((wb = (dc.cross(da)).dot(n)) < 0.0f) return false;
	if ((wc = (da.cross(db)).dot(n)) < 0.0f) return false;

	//Compute barycentric coordinates
	float area2 = n.dot(n);
	wa /= area2, wb /= area2, wc /= area2;

	baryc = Vec3f(wa, wb, wc);

	return true;
}

static inline bool _insideLnSeg(const Vec3f &a, const Vec3f &b, const Vec3f &c)
{
	return (a-b).dot(a-c)<=0;
}

static inline bool checkRootValidity_VE(float t, NewtonCheckData &data)
{
	return _insideLnSeg(
		data.ad*t + data.a0, 
		data.bd*t + data.b0, 
		data.cd*t + data.c0);

}

static inline bool checkRootValidity_VF(double t, Vec3f& baryc, NewtonCheckData &data) {
	return _insideTriangle(
		data.ad*t + data.a0, 
		data.bd*t + data.b0, 
		data.cd*t + data.c0, 
		data.pd*t + data.p0,
		baryc);
}


// http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline3d/

/*
Calculate the line segment PaPb that is the shortest route between
two lines P1P2 and P3P4. Calculate also the values of mua and mub where
Pa = P1 + mua (P2 - P1)
Pb = P3 + mub (P4 - P3)
Return FALSE if no solution exists.
*/
inline bool LineLineIntersect(
							  const Vec3f &p1, const Vec3f &p2, const Vec3f &p3, const Vec3f &p4,
							  Vec3f &pa, Vec3f &pb, double &mua, double &mub)
{
	Vec3f p13,p43,p21;
	double d1343,d4321,d1321,d4343,d2121;
	double numer,denom;

	p13 = p1 - p3;
	p43 = p4 - p3;
	if (fabs(p43[0])  < GLH_EPSILON && fabs(p43[1])  < GLH_EPSILON && fabs(p43[2])  < GLH_EPSILON)
		return false;

	p21 = p2 - p1;
	if (fabs(p21[0])  < GLH_EPSILON && fabs(p21[1])  < GLH_EPSILON && fabs(p21[2])  < GLH_EPSILON)
		return false;

	d1343 = p13.dot(p43);
	d4321 = p43.dot(p21);
	d1321 = p13.dot(p21);
	d4343 = p43.dot(p43);
	d2121 = p21.dot(p21);

	denom = d2121 * d4343 - d4321 * d4321;
	if (fabs(denom) < GLH_EPSILON)
		return false;
	numer = d1343 * d4321 - d1321 * d4343;

	mua = numer / denom;
	if (mua < 0 || mua > 1)
		return false;

	mub = (d1343 + d4321 * mua) / d4343;
	if (mub < 0 || mub > 1)
		return false;

	pa = p1 + p21*mua;
	pb = p3 + p43*mub;
	return true;
}

static inline bool checkRootValidity_EE(double t, Vec3f &pt, NewtonCheckData &data) {
	Vec3f a = data.ad*t + data.a0;
	Vec3f b = data.bd*t + data.b0;
	Vec3f c = data.cd*t + data.c0;
	Vec3f d = data.pd*t + data.p0;

	Vec3f p1, p2;
	double tab, tcd;

	if (LineLineIntersect(a, b, c, d, p1, p2, tab, tcd)) {
		t = tab;
		pt = p1;
		return true;
	}

	return false;
}

inline bool solveSquare(float a, float b, float c, NewtonCheckData &data)
{
	if (IsZero(a)) {
		float t = -c/b;
		return (t >= 0 && t <= 1) ? checkRootValidity_VE(t, data) : false;
	}

	float discriminant = b*b-4*a*c;
	if (discriminant < 0)
		return false;

	float tmp = sqrt(discriminant);
	float ta = 0.5f/a;

	Vec3f baryc;
	float r1 = (-b+tmp)*ta;
	bool v1 = (r1 >= 0.f && r1 <= 1.f) ? checkRootValidity_VE(r1, data) : false;
	if (v1) return true;

	float r2 = (-b-tmp)*ta;
	bool v2 = (r2 >= 0.f && r2 <= 1.f) ? checkRootValidity_VE(r2, data) : false;
	return v2;
}

#ifdef SQR_SLV
inline bool solveSquare(float a, float b, float c, float &ret, bool bVF)
{
	float discriminant = b*b-4*a*c;
	if (discriminant < 0)
		return false;

	float tmp = sqrt(discriminant);
	float ta = 0.5f/a;

	Vec3f baryc;
	float r1 = (-b+tmp)*ta;
	bool v1 = (r1 >= 0.f && r1 <= 1.f) ? ((bVF) ? checkRootValidity_VF(r1, baryc) : checkRootValidity_EE(r1, baryc)) : false;

	float r2 = (-b-tmp)*ta;
	bool v2 = (r2 >= 0.f && r2 <= 1.f) ? ((bVF) ? checkRootValidity_VF(r2, baryc) : checkRootValidity_EE(r2, baryc)) : false;

	if (v1 && v2) {
		ret = r1 > r2 ? r2 : r1;
		return true;
	}
	if (v1) {
		ret = r1;
		return true;
	}
	if (v2) {
		ret = r2;
		return true;
	}

	return false;
}
#endif

inline bool solveCubicWithIntervalNewton(double &l, double &r, Vec3f& baryc, bool bVF,
										 NewtonCheckData &data, double coeffs[])
{
	double v2[2]={l*l,r*r};
	double v[2]={l,r};
	double rBkUp;

	unsigned char min3, min2, min1, max3, max2, max1;

	min3=*((unsigned char*)&coeffs[3]+7)>>7;max3=min3^1;
	min2=*((unsigned char*)&coeffs[2]+7)>>7;max2=min2^1;
	min1=*((unsigned char*)&coeffs[1]+7)>>7;max1=min1^1;

	// bound the cubic

	double minor=coeffs[3]*v2[min3]*v[min3]+coeffs[2]*v2[min2]+coeffs[1]*v[min1]+coeffs[0];
	double major=coeffs[3]*v2[max3]*v[max3]+coeffs[2]*v2[max2]+coeffs[1]*v[max1]+coeffs[0];

	if (major<0) return false;
	if (minor>0) return false;

	// starting here, the bounds have opposite values

	double m=0.5*(r+l);

	// bound the derivative

	double dminor=3.0*coeffs[3]*v2[min3]+2.0*coeffs[2]*v[min2]+coeffs[1];
	double dmajor=3.0*coeffs[3]*v2[max3]+2.0*coeffs[2]*v[max2]+coeffs[1];

	if ((dminor>0)||(dmajor<0)) // we can use Newton
	{
		double m2=m*m;
		double fm=coeffs[3]*m2*m+coeffs[2]*m2+coeffs[1]*m+coeffs[0];
		double nl=m;
		double nu=m;
		if (fm>0) {nl-=fm*(1.0/dminor);nu-=fm*(1.0/dmajor);}
		else {nu-=fm*(1.0/dminor);nl-=fm*(1.0/dmajor);}

		//intersect with [l,r]

		if (nl>r) return false; // pas de solution
		if (nu<l) return false; // pas de solution
		if (nl>l)
		{
			if (nu<r) {l=nl;r=nu;m=0.5*(l+r);}
			else {l=nl;m=0.5*(l+r);}
		}
		else
		{
			if (nu<r) {r=nu;m=0.5*(l+r);}
		}
	}

	// sufficient temporal resolution, check root validity
	if ((r-l)<ccdTimeResolution)
		if (bVF)
			return checkRootValidity_VF(r, baryc, data);
		else
			return checkRootValidity_EE(r, baryc, data);

	rBkUp = r, r = m;
	if (solveCubicWithIntervalNewton(l,r,baryc, bVF, data, coeffs)) return true;	
	l = m, r = rBkUp;
	return (solveCubicWithIntervalNewton(l,r,baryc, bVF, data, coeffs));
}

/*
* Computes the coefficients of the cubic equations from the geometry data.
*/
inline void _equateCubic_VF(const Vec3f &a0, const Vec3f &ad, const Vec3f &b0, const Vec3f &bd, 
							const Vec3f &c0, const Vec3f &cd, const Vec3f &p0, const Vec3f &pd,
							float &a, float &b, float &c, float &d)
{
	/*
	* For definitions & notation refer to the semester thesis doc.
	*/
	Vec3f dab, dac, dap;
	Vec3f oab, oac, oap;
	Vec3f dabXdac, dabXoac, oabXdac, oabXoac;

	dab = bd - ad, dac = cd - ad, dap = pd - ad;
	oab = b0 - a0, oac = c0 - a0, oap = p0 - a0;
	dabXdac = dab.cross(dac);
	dabXoac = dab.cross(oac);
	oabXdac = oab.cross(dac);
	oabXoac = oab.cross(oac);

	a = dap.dot(dabXdac);
	b = oap.dot(dabXdac) + dap.dot(dabXoac + oabXdac);
	c = dap.dot(oabXoac) + oap.dot(dabXoac + oabXdac);
	d = oap.dot(oabXoac);
}

/*
* Computes the coefficients of the cubic equations from the geometry data.
*/
inline void _equateCubic_VE(
							const Vec3f &a0, const Vec3f &ad, const Vec3f &b0, const Vec3f &bd, 
							const Vec3f &c0, const Vec3f &cd, const Vec3f &L,
							float &a, float &b, float &c)
{
	/*
	* For definitions & notation refer to the semester thesis doc.
	*/
	Vec3f dab, dcb;
	Vec3f oab, ocb;

	dab = ad-bd; dcb = cd-bd;
	oab = a0-b0; ocb = c0-b0;

	Vec3f Ldcb = L.cross(dcb);
	Vec3f Locb = L.cross(ocb);

	a = Ldcb.dot(dab);
	b = Ldcb.dot(oab) + Locb.dot(dab);
	c = Locb.dot(oab);
}

/*
* Computes the coefficients of the cubic equations from the geometry data.
*/
inline void _equateCubic_EE(const Vec3f &a0, const Vec3f &ad, const Vec3f &b0, const Vec3f &bd, 
							const Vec3f &c0, const Vec3f &cd, const Vec3f &d0, const Vec3f &dd,
							float &a, float &b, float &c, float &d)
{
	/*
	* For definitions & notation refer to the semester thesis doc.
	*/
	Vec3f dba, ddc, dca;
	Vec3f odc, oba, oca;
	Vec3f dbaXddc, dbaXodc, obaXddc, obaXodc;

	dba = bd - ad, ddc = dd - cd, dca = cd - ad;
	odc = d0 - c0, oba = b0 - a0, oca = c0 - a0;
	dbaXddc = dba.cross(ddc);
	dbaXodc = dba.cross(odc);
	obaXddc = oba.cross(ddc);
	obaXodc = oba.cross(odc);

	a = dca.dot(dbaXddc);
	b = oca.dot(dbaXddc) + dca.dot(dbaXodc + obaXddc);
	c = dca.dot(obaXodc) + oca.dot(dbaXodc + obaXddc);
	d = oca.dot(obaXodc);
}

bool
Intersect_VE(const Vec3f &ta0, const Vec3f &tb0, const Vec3f &tc0,
			 const Vec3f &ta1, const Vec3f &tb1, const Vec3f &tc1, const Vec3f &L)
{
	Vec3f ad, bd, cd;
	/* diff. vectors for linear interpolation */
	ad = ta1 - ta0, bd = tb1 - tb0, cd = tc1 - tc0;

	/*
	* Compute scalar coefficients by evaluating dot and cross-products.
	*/
	float a, b, c; /* cubic polynomial coefficients */
	_equateCubic_VE(ta0, ad, tb0, bd, tc0, cd, L, a, b, c);

	if (IsZero(a) && IsZero(b) && IsZero(c))
		return true;

	NewtonCheckData data;
	data.a0 = ta0, data.b0 = tb0, data.c0 = tc0;
	data.ad = ad,  data.bd = bd,  data.cd = cd;

	return solveSquare(a, b, c, data);
}

float
Intersect_VF(const Vec3f &ta0, const Vec3f &tb0, const Vec3f &tc0,
			 const Vec3f &ta1, const Vec3f &tb1, const Vec3f &tc1,
			 const Vec3f &q0, const Vec3f &q1,
			 Vec3f &qi, Vec3f &baryc)
{
	/* Default value returned if no collision occurs */
	float collisionTime = -1.0f;

	Vec3f qd, ad, bd, cd;
	/* diff. vectors for linear interpolation */
	qd = q1 - q0, ad = ta1 - ta0, bd = tb1 - tb0, cd = tc1 - tc0;


	/*
	* Compute scalar coefficients by evaluating dot and cross-products.
	*/
	float a, b, c, d; /* cubic polynomial coefficients */
	_equateCubic_VF(ta0, ad, tb0, bd, tc0, cd, q0, qd, a, b, c, d);

#ifdef SQR_SLV
	if (IsZero(a)) {
		solveSquare(b, c, d, collisionTime, true);
		return collisionTime;
	}
#endif

	if (IsZero(a) && IsZero(b) && IsZero(c) && IsZero(d))
		return collisionTime;

	/*
	* iteratively solve the cubic (scalar) equation and test for validity of the solution.
	*/
	double l = 0;
	double r = 1;
	NewtonCheckData data;
	data.a0 = ta0, data.b0 = tb0;
	data.c0 = tc0, data.p0 = q0;
	data.ad = ad, data.bd = bd;
	data.cd = cd, data.pd = qd;

	double coeffs[4];
	coeffs[3] = a, coeffs[2] = b, coeffs[1] = c, coeffs[0] = d;
	if (solveCubicWithIntervalNewton(l, r, baryc, true, data, coeffs)) {
		collisionTime = (l+r)*0.5f;	
		qi = qd*collisionTime + q0;				
	}

	return collisionTime;
}

float
Intersect_EE(const Vec3f &ta0, const Vec3f &tb0, const Vec3f &tc0, const Vec3f &td0,
			 const Vec3f &ta1, const Vec3f &tb1, const Vec3f &tc1, const Vec3f &td1,
			 Vec3f &qi)
{
	/* Default value returned if no collision occurs */
	float collisionTime = -1.0f;

	Vec3f ad, bd, cd, dd;
	/* diff. vectors for linear interpolation */
	dd = td1 - td0, ad = ta1 - ta0, bd = tb1 - tb0, cd = tc1 - tc0;

	/*
	* Compute scalar coefficients by evaluating dot and cross-products.
	*/
	float a, b, c, d; /* cubic polynomial coefficients */
	_equateCubic_EE(ta0, ad, tb0, bd, tc0, cd, td0, dd, a, b, c, d);

#ifdef SQR_SLV
	if (IsZero(a)) {
		solveSquare(b, c, d, collisionTime, false);
		return collisionTime;
	}
#endif

	if (IsZero(a) && IsZero(b) && IsZero(c) && IsZero(d))
		return collisionTime;

	/*
	* iteratively solve the cubic (scalar) equation and test for validity of the solution.
	*/
	double l = 0;
	double r = 1;
	NewtonCheckData data;
	data.a0 = ta0, data.b0 = tb0;
	data.c0 = tc0, data.p0 = td0;
	data.ad = ad, data.bd = bd;
	data.cd = cd, data.pd = dd;


	double coeffs[4];
	coeffs[3] = a, coeffs[2] = b, coeffs[1] = c, coeffs[0] = d;

	Vec3f pab;
	if (solveCubicWithIntervalNewton(l, r, pab, false, data, coeffs)) {
		collisionTime = (l+r)*0.5f;	
		qi = pab;				
	}

	return collisionTime;
}