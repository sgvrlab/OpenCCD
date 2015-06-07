/************************************************************************
*								BVH_Node.h
************************************************************************
* Author		: Duksu Kim (bluekds@tclab.kaist.ac.kr)
* Affiliation	: SGLAB(http://sglab.kaist.ac.kr), Dept. of Computer Science(http://cs.kaist.ac.kr) , KAIST(http://www.kaist.ac.kr)
* Version		: 0.5
* Create		: 2009/02/23
* Last update	: 2009/03/10
************************************************************************/
#pragma once

#include <vector>
#include "typeDef.h"
#include "Geometry/box.h"
#include "CCD_output.h"

class CCD_Object ;

#define		BVH_NODE_TYPE_OBJECT		-1
#define		BVH_NODE_TYPE_ROOT			0
#define		BVH_NODE_TYPE_ROOT_STATIC	1
#define		BVH_NODE_TYPE_INTER			2
#define		BVH_NODE_TYPE_LEAF			3

using namespace std ;

class BVH_Node {
public:

	/************************************************************************/
	/* Friends                                                              */
	/************************************************************************/
	friend class BVH ;
	friend class BVH_Node ;
	friend class CCD ;

private:

	/************************************************************************/
	/* Attributes                                                           */
	/************************************************************************/
	int	_nodeType	;		// 4 byte
	BOX		_box	;		// 24 byte

	// for lazy evaluation
	union {	// 8 byte 
		struct {	// low level node
			UINT*	_triList ;
			UINT	_numTris ;
		};
		struct {	// high level node
			UINT*	_objList ;
			UINT	_numObj	 ;
		};
	};

	// 8 byte
	union {
		struct {	// not leaf node
			BVH_Node*	_leftChild	;
			BVH_Node*	_rightChild	;
		};
		struct {	// leaf node
			CCD_Object*	_pObject	;
			UINT		_triID		;
		} ;
	};

public:

	BVH_Node(void) ;
	BVH_Node( int type ) ;
	~BVH_Node(void)	;

	void semi_init ( int type = BVH_NODE_TYPE_INTER ) ; 

	/************************************************************************/
	/* Node type                                                            */
	/************************************************************************/
	FORCEINLINE void setType ( int type ) {
		_nodeType = type ;
	}
	FORCEINLINE int getType ( void ) {
		return _nodeType ;
	}

	/************************************************************************/
	/* Box                                                                  */
	/************************************************************************/
	FORCEINLINE void setBox ( BOX box ) {
		_box = box ;
	}
	FORCEINLINE BOX getBox ( void ) {
		return _box ;
	}

	/************************************************************************/
	/* Set child                                                            */
	/************************************************************************/
	FORCEINLINE void setLeftChild( BVH_Node* child ) {
		_leftChild = child ;
	}
	FORCEINLINE void setRightChild( BVH_Node* child ) {
		_rightChild = child ;
	}
	FORCEINLINE void setChilds( BVH_Node* lChild, BVH_Node* rChild ) {
		_leftChild = lChild ; _rightChild = rChild ;
	}

	/************************************************************************/
	/* Get child                                                            */
	/************************************************************************/
	FORCEINLINE BVH_Node* getLeftChild (void){
		return _leftChild ;
	}
	FORCEINLINE BVH_Node* getRightChild (void){
		return _rightChild ;
	}

	/************************************************************************/
	/* Leaf                                                                 */
	/************************************************************************/
	FORCEINLINE bool isLeaf ( void ) {
		return ( _nodeType == BVH_NODE_TYPE_LEAF ? true : false ) ;
	}
	FORCEINLINE void setObject( CCD_Object* object ) {
		_pObject = object ;
	}
	FORCEINLINE void setTriID ( UINT id ) {
		_triID = id ;
	}
	FORCEINLINE UINT getTriID ( void ) {
		return _triID ;
	}

	/************************************************************************/
	/* Structure                                                            */
	/************************************************************************/
	void refit ( void )		;
	BVH_Node* rebuild ( CCD_Object* object, UINT* triList, UINT numTris ) ;
	void clear (void ) ;
	bool clearObjNode ( void ) ;

	/************************************************************************/
	/* CCD                                                                  */
	/************************************************************************/
	void collide ( BVH_Node* target )		;
	bool leafCollide ( BVH_Node* target )	;
	void selfCollide ( void ) ;

	/************************************************************************/
	/* Utilities                                                            */
	/************************************************************************/
	void visualize ( int level ) ;
} ;
