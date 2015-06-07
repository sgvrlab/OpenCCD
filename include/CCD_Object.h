/************************************************************************
*								CCD_Object.h
************************************************************************
* Author		: Duksu Kim (bluekds@tclab.kaist.ac.kr)
* Affiliation	: SGLAB(http://sglab.kaist.ac.kr), Dept. of Computer Science(http://cs.kaist.ac.kr) , KAIST(http://www.kaist.ac.kr)
* Version		: 0.1 ( Initail version )
* Create		: 2009/02/18
* Last update	: 2009/03/10
************************************************************************/

#pragma once

#include "Geometry/box.h"
#include "typeDef.h"
#include "BVH.h"

#define		CCD_OBJECT_TYPE_MAX			2
#define		CCD_OBJECT_TYPE_STATIC		1
#define		CCD_OBJECT_TYPE_DEFORMABLE	2

#define		CCD_OBJECT_STATE_INIT	0
#define		CCD_OBJECT_STATE_BEGIN	1
#define		CCD_OBJECT_STATE_END	2

#define		CCD_OBJECT_PROPERTY_OBJECT_TYPE		0
#define		CCD_OBJECT_PROPERTY_NUM_FRAME		1
#define		CCD_OBJECT_PROPERTY_NUM_VERTEX		2
#define		CCD_OBJECT_PROPERTY_NUM_TRIANGLE	3
#define		CCD_OBJECT_PROPERTY_NUM_EDGE		4
#define		CCD_OBJECT_PROPERTY_STATE			5


/**
* A object class.
* It maintain information of a object.
* @author	Duksu Kim ( bluekds@tclab.kaist.ac.kr )
* @author	SGLAB, KAIST ( http://sglab.kaist.ac.kr )
* @version	0.1
*/
class CCD_Object {
	/************************************************************************/
	/* friends                                                              */
	/************************************************************************/
	friend class CCD		;	// Set CCD class as a friend
	friend class BVH_Node	;	// Set BVH_Node class as a friend 
	friend class BVH		;	// Set BVH class as a friend 

private:
	int	_ID				;
	int _curFrame		;

	// Properties
	UINT	_objType	;	// Object type ( CCD_OBJECT_TYPE_STATIC, CCD_OBJECT_TYPE_DEFORMABLE )

	UINT	_numFrames	;	// The number of key frames
	UINT	_numVtxs	;	// The number of vertices composing this object
	UINT	_numTris	;	// The number of triangles composing this object
	UINT	_numEdge	;	// The number of edges composing this object

	// 
	Vec3f*	_vtxs		;	// list of vertices of key frames
	tri3f*	_tris		;	// list of triangles

	edge2v* _edges		;	// list of edges
	tri3e*	_tris_edge	;	// list of triangles havind edge Info.

	Vec3f*	_cur_vtxs	;	// Vertex list of current frame
	Vec3f*	_prev_vtxs	;	// Vertex list of previous frame

	BOX*	_tris_box	;	// BVs of triangles

	mat4f	_transMat	;	// Matrix(4x4) that represent transformation

	int		_state		;	// 0 : before begin, 1: begin,	2: end'

	BVH*	_bvh		;	// BVH of the object

	#ifdef SHOW_COLLIDE_TRIANGLES
	bool*	_flags		;
	#endif

	// results
	CCD_Output*	_output ;

	/**
	* It is used for generating edges by using current Info. of this object.
	*/
	void generateEdges(void) ;

	void refitTriBox( void ) ;

public:

	CCD_Object(void)	;
	~CCD_Object(void)	;

	/**
	* Set object ID
	* @return	Object ID
	*/
	int getID() const { return _ID; }

	/**
	* Get object ID
	* @param val Object ID
	*/
	void setID(int val) { _ID = val; }

	/**
	* Set the basic property of the objects.
	* @param numFrame	the number of frames
	* @param numVtx		the number of vertices
	* @param numTri		the number of triangles
	* @param objType	a type of the object ( CCD_OBJECT_TYPE_STATIC, CCD_OBJECT_TYPE_DEFORMABLE )
	*/
	void beginObject( UINT numFrame, UINT numVtx, UINT numTri, UINT objType ) ;

	/**
	* Assemble information of object.<br>
	* It should be called after inputing all information of vetices and triangles.
	*/
	void endObject ( void ) ;

	/**
	* Set position of a indexed vertex
	* @param frame	the index of frame including the vertex that will be set
	* @param index	the index of vertex
	* @param pos	the position of the vertex
	*/
	void setVtx( UINT frame, UINT index, Vec3f pos ) ;

	/**
	* Set current position of a indexed vertex
	* @param index	the index of vertex
	* @param pos	the position of the vertex
	*/
	void setCurVtx ( UINT index, Vec3f pos ) ;
	/**
	* Set current position of vertexes
	* @param vtxs	array of vertexes
	*/
	void setCurVtx ( Vec3f* vtxs ) ;

	/**
	* Copy current vertexes to previous vertexes
	*/
	void swapVtxs_Cur_Prev ( void ) ;

	/**
	* Set previous position of a indexed vertex
	* @param index	the index of vertex
	* @param pos	the position of the vertex
	*/
	void setPrevVtx ( UINT index, Vec3f pos ) ;
	/**
	* Set previous position of vertexes
	* @param vtxs	array of vertexes
	*/
	void setPrevVtx ( Vec3f* vtxs ) ;

	/**
	* Set three vertex's indexes composing the indexed triangle
	* @param index	the index of the triangle
	* @param a		the first vertex
	* @param b		the second vertex
	* @param c		the third vertex
	*/
	void setTri( UINT index, UINT a, UINT b, UINT c ) ;

	/**
	* Set the transformation matrix
	* @param transMat	Transformation matrix applying to the object
	*/
	void setTransformationMatrix( mat4f transMat ) ;

	/**
	* Set property
	* @param prop	The property which will be set ( CCD_PROPERTY_[the name of property] )
	* @param value	The value will be assigned
	* @return		true : success, false : fail
	*/
	bool setProperty( UINT prop, UINT value ) ;

	/**
	* Set property
	* @param prop	The property what want to know ( CCD_PROPERTY_[the name of property] - see the header file "CCD_Object.h" )
	* @return		The value of the property <br> 0 : fail to get the property
	*/
	UINT getProperty( UINT prop ) ;

	/**
	* Show information of the object. <br>
	* Information includes <br>
	* 1. Type and state
	* 2. The number of vertex, triangles, and frame <br>
	* 3. Transformation matrix <br>
	* 4. Memory usage
	*/
	void printObjectInformation ( void ) ;

	/**
	* Set the number of edges and allocate memory to store edge Info.<br>
	* @param numFrame	The number of edges
	*/
	void initEdges( UINT numEdges) ;
	/**
	* Set two vertex's indexes composing the indexed triangle
	* @param index	the index of the edge	
	* @param a		Id0 ( vertex index )
	* @param b		Id1 ( vertex index ) 
	*/
	void setEdge( UINT index, UINT a, UINT b ) ;

	/**
	* Set three edge's indexes composing the indexed triangle
	* @param index	the index of the triangle
	* @param a		the first edge
	* @param b		the second edge
	* @param c		the third edge
	*/
	void setTrisEdge (  UINT index, UINT a, UINT b, UINT c ) ;

	/**
	* Set a BV of the indicated triangle
	* @param index	The index of a triangle
	* @param box	A BV to apply
	*/
	void setBox ( UINT index, BOX box ) ;

	/**
	* Get a BV of the indicated triangle
	* @param index	The index of a triangle
	* @return		A BV of the triangle
	*/
	BOX	getTriBox ( UINT index ) ;

	/**
	* Move to next frame if this object has information of next frame.
	*/
	void nextFrame( void ) ;

	/**
	* Set the CCD_Ouput instance that is used for collect the result of CCD.
	* @param	output	The pointer of CCD_Output class
	*/
	inline void setOutput( CCD_Output* output ) {
		_output = output ;
	}

	/************************************************************************/
	/* Utilities                                                            */
	/************************************************************************/

	/**
	* If you want to visualize BVHs, you can call this method at the display part of openGL
	* @param level	The BV level which you want to see
	*/
	void visualizeBVH( int level ) {
		_bvh->visualize( level ) ;
	}

	/**
	* If you want to visualize this object, you can call this method at the display part of openGL
	*/
	void visualizeObject( void	) ;
} ;