/************************************************************************
*								CCD.h
************************************************************************
* Author		: Duksu Kim (bluekds@tclab.kaist.ac.kr)
* Affiliation	: SGLAB(http://sglab.kaist.ac.kr), Dept. of Computer Science(http://cs.kaist.ac.kr) , KAIST(http://www.kaist.ac.kr)
* Version		: 0.5
* Create		: 2009/02/18
* Last update	: 2009/03/10
************************************************************************/

#pragma once

#include "CCD_Object.h"
#include "CCD_Output.h"

using namespace std;
/**
* A CCD class.
* It maintain information for performing CCD.
* @author	Duksu Kim ( bluekds@tclab.kaist.ac.kr )
* @author	SGLAB, KAIST ( http://sglab.kaist.ac.kr )
* @version	0.5
*/
class CCD {

private :
	vector<CCD_Object*>	_objs ;
	CCD_Output*			_output	;
	UINT				_numObjs ;
	UINT*				_objList ;
	BVH_Node*			_bvh	;

	BVH_Node* rebuilObjBVH( UINT* objList, UINT numObj ) ;

	UINT _curFrame	;
	UINT _endFrame	;

public:
	/**
	* Set the CCD_Ouput instance that is used for collect the result of CCD.
	* @param	output	The pointer of CCD_Output class
	*/
	void setOutput ( CCD_Output* output ) {
		_output = output ;
	}

	/**
	* Add a object to the collision test.
	* @param obj	The pointer indicating instance of object class which is added to the collision test.
	* @return		The ID allocating to the object
	*/
	int addObject ( CCD_Object* obj ) ;	

	/**
	* Delete a object from the collision test.
	* @param objID	ID of the object that will be removed from the collision test.
	* @return		The number of objects in CCD
	*/
	int delObject ( int objID ) ;

	/**
	* Ready to perform CCD
	* @param startFrame		The frame number of starting frame
	* @param endFrame		The frame number of final frame
	* @param interpolation	Set the number of in-between frame among two frames
	*/
	void readyCCD ( UINT startFrame, UINT endFrame, UINT interpolation ) ;
	
	/**
	* Perform continuous collision detection
	*/
	void performCCD ( ) ;

	/**
	* Move to next frame if it has next frame.
	*/
	void nextFrame( void ) ;

	/************************************************************************/
	/* Utilities                                                            */
	/************************************************************************/

	/**
	* If you want to visualize BVHs, you can call this method at the display part of openGL
	* @param level	The BV level which you want to see
	*/
	void visualizeBVH( int level ) ;

	/**
	* If you want to visualize objects, you can call this method at the display part of openGL
	*/
	void visualizeObjects( void ) ;
} ;