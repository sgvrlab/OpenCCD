/************************************************************************
*								BVH.h
************************************************************************
* Author		: Duksu Kim (bluekds@tclab.kaist.ac.kr)
* Affiliation	: SGLAB(http://sglab.kaist.ac.kr), Dept. of Computer Science(http://cs.kaist.ac.kr) , KAIST(http://www.kaist.ac.kr)
* Version		: 0.1 ( Initail version )
* Create		: 2009/02/23
* Last update	: 2009/02/23
************************************************************************/
#pragma once

#include "BVH_Node.h"

class CCD_Object ;

class BVH
{
	friend class CCD ;
	friend class BVH_Node ;

private:
	CCD_Object*	_pObject	;
	int			_numTris	;
	BVH_Node*	_rootNode	;
	UINT*		_triList	;

public:
	BVH(){}
	BVH( CCD_Object* object, UINT numTri ) ;
	~BVH(void);

	FORCEINLINE void refit( void )	{ _rootNode->refit() ; }
	FORCEINLINE void rebuild( ){
		_rootNode->rebuild( _pObject, _triList, _numTris ) ;
	}

	FORCEINLINE  void visualize( int level ) {
		_rootNode->visualize( level ) ;
	}
} ;
