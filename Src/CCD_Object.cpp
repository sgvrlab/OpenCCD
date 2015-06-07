/************************************************************************
*								CCD_Object.cpp
************************************************************************
* Author		: Duksu Kim (bluekds@tclab.kaist.ac.kr)
* Affiliation	: SGLAB(http://sglab.kaist.ac.kr), Dept. of Computer Science(http://cs.kaist.ac.kr) , KAIST(http://www.kaist.ac.kr)
* Version		: 0.1 ( Initail version )
* Create		: 2009/02/22
* Last update	: 2009/03/10
************************************************************************/
#include <list>
#include <vector>
#include <assert.h>
#include <algorithm>
#include "CCD_Object.h"
#include <gl/glut.h>

/************************************************************************/
/* Begin & End                                                          */
/************************************************************************/
void CCD_Object::beginObject( UINT numFrame, UINT numVtx, UINT numTri, UINT objType )
{
	printf("Begin Object\n") ;
	
	_curFrame = 0 ;

	// Setting objects properties
	_numFrames	= numFrame	;
	_numVtxs	= numVtx	;
	_numTris	= numTri	;

	if ( _objType <= CCD_OBJECT_TYPE_MAX ) {
		_objType	= objType	;
	} else {
		_objType	= CCD_OBJECT_TYPE_STATIC ;
	}

	// Allocating memory
	_vtxs		= new Vec3f[ _numFrames * _numVtxs ] ;
	_tris		= new tri3f[ _numTris ] ;
	_tris_edge	= new tri3e[ _numTris ] ;

	_cur_vtxs	= new Vec3f[ _numVtxs ] ;
	_prev_vtxs	= new Vec3f[ _numVtxs ] ;

	#ifdef SHOW_COLLIDE_TRIANGLES
	_flags		= new bool[ _numTris ] ;
	#endif

	// State transition
	_state		= CCD_OBJECT_STATE_BEGIN ;
}

void CCD_Object::endObject( void ){

	generateEdges() ;

	//#pragma omp parallel for
	for ( __int64 i = 0 ; i < _numVtxs*_numFrames ; i++ ) {
		_transMat.mulVec3f( _vtxs[i] ) ;
	}

	memcpy( _prev_vtxs, _vtxs, sizeof(Vec3f)*_numVtxs ) ;
	memcpy( _cur_vtxs, _vtxs, sizeof(Vec3f)*_numVtxs ) ;

	_tris_box	= new BOX[ _numTris ] ;
	//#pragma omp parallel for
	for ( __int64 i = 0 ; i < _numTris ; i++ ) {
		_tris_box[i] += _cur_vtxs[_tris[i].id0()] ;
		_tris_box[i] += _cur_vtxs[_tris[i].id1()] ;
		_tris_box[i] += _cur_vtxs[_tris[i].id2()] ;
	}
	// Build BVH
	_bvh = new BVH(this, _numTris) ;
	_bvh->rebuild() ;

	// State transition
	_state		= CCD_OBJECT_STATE_END ;

	printObjectInformation() ;
	printf("End Object\n") ;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCD_Object::generateEdges( void ){
	list<edge2f> edgelist_temp;		// temporary edge list
	
	UINT id0, id1, id2;

	for ( UINT i = 0 ; i < _numTris ; i++ ) {

		id0 = _tris[i].id0() ;
		id1 = _tris[i].id1() ;
		id2 = _tris[i].id2() ;

		edgelist_temp.push_back( edge2f(id0, id1, i) );
		edgelist_temp.push_back( edge2f(id1, id2, i) );
		edgelist_temp.push_back( edge2f(id2, id0, i) );
	}

	edgelist_temp.sort() ;

	list<edge2f> edge_unqie;
	for (list<edge2f>::iterator it=edgelist_temp.begin(); it!=edgelist_temp.end(); it++) {
		if (!edge_unqie.empty() && *it == edge_unqie.back()) { // find duplicated with other fid
			unsigned int fid = (*it).fid(0);
			assert(fid != -1);
			edge_unqie.back().set_fid2(fid);
		} else
			edge_unqie.push_back(*it);
	}

	edgelist_temp.clear();

	vector<edge2f> edge_array;

	UINT _num_edge = (unsigned int)edge_unqie.size();
	initEdges( _num_edge ) ;

	edge2f tempEdge ;
	unsigned int t=0;
	for (list<edge2f>::iterator it=edge_unqie.begin(); it != edge_unqie.end(); it++){
		tempEdge = *it ;
		setEdge( t++, tempEdge.vid(0), tempEdge.vid(1) ) ;
		edge_array.push_back(*it);
	}

	// copy over temp list to static array
	vector <edge2f>::iterator first = edge_array.begin();
	vector <edge2f>::iterator last = edge_array.end();
	for (t = 0; t < _numTris ; t++) {
		
		vector <edge2f>::iterator it1 = lower_bound(first, last, edge2f(_tris[t].id0(), _tris[t].id1(), 0));
		vector <edge2f>::iterator it2 = lower_bound(first, last, edge2f(_tris[t].id1(), _tris[t].id2(), 0));
		vector <edge2f>::iterator it3 = lower_bound(first, last, edge2f(_tris[t].id2(), _tris[t].id0(), 0));

		setTrisEdge( t, it1-first, it2-first, it3-first ) ;
	}
}

/************************************************************************/
/* Sets                                                                 */
/************************************************************************/
void CCD_Object::setVtx( UINT frame, UINT index, Vec3f pos ) {
	_vtxs[ frame*_numVtxs + index ] = pos ;
}

void CCD_Object::setCurVtx( UINT index, Vec3f pos ){
	_cur_vtxs[ index ] = pos ;
}

void CCD_Object::setCurVtx( Vec3f* vtxs ){
	memcpy(_cur_vtxs, vtxs, _numVtxs ) ;
}

void CCD_Object::swapVtxs_Cur_Prev( void ){
	Vec3f* temp ;
	temp = _prev_vtxs ;
	_prev_vtxs = _cur_vtxs ;
	_cur_vtxs = temp ;
}

void CCD_Object::setPrevVtx( UINT index, Vec3f pos ){
	_prev_vtxs[ index ] = pos ;
}

void CCD_Object::setPrevVtx( Vec3f* vtxs ){
	memcpy(_prev_vtxs, vtxs, _numVtxs ) ;
}


void CCD_Object::setTri( UINT index, UINT a, UINT b, UINT c ) {
	_tris[ index ].set( a, b, c ) ;
}

void CCD_Object::setTrisEdge( UINT index, UINT a, UINT b, UINT c ) {
	_tris_edge[ index ].set( a, b, c ) ;
}

void CCD_Object::initEdges( UINT numEdges ) {
	_numEdge = numEdges ;
	_edges = new edge2v[_numEdge] ;
}

void CCD_Object::setEdge( UINT index, UINT a, UINT b ) {
	_edges[index].set( a, b ) ;
}

void CCD_Object::setTransformationMatrix( mat4f transMat ) {
	_transMat = transMat ;
}

bool CCD_Object::setProperty( UINT prop, UINT value ) {
	switch ( prop ) {
		case	CCD_OBJECT_PROPERTY_OBJECT_TYPE :
			_objType = value ;
			return true ;

		//case	CCD_PROPERTY_NUM_FRAME :
		//	_numFrames = value ;
		//	return true ;

		//case	CCD_PROPERTY_NUM_VERTEX :
		//	_numVtxs = value ;
		//	return true ;

		//case	CCD_PROPERTY_NUM_TRIANGLE :
		//	_numTris = value ;
		//	return true ;

		default :
			return false ;
	}
}

/************************************************************************/
/* Information                                                          */
/************************************************************************/
UINT CCD_Object::getProperty( UINT prop ) {
	switch ( prop ) {
		case	CCD_OBJECT_PROPERTY_OBJECT_TYPE :
			return _objType ;
		
		case	CCD_OBJECT_PROPERTY_NUM_FRAME :
			return _numFrames ;
		
		case	CCD_OBJECT_PROPERTY_NUM_VERTEX :
			return _numVtxs ;
		
		case	CCD_OBJECT_PROPERTY_NUM_TRIANGLE :
			return _numTris ;

		case	CCD_OBJECT_PROPERTY_NUM_EDGE :
			return _numEdge ;

		case	CCD_OBJECT_PROPERTY_STATE :
			return _state ;

		default :
			return  0;
	}
}

void CCD_Object::printObjectInformation( void ) {
	printf("----------------------------\n") ;
	printf("\tModel Info.\n") ;
	// 1. Type and state
	switch ( _objType ) {
		case CCD_OBJECT_TYPE_STATIC :

			break ;
		
		case CCD_OBJECT_TYPE_DEFORMABLE :

			break ;
	} 
	// 2. The number of vertex, triangles, and frame
	printf("# of Vtx : %d, # of Edges : %d,# of Tris : %d, # of Frame : %d\n", _numVtxs, _numEdge, _numTris, _numFrames ) ;
	// 3. Transformation matrix
	// 4. Memory usage
}

/************************************************************************/
/* Constructor & Destructor                                             */
/************************************************************************/
CCD_Object::CCD_Object( void ){
	_vtxs		= NULL ;
	_tris		= NULL ;
	_tris_edge	= NULL ;
	_cur_vtxs	= NULL ;
	_prev_vtxs	= NULL ;
	_tris_box	= NULL ;
	_edges		= NULL ;
	_bvh		= NULL ;

	_state	= CCD_OBJECT_STATE_INIT ;
	_objType = CCD_OBJECT_TYPE_DEFORMABLE ;
}

CCD_Object::~CCD_Object(){
	delete _vtxs ;
	delete _tris ;
	delete _tris_edge ;

	delete	_cur_vtxs ;
	delete	_prev_vtxs ;

	delete	_tris_box ;
	
	delete _edges ;

	delete _bvh ;

	#ifdef SHOW_COLLIDE_TRIANGLES
	delete _flags ;
	#endif

}

/************************************************************************/
/* BOX                                                                  */
/************************************************************************/
BOX CCD_Object::getTriBox( UINT index ){
	return _tris_box[index] ;
}

void CCD_Object::refitTriBox( void )
{
	//#pragma omp parallel for
	for ( __int64 i = 0 ; i < _numTris ; i++ ) {
		_tris_box[i].empty() ;
		for ( int j = 0 ; j < 3 ; j++ ) {
			_tris_box[i] += _prev_vtxs[ _tris[i].id(j) ] ;
			_tris_box[i] += _cur_vtxs[ _tris[i].id(j) ] ;
		}
	} // end for i
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCD_Object::nextFrame( void ){

	#ifdef SHOW_COLLIDE_TRIANGLES
	memset( _flags, 0, sizeof(bool)*_numTris ) ;
	#endif

//	if ( _vtxs + _numVtxs*_numFrames >  _cur_vtxs + _numVtxs ) {
	if ( _numFrames > _curFrame ) {

		//_prev_vtxs = _cur_vtxs ;
		//_cur_vtxs = _cur_vtxs + _numVtxs ;
		Vec3f* temp ;
		temp = _prev_vtxs ;
		_prev_vtxs = _cur_vtxs ;
		_cur_vtxs = temp ;
		
		memcpy( _cur_vtxs, _vtxs + _curFrame*_numVtxs, sizeof(Vec3f)*_numVtxs ) ;

		_curFrame++ ;
	
	} // end if
}

void CCD_Object::visualizeObject( void ){
	glBegin( GL_TRIANGLES ) ;
	for ( UINT i = 0 ; i < _numTris ; i++ ) {

		#ifdef SHOW_COLLIDE_TRIANGLES
		if ( _flags[i] == true ) {
			glColor3f( 1.0f, 0.0f, 0.0f ) ;
		} else {
			glColor3f( 0.0f, 1.0f, 0.0f ) ;
		}
		#endif

		Vec3f	_norm ;
		_norm = (_cur_vtxs[_tris[i].id(0)] - _cur_vtxs[_tris[i].id(1)]).cross(_cur_vtxs[_tris[i].id(0)] - _cur_vtxs[_tris[i].id(2)]) ;

		glNormal3f( _norm.x, _norm.y, _norm.z ) ;
		for ( int j = 0 ; j < 3 ; j++ ) {
			glVertex3f( _cur_vtxs[_tris[i].id(j)].x, _cur_vtxs[_tris[i].id(j)].y, _cur_vtxs[_tris[i].id(j)].z ) ;
		}
	}
	glEnd() ;
}