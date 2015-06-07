/************************************************************************
*								CCD.cpp
************************************************************************
* Author		: Duksu Kim (bluekds@tclab.kaist.ac.kr)
* Affiliation	: SGLAB(http://sglab.kaist.ac.kr), Dept. of Computer Science(http://cs.kaist.ac.kr) , KAIST(http://www.kaist.ac.kr)
* Version		: 0.5
* Create		: 2009/02/22
* Last update	: 2009/03/10
************************************************************************/


#include "CCD.h"
#include "typeDef.h"
#include "Geometry/aap.h"
#include <gl/glut.h>

/************************************************************************/
/*                                                                      */
/************************************************************************/
int CCD::addObject( CCD_Object* obj ){
	_objs.push_back( obj ) ;
	return _objs.size() - 1 ;
}

int CCD::delObject( int objID ){
	vector<CCD_Object*>::iterator it = _objs.begin() ;
	for ( int i = 0 ; i < objID ; i++ ) {
		it++ ;
	}
	_objs.erase( it ) ;

	return _objs.size() ;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/

void CCD::readyCCD( UINT startFrame, UINT endFrame, UINT interpolation ){
	_curFrame = startFrame;
	_endFrame = endFrame ;

	_numObjs = _objs.size() ;
	_objList = new UINT[_numObjs] ;

	for ( UINT i = 0 ; i < _numObjs ; i++ ) {
		_objList[i] = i ;
		_objs[i]->setOutput( _output ) ;
	}

	// Build high level BVH
	_bvh = rebuilObjBVH( _objList, _numObjs ) ;
}

void CCD::performCCD( ){

	for ( UINT i = 0 ; i < _numObjs ; i++ ) {
		_objs[i]->refitTriBox() ;
	}

	_bvh->refit() ;
	_bvh->selfCollide() ;
}

void CCD::nextFrame( void ){
	if ( _curFrame <= _endFrame ){

		for ( UINT i = 0 ; i < _numObjs ; i++ ) {
			_objs[i]->nextFrame() ;
		}

		_curFrame++ ;
	}
}

void CCD::visualizeBVH( int level ){

	glColor3f ( 1.0f, 1.0f, 1.0f ) ;

	for ( int i = 0 ; i < _objs.size() ; i++ ) {
		_objs[i]->visualizeBVH( level ) ;
	}
}

void CCD::visualizeObjects( void ){
	
	glColor3f ( 0.0f, 1.0f, 0.0f ) ;
	glEnable(GL_COLOR_MATERIAL) ;

	for ( int i = 0 ; i < _objs.size() ; i++ ) {
		_objs[i]->visualizeObject() ;
	}
}

BVH_Node* CCD::rebuilObjBVH(  UINT* objList, UINT numObj ){

	if ( numObj == 1 ) {
		return _objs[ objList[0] ]->_bvh->_rootNode ;
	}

	BVH_Node* parentNode = new BVH_Node( BVH_NODE_TYPE_OBJECT ) ;
	parentNode->_objList = objList ;
	parentNode->_numObj = numObj ;

	parentNode->_nodeType = BVH_NODE_TYPE_OBJECT ;
	parentNode->_box.empty() ;

	for ( UINT i = 0 ; i < numObj ; i++ ) {
		parentNode->_box += _objs[ objList[i] ]->_bvh->_rootNode->getBox() ;
	}

	aap plan(parentNode->_box) ;
	UINT left_buf = 0, right_buf = numObj - 1 ;

	for ( UINT i = 0 ; i < numObj ; i++ ){
		if ( plan.inside( _objs[ objList[left_buf] ]->_bvh->_rootNode->getBox().center() ) ) {
			left_buf++ ;
		} else {
			UINT temp = objList[left_buf] ;
			objList[left_buf] = objList[right_buf] ;
			objList[right_buf] = temp ;
			right_buf-- ;
		}
	}

	if ( left_buf == 0 || left_buf == numObj ) {
		left_buf = numObj/2 ;
	}


	parentNode->setLeftChild( rebuilObjBVH(objList, left_buf) ) ; 
	parentNode->setRightChild( rebuilObjBVH(objList + left_buf , numObj - left_buf ) ) ;

	return parentNode ;

}