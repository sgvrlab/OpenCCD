/************************************************************************
*								BVH_Node.cpp
************************************************************************
* Author		: Duksu Kim (bluekds@tclab.kaist.ac.kr)
* Affiliation	: SGLAB(http://sglab.kaist.ac.kr), Dept. of Computer Science(http://cs.kaist.ac.kr) , KAIST(http://www.kaist.ac.kr)
* Version		: 0.5
* Create		: 2009/02/23
* Last update	: 2009/03/10
************************************************************************/
#include "CCD_Object.h"
#include "BVH_Node.h"
#include "Geometry/aap.h"
#include "Utility/ccd_vf.h"

BVH_Node::BVH_Node( int type /*= BVH_NODE_TYPE_MID*/ ){

	_nodeType	= type	 ;

	_leftChild	= NULL	;
	_rightChild	= NULL	;

	_triList	= NULL	;
}

BVH_Node::BVH_Node( void ){
	_leftChild	= NULL	;
	_rightChild	= NULL	;

	_triList	= NULL	;
}

BVH_Node::~BVH_Node( void ){

}

void BVH_Node::semi_init( int type /*= BVH_NODE_TYPE_INTER */ ){
	if ( _nodeType == BVH_NODE_TYPE_LEAF ){
		_leftChild = NULL ;
		_rightChild = NULL ;
	}

	_nodeType	= type	 ;
	_box.empty() ;
	_triList = NULL ;
	_numTris = 0	;
}

/************************************************************************/
/* Structure                                                            */
/************************************************************************/
void BVH_Node::refit( void ){
	if ( isLeaf() ) {
		_box = _pObject->getTriBox( _triID ) ;
		return ;
	}

	_leftChild->refit() ;
	_rightChild->refit() ;

	_box = _leftChild->getBox() + _rightChild->getBox() ;
}

BVH_Node* BVH_Node::rebuild( CCD_Object* object, UINT* triList, UINT numTris ){

	_triList = triList ;
	_numTris =  numTris ;

	//************ Case 1 ************// leaf node
	if ( _numTris == 1 ) {
		clear() ;
		_box	= object->getTriBox( _triList[0] ) ;
		_triID = _triList[0] ;
		_pObject = object	;
		_nodeType = BVH_NODE_TYPE_LEAF ;
		return this ;
	}

	//************ Case 2 ************// Intermediate nodes

	// make rough BV ( by uniform sampling )
	_box.empty() ;
	for ( UINT i = 0 ; i < numTris ; i+=5 ){
		_box += object->getTriBox( _triList[i] ) ;
	}

	aap plan(_box) ;
	UINT left_buf = 0, right_buf = numTris - 1 ;

	// divide
	for ( UINT i = 0 ; i < numTris ; i++ ){
		if ( plan.inside( object->getTriBox( _triList[left_buf] ).center() ) ) {
			left_buf++ ;
		} else {
			UINT temp = _triList[left_buf] ;
			triList[left_buf] = _triList[right_buf] ;
			_triList[right_buf] = temp ;
			right_buf-- ;
		}
	}

	if ( left_buf == 0 || left_buf == numTris ) {
		left_buf = numTris/2 ;
	}

	if ( _leftChild == NULL ) {
		_leftChild = new BVH_Node( BVH_NODE_TYPE_INTER ) ;
	} else {
		_leftChild->semi_init( BVH_NODE_TYPE_INTER ) ;
	}

	if ( _rightChild == NULL ) {
		_rightChild = new BVH_Node( BVH_NODE_TYPE_INTER ) ;
	} else {
		_rightChild->semi_init( BVH_NODE_TYPE_INTER ) ;
	}

	_leftChild->rebuild(object, _triList, left_buf ) ;
	_rightChild->rebuild(object, _triList + left_buf, numTris - left_buf ) ;

	_box		= _leftChild->_box + _rightChild->_box; 

	return this ;
}

void BVH_Node::clear( void ){
	if ( isLeaf() )
		return ;

	if ( _leftChild != NULL ){
		_leftChild->clear()	;
		delete _leftChild	;
	}

	if ( _rightChild != NULL ){
		_rightChild->clear() ;
		delete _rightChild	;
	}
}

bool BVH_Node::clearObjNode( void ){
	if ( _nodeType != BVH_NODE_TYPE_OBJECT )
		return false ;

	if ( _leftChild != NULL ){
		if ( _leftChild->clearObjNode() )
			delete _leftChild	;
	}

	if ( _rightChild != NULL ){
		if ( _rightChild->clearObjNode() )
			delete _rightChild	;
	}

	return true ;
}

/************************************************************************/
/* CCD                                                                  */
/************************************************************************/
void BVH_Node::selfCollide( void ){
	if ( isLeaf() || _nodeType == BVH_NODE_TYPE_ROOT_STATIC )
		return ;
	
	_leftChild->collide( _rightChild ) ;

	_leftChild->selfCollide() ;
	_rightChild->selfCollide() ;

}

void BVH_Node::collide( BVH_Node* target ){
	if ( _box.overlaps(target->getBox()) ) {
		if ( isLeaf() ){
			leafCollide( target ) ;
			return ;
		} else {
			_leftChild->collide( target )	;
			_rightChild->collide( target )	;
		}
	}
}

bool BVH_Node::leafCollide( BVH_Node* target ){

	if ( target->isLeaf() ) {

		Vec3f qi, baryc ;
		float ret ;

		tri3f triA, triB ;
		triA = _pObject->_tris[_triID ] ;
		triB = target->_pObject->_tris[target->getTriID()] ;

		// VF Test
		for ( int i = 0 ; i < 3 ; i++ ) {

			// face A - vertex B
			ret = Intersect_VF(
			_pObject->_prev_vtxs[ triA.id(0) ]
			, _pObject->_prev_vtxs[ triA.id(1) ]
			, _pObject->_prev_vtxs[ triA.id(2) ]
			, _pObject->_cur_vtxs[ triA.id(0) ]
			, _pObject->_cur_vtxs[ triA.id(1) ]
			, _pObject->_cur_vtxs[ triA.id(2) ]
			, target->_pObject->_prev_vtxs[ triB.id(i) ], target->_pObject->_cur_vtxs[ triB.id(i) ]
			, qi, baryc ) ;

			if ( ret > -0.5 ) {
				#ifdef SHOW_COLLIDE_TRIANGLES
				_pObject->_flags[_triID] = true ;
				target->_pObject->_flags[target->getTriID()] = true ;
				#endif

				_pObject->_output->addVF( target->_pObject->getID(), _pObject->getID(), target->getTriID(), _triID, triB.id(i), ret ) ;
			}

			// vertex B - face A
			ret = Intersect_VF(
			target->_pObject->_prev_vtxs[ triB.id(0) ]
			, target->_pObject->_prev_vtxs[ triB.id(1) ]
			, target->_pObject->_prev_vtxs[ triB.id(2) ]
			, target->_pObject->_cur_vtxs[ triB.id(0) ]
			, target->_pObject->_cur_vtxs[ triB.id(1) ]
			, target->_pObject->_cur_vtxs[ triB.id(2) ]
			, _pObject->_prev_vtxs[ triA.id(i) ], _pObject->_cur_vtxs[ triA.id(i) ]
			, qi, baryc ) ;

			if ( ret > -0.5 ) {
				#ifdef SHOW_COLLIDE_TRIANGLES
				_pObject->_flags[_triID] = true ;
				target->_pObject->_flags[target->getTriID()] = true ;
				#endif
				
				_pObject->_output->addVF( _pObject->getID(), target->_pObject->getID(), _triID, target->getTriID(), triA.id(i), ret ) ;
			}
		}

		tri3e trieA, trieB ;
		trieA = _pObject->_tris_edge[_triID] ;
		trieB = target->_pObject->_tris_edge[target->getTriID()] ;

		// EE Test
		for ( int i = 0 ; i < 3 ; i++ ) {
			for ( int j = 0 ; j < 3 ; j++ ) {

				ret = Intersect_EE(
					_pObject->_prev_vtxs[_pObject->_edges[trieA.id(i)].id0()], _pObject->_prev_vtxs[_pObject->_edges[trieA.id(i)].id1()],
					target->_pObject->_prev_vtxs[target->_pObject->_edges[trieB.id(j)].id0()], target->_pObject->_prev_vtxs[target->_pObject->_edges[trieB.id(j)].id1()],
					_pObject->_cur_vtxs[_pObject->_edges[trieA.id(i)].id0()], _pObject->_cur_vtxs[_pObject->_edges[trieA.id(i)].id1()],
					target->_pObject->_cur_vtxs[target->_pObject->_edges[trieB.id(j)].id0()], target->_pObject->_cur_vtxs[target->_pObject->_edges[trieB.id(j)].id1()],
					qi ) ;

				if ( ret > -0.5 ) {
					#ifdef SHOW_COLLIDE_TRIANGLES
					_pObject->_flags[_triID] = true ;
					target->_pObject->_flags[target->getTriID()] = true ;
					#endif

					_pObject->_output->addEE( _pObject->getID(), target->_pObject->getID(),
											_triID, target->getTriID(),
											_pObject->_edges[trieA.id(i)].id0(), _pObject->_edges[trieA.id(i)].id1(),
											target->_pObject->_edges[trieB.id(j)].id0(), target->_pObject->_edges[trieB.id(j)].id1(),
											ret) ;
				}

			} // end for j
		} // end for i

	} else {
		if ( _box.overlaps( target->getLeftChild()->getBox()) ) 
			leafCollide( target->getLeftChild() )	;
		if ( _box.overlaps( target->getRightChild()->getBox()) )
			leafCollide( target->getRightChild() )	;
	}

	return false ;
}

/************************************************************************/
/* Utilities                                                            */
/************************************************************************/
void BVH_Node::visualize( int level ){
	if ( !isLeaf() ) {
		if ( level == 1 )
			_box.visulization() ;
		else {
			_leftChild->visualize( level - 1 ) ;
			_rightChild->visualize( level - 1 ) ;
		}
	} else {
		_box.visulization() ;
	}
}