/************************************************************************
*								ObjectLoader.h
************************************************************************
* Author		: Duksu Kim (bluekds@tclab.kaist.ac.kr)
* Affiliation	: SGLAB(http://sglab.kaist.ac.kr), Dept. of Computer Science(http://cs.kaist.ac.kr) , KAIST(http://www.kaist.ac.kr)
* Version		: 0.1 ( Initail version )
* Create		: 2009/02/22
* Last update	: 2009/02/22
************************************************************************/
#pragma once

#include "Geometry/plyfile.h"
#include "CCD_Object.h"

/**
* Read object(s) from ply files
* @param path		The path of the folder that contains ply files
* @param num_frame	The number of frames
* @param scale		A scale factor that will be applied to the object(s)
* @return			A pointer of created CCD_Object
*/
CCD_Object* ply2Object( char *path, UINT num_frame, float scale, UINT objType ) ;