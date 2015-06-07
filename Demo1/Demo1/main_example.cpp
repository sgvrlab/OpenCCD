#include <OpenCCD.h>

void main ( void ) {
	CCD ccd ;
	CCD_Output output ;
	CCD_Object obj1, obj2 ;

	// Assign output instance to CCD
	ccd.setOutput( &output ) ;

	// set object 1
	obj1.beginObject(2, 3, 1, CCD_OBJECT_TYPE_STATIC ) ;
		// frame 1
		obj1.setVtx(0, 0, Vec3f( 0, 0, 0 ) ) ;
		obj1.setVtx(0, 1, Vec3f( 0, 0, 1 ) ) ;
		obj1.setVtx(0, 2, Vec3f( 0, 1, 1 ) ) ;

		// frame 2
		obj1.setVtx(1, 0, Vec3f( 0, 0, -1 ) ) ;
		obj1.setVtx(1, 1, Vec3f( 0, 0, 0  ) ) ;
		obj1.setVtx(1, 2, Vec3f( 0, 1, 0  ) ) ;

		obj1.setTri( 0, 0, 1, 2 ) ;
	obj1.endObject() ;

	// set object 2
	obj2.beginObject(2, 3, 1, CCD_OBJECT_TYPE_STATIC ) ;
		// frame 1
		obj2.setVtx(0, 0, Vec3f( -10, 0, -0.5 ) ) ;
		obj2.setVtx(0, 1, Vec3f( 0, 10, -0.5 ) ) ;
		obj2.setVtx(0, 2, Vec3f( 10, 0, -0.5 ) ) ;

		// frame 2
		obj2.setVtx(1, 0, Vec3f( -10, 0, -0.5 ) ) ;
		obj2.setVtx(1, 1, Vec3f( 0, 10, -0.5 ) ) ;
		obj2.setVtx(1, 2, Vec3f( 10, 0, -0.5 ) ) ;

		obj2.setTri( 0, 0, 1, 2 ) ;
	obj2.endObject() ;

	// Add objects to CCD
	obj1.setID ( ccd.addObject( &obj1 ) );
	obj2.setID ( ccd.addObject( &obj2 ) );

	// Ready to perform CCD
	ccd.readyCCD( 0, 2, 0 ) ;

	// Perform CCD
	for ( int i = 0 ; i < 3 ; i++ ) {
		ccd.performCCD() ;
		ccd.nextFrame() ;
	}

	// Print results
	output.printSummary() ;

	//CCD ccd ;
	//CCD_Output output ;
	//CCD_Object obj1, obj2 ;

	//// Assign output instance to CCD
	//ccd.setOutput( &output ) ;

	//// set object 1
	//obj1.beginObject(1, 3, 1, CCD_OBJECT_TYPE_STATIC ) ;
	//	obj1.setVtx(0, 0, Vec3f( 0, 0, 0 ) ) ;
	//	obj1.setVtx(0, 1, Vec3f( 0, 0, 1 ) ) ;
	//	obj1.setVtx(0, 2, Vec3f( 0, 1, 1 ) ) ;

	//	obj1.setTri( 0, 0, 1, 2 ) ;
	//obj1.endObject() ;

	//// set object 2
	//obj2.beginObject(1, 3, 1, CCD_OBJECT_TYPE_STATIC ) ;
	//	obj2.setVtx(0, 0, Vec3f( -10, 0, -0.5 ) ) ;
	//	obj2.setVtx(0, 1, Vec3f( 0, 10, -0.5 ) ) ;
	//	obj2.setVtx(0, 2, Vec3f( 10, 0, -0.5 ) ) ;

	//	obj2.setTri( 0, 0, 1, 2 ) ;
	//obj2.endObject() ;

	//// Add objects to CCD
	//obj1.setID ( ccd.addObject( &obj1 ) );
	//obj2.setID ( ccd.addObject( &obj2 ) );

	//// Ready to perform CCD
	//ccd.readyCCD( 0, 1, 0 ) ;

	//// Apply model changes
	//obj1.swapVtxs_Cur_Prev() ;
	//	obj1.setCurVtx(0, Vec3f( 0, 0, -1 ) ) ;
	//	obj1.setCurVtx(1, Vec3f( 0, 0, 0  ) ) ;
	//	obj1.setCurVtx(2, Vec3f( 0, 1, 0  ) ) ;

	//obj2.swapVtxs_Cur_Prev() ;
	//	obj2.setCurVtx(0, Vec3f( -10, 0, -0.5 ) ) ;
	//	obj2.setCurVtx(1, Vec3f( 0, 10, -0.5 ) ) ;
	//	obj2.setCurVtx(2, Vec3f( 10, 0, -0.5 ) ) ;

	//// perform CCD
	//ccd.performCCD() ;

	//// Print results
	//output.printSummary() ;
}