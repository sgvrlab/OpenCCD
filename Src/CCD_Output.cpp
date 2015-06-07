/************************************************************************
*								CCD_Output.h
************************************************************************
* Author		: Duksu Kim (bluekds@tclab.kaist.ac.kr)
* Affiliation	: SGLAB(http://sglab.kaist.ac.kr), Dept. of Computer Science(http://cs.kaist.ac.kr) , KAIST(http://www.kaist.ac.kr)
* Version		: 0.5
* Create		: 2009/02/18
* Last update	: 2009/03/10
************************************************************************/
#include "CCD_Output.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/
CCD_Output::CCD_Output( void ){
	init() ;
}

CCD_Output::~CCD_Output( void ){
	VFs.clear() ;
	EEs.clear() ;
}

void CCD_Output::init( void ){
	numVFs = 0 ;
	numEEs = 0 ;

	VFs.clear() ;
	VFs.resize( DEFAULT_VECTOR_SIZE ) ;
	EEs.clear() ;
	EEs.resize( DEFAULT_VECTOR_SIZE ) ;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCD_Output::printSummary( void ){
	printf("--------------------------------------\n") ;
	printf("\tCCD result\n") ;
	printf("True VF : %d \n", numVFs) ;
	printf("True EE : %d \n", numEEs ) ;
	printf("--------------------------------------\n") ;
}

int CCD_Output::getIntersetingVFs( intersectingVF* listVFs ){
	listVFs = new intersectingVF[numVFs] ;
	//#pragma omp parallel
	for ( __int64 i = 0 ; i < numVFs ; i++ ) {
		listVFs[i] = VFs[i] ;
	}
	return numVFs ;
}

int CCD_Output::getIntersetingEEs( intersectingEE* listEEs ){
	listEEs = new intersectingEE[numEEs] ;
	//#pragma omp parallel
	for ( __int64 i = 0 ; i < numEEs ; i++ ) {
		listEEs[i] = EEs[i] ;
	}
	return numEEs ;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
void CCD_Output::addVF( UINT objV, UINT objF, UINT tV, UINT tF, UINT vID, float cTime ){
	if ( VFs.size() <= numVFs )
		VFs.resize( VFs.size() + DEFAULT_VECTOR_SIZE ) ;

	VFs[numVFs].objVtx		= objV ;
	VFs[numVFs].objFace		= objF ;
	VFs[numVFs].triVtx		= tV ;
	VFs[numVFs].triFace		= tF ;
	VFs[numVFs].indexVtx	= vID ;
	VFs[numVFs].collisionTime	= cTime ;

	numVFs++ ;
}

void CCD_Output::addEE( UINT obj1, UINT obj2, UINT t1, UINT t2, UINT e1_ID0, UINT e1_ID1, UINT e2_ID0, UINT e2_ID1, float cTime ){
	if ( EEs.size() <= numEEs )
		EEs.resize( EEs.size() + DEFAULT_VECTOR_SIZE ) ;

	EEs[numEEs].obj1			= obj1		;
	EEs[numEEs].obj2			= obj2		;
	EEs[numEEs].tri1			= t1		;
	EEs[numEEs].tri2			= t2		;
	EEs[numEEs].indexEdge1[0]	= e1_ID0	;
	EEs[numEEs].indexEdge1[1]	= e1_ID1	;
	EEs[numEEs].indexEdge2[0]	= e2_ID0	;
	EEs[numEEs].indexEdge2[1]	= e2_ID1	;
	EEs[numEEs].collisionTime	= cTime		;

	numEEs++ ;

}