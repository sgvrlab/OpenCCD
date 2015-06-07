/************************************************************************
*								BVH.cpp
************************************************************************
* Author		: Duksu Kim (bluekds@tclab.kaist.ac.kr)
* Affiliation	: SGLAB(http://sglab.kaist.ac.kr), Dept. of Computer Science(http://cs.kaist.ac.kr) , KAIST(http://www.kaist.ac.kr)
* Version		: 0.1 ( Initail version )
* Create		: 2009/02/23
* Last update	: 2009/02/23
************************************************************************/
#include "BVH.h"
#include "CCD_Object.h"

BVH::BVH( CCD_Object* object, UINT numTri ){
	_pObject = object ;
	_numTris = numTri ;
	_rootNode = new BVH_Node() ;
	_triList = new UINT[numTri] ;

	for ( UINT i = 0 ; i < _numTris ; i++ )
		_triList[i] = i ;

	if ( object->getProperty(CCD_OBJECT_PROPERTY_OBJECT_TYPE) == CCD_OBJECT_TYPE_STATIC ) {
		_rootNode->_nodeType = BVH_NODE_TYPE_ROOT_STATIC ;
	} else {
		_rootNode->_nodeType = BVH_NODE_TYPE_ROOT ;
	}
}

BVH::~BVH(void){
	delete _rootNode ;
	delete _triList	;
}
