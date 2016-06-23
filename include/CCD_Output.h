/************************************************************************
*								CCD_Output.h
************************************************************************
* Author		: Duksu Kim (bluekds@tclab.kaist.ac.kr)
* Affiliation	: SGLAB(http://sglab.kaist.ac.kr), Dept. of Computer Science(http://cs.kaist.ac.kr) , KAIST(http://www.kaist.ac.kr)
* Version		: 0.5
* Create		: 2009/02/18
* Last update	: 2009/03/10
************************************************************************/

#pragma once

#include "typeDef.h"
#include <vector>

#define		DEFAULT_VECTOR_SIZE		100 

using namespace std ;

/**
* A simulation output class.
* It maintain results of the last simulation.
* @author	Duksu Kim ( bluekds@tclab.kaist.ac.kr )
* @author	SGLAB, KAIST ( http://sglab.kaist.ac.kr )
* @version	0.5
*/
class CCD_Output {
private :
	UINT	numVFs ;
	UINT	numEEs	;

	vector<intersectingVF>	VFs ;
	vector<intersectingEE>	EEs ;

public :

	CCD_Output(void) ;
	~CCD_Output(void) ;

	/**
	* Initialize results
	*/
	void init ( void ) ;

	/**
	* Show the summary of the last simulation's results
	*/
	void printSummary ( void ) ;

	/**
	* Get the number of intersecting VF of last simulation
	* @return The number of of intersecting VF pairs
	*/
	FORCEINLINE int getNumVF (void) { return numVFs ; }

	/**
	* Get the number of intersecting EE of last simulation
	* @return The number of of intersecting EE pairs
	*/
	FORCEINLINE int getNumEE (void) { return numEEs ; }

	/**
	* Get the list of intersecting Vertex-Face pairs. <br>
	* This method allocate memory dynamically to the parameter, listVFs. <br>
	* User must released memory explicitly after using it.
	* @param listVFs	The pointer indicating the first address of array, containing the list of intersecting VF pairs
	* @return			The number of of intersecting VF pairs
	*/
	int getIntersetingVFs ( intersectingVF* &listVFs ) ;

	/**
	* Get the list of intersecting Edge-Edge pairs.<br>
	* This method allocate memory dynamically to the parameter, listEEs.<br>
	* User must released memory explicitly after using it.
	* @param listVFs	The pointer indicating the first address of array, containing the list of intersecting EE pairs
	* @return			The number of of intersecting EE pairs
	*/
	int getIntersetingEEs ( intersectingEE* &listEEs ) ;

	/************************************************************************/
	/* Add results                                                          */
	/************************************************************************/
	
	/**
	* Add VF intersecting result
	* @param	objV	the ID of a object containing collided vertex
	* @param	objF	the ID of a object containing Face
	* @param	triV	the ID of a triangle containing collided vertex
	* @param	triF	the ID of a face(triangle)
	* @param	vID		the ID of a vertex
	* @param	cTime	the collision time ( 0 ~ 1.0 )
	*/
	void addVF ( UINT objV, UINT objF, UINT triV, UINT triF, UINT vID, float cTime ) ;

	/**
	* Add EE intersecting result
	* @param	obj1	the ID of a object1
	* @param	obj2	the ID of a object2
	* @param	tri1	the ID of a triangle containing edge1
	* @param	tri2	the ID of a triangle containing edge2
	* @param	e1_ID0	the ID of the first vertex of edge1
	* @param	e1_ID1	the ID of the second vertex of edge1
	* @param	e2_ID0	the ID of the first vertex of edge2
	* @param	e2_ID1	the ID of the second vertex of edge1
	* @param	cTime	the collision time ( 0 ~ 1.0 )
	*/
	void addEE ( UINT obj1, UINT obj2, UINT tri1, UINT tri2, UINT e1_ID0, UINT e1_ID1, UINT e2_ID0, UINT e2_ID1, float cTime ) ;
};