/************************************************************************
*								ObjectLoader.cpp
************************************************************************
* Author		: Duksu Kim (bluekds@tclab.kaist.ac.kr)
* Affiliation	: SGLAB(http://sglab.kaist.ac.kr), Dept. of Computer Science(http://cs.kaist.ac.kr) , KAIST(http://www.kaist.ac.kr)
* Version		: 0.5
* Create		: 2009/02/22
* Last update	: 2009/03/10
************************************************************************/
#include <list>
#include <vector>
#include <assert.h>
#include <algorithm>
#include "ObjectLoader.h"

using namespace std ;

/************************************************************************/
/* Data structure                                                       */
/************************************************************************/
// Data structure for PLY
typedef struct PLYVertex{
	float coords[3];
	unsigned char color[3];
	void *other_props;		
} PLYVertex;

typedef struct PLYFace{
	unsigned char nverts;
	int *verts;		
	void *other_props;
} PLYFace;

/************************************************************************/
/* pre-defined variables                                                */
/************************************************************************/
static PlyProperty vert_props[] = { /* list of property information for a vertex */
	{"x", PLY_FLOAT, PLY_FLOAT, 0, 0, 0, 0, 0},
	{"y", PLY_FLOAT, PLY_FLOAT, 4, 0, 0, 0, 0},
	{"z", PLY_FLOAT, PLY_FLOAT, 8, 0, 0, 0, 0},
	{"red", PLY_UCHAR, PLY_UCHAR, (int)offsetof(PLYVertex,color[0]), 0, 0, 0, 0},
	{"green", PLY_UCHAR, PLY_UCHAR, (int)offsetof(PLYVertex,color[1]), 0, 0, 0, 0},
	{"blue", PLY_UCHAR, PLY_UCHAR, (int)offsetof(PLYVertex,color[2]), 0, 0, 0, 0},
};

static PlyProperty face_props[] = { /* list of property information for a vertex */
	{"vertex_indices", PLY_INT, PLY_INT, offsetof(PLYFace,verts), 1, PLY_UCHAR, PLY_UCHAR, offsetof(PLYFace,nverts)},
};

CCD_Object* ply2Object( char *fname, UINT num_frame, float ply_scale, UINT objType ){
	CCD_Object* newObj = new CCD_Object() ;

	char ply_fname[256] ;

	printf("Start to load model..\n") ;

	for (unsigned int cur_f = 0; cur_f < num_frame; cur_f++) {		// for each frame
		sprintf_s(ply_fname, "%s%d.ply", fname, cur_f);

		FILE *fp = fopen(ply_fname, "rb");
		assert(fp);

		PlyFile *ply;	// PLY object:

		char **elist;	// PLY properties
		int nelems;

		// hand over the stream to the ply functions:
		ply = ply_read(fp, &nelems, &elist);
		assert(ply);

		// Begin CCD_object at first frame
		if ( newObj->getProperty(CCD_OBJECT_PROPERTY_STATE ) == CCD_OBJECT_STATE_INIT ) {
			int numVtx, numTris ;

			for (int i=0; i<nelems; i++) {
				if (equal_strings ("vertex", ply->elems[i]->name) ) {
					numVtx = ply->elems[i]->num ;
				}

				if (equal_strings ("face", ply->elems[i]->name) ) {
					numTris = ply->elems[i]->num ;
				}
			}

			newObj->beginObject( num_frame, numVtx, numTris, objType ) ;
		}

		int file_type;
		float version;		
		ply_get_info(ply, &version, &file_type);		// get ply's info

		for (int i=0; i<nelems; i++) {	// for each element
			char *elem_name = elist[i];

			int num_elems, nprops;
			PlyProperty **plist = ply_get_element_description(ply, elem_name, &num_elems, &nprops);

			bool has_vertex_x = false, has_vertex_y = false, has_vertex_z = false, has_colors = false;
			unsigned char color_components = 0;

			/*********************************************************************************/
			// Vertex
			if (equal_strings ("vertex", elem_name)) {
				// read vertices's properties and saved it
				for (int j=0; j<nprops; j++)	// for each properties
				{
					if (equal_strings("x", plist[j]->name))
					{
						ply_get_property (ply, elem_name, &vert_props[0]);  /* x */
						has_vertex_x = true;
					}
					else if (equal_strings("y", plist[j]->name))
					{
						ply_get_property (ply, elem_name, &vert_props[1]);  /* y */
						has_vertex_y = true;
					}
					else if (equal_strings("z", plist[j]->name))
					{
						ply_get_property (ply, elem_name, &vert_props[2]);  /* z */
						has_vertex_z = true;
					}
					else if (equal_strings("red", plist[j]->name))
					{
						ply_get_property (ply, elem_name, &vert_props[3]);  /* z */
						color_components++;
					}
					else if (equal_strings("green", plist[j]->name))
					{
						ply_get_property (ply, elem_name, &vert_props[4]);  /* z */
						color_components++;
					}
					else if (equal_strings("blue", plist[j]->name))
					{
						ply_get_property (ply, elem_name, &vert_props[5]);  /* z */
						color_components++;
					}
				}	// end for j

				has_colors = color_components == 3;

				// test for necessary properties
				if ((!has_vertex_x) || (!has_vertex_y) || (!has_vertex_z))
				{
					// logger part
					printf("Warning: Vertex with less than 3 coordinated detected. Output will most likely be corrupt!");
					continue;
				}

				// grab all the vertex elements
				PLYVertex plyNewVertex ;
				for (int j=0; j<num_elems; j++) {
					// read vertex info
					ply_get_element(ply, (void *)&plyNewVertex);								

					// store vertex of scaled vector
					newObj->setVtx( cur_f, j, Vec3f(plyNewVertex.coords) * ply_scale ) ;

				} // end for j
			}

			/*********************************************************************************/
			// this is a face (and, hopefully, a triangle):
			else if (equal_strings ("face", elem_name) && cur_f == 0 ) {
				// I need this for..., otherwise error ...
				for (int j=0; j<nprops; j++)
				{
					if (equal_strings("vertex_indices", plist[j]->name))
					{
						ply_get_property (ply, elem_name, &face_props[0]);  /* vertex_indices */
					}
				}

				/* grab all the face elements */
				PLYFace plyFace;	
				plyFace.other_props = NULL;			

			//1	list<edge2f> edgelist_temp;		// temporary edge list
				vector<tri3f> trilist_temp;		// temporaray triangle list

				for (int j = 0; j < num_elems; j++) {	// 
					ply_get_element(ply, (void *)&plyFace) ;

					for (int fi = 0; fi < plyFace.nverts-2; fi++) {
						// copy vertex indices
						unsigned int id0, id1, id2;

						// get the IDs of vertices composing a face
						id0 = plyFace.verts[0];
						id1 = plyFace.verts[fi+1];
						id2 = plyFace.verts[fi+2];

						// create triangle consisting with three vertices
						tri3f tri(id0, id1, id2);

						// insert triangle into list
						trilist_temp.push_back(tri);	// total tilist size = frame# * face#
						unsigned int fid = (unsigned int)trilist_temp.size()-1;

						// store edges composing the triangle
						// it can be overlapping
				//1		edgelist_temp.push_back(edge2f(id0, id1, fid));
				//1		edgelist_temp.push_back(edge2f(id1, id2, fid));
				//1		edgelist_temp.push_back(edge2f(id2, id0, fid));
					}
					free(plyFace.verts);												
				}

				//edgelist_temp.sort();	// sort edges

				//// ? asign other fid for shared edges?
				//list<edge2f> edge_unqie;
				//for (list<edge2f>::iterator it=edgelist_temp.begin(); it!=edgelist_temp.end(); it++) {
				//	if (!edge_unqie.empty() && *it == edge_unqie.back()) { // find duplicated with other fid
				//		unsigned int fid = (*it).fid(0);
				//		assert(fid != -1);
				//		edge_unqie.back().set_fid2(fid);
				//	} else
				//		edge_unqie.push_back(*it);
				//}

				//edgelist_temp.clear();
				//vector<edge2f> edge_array;

				//UINT _num_edge = (unsigned int)edge_unqie.size();
				//newObj->initEdges( _num_edge ) ;
				//
				//edge2f tempEdge ;
				unsigned int t=0;
				//for (list<edge2f>::iterator it=edge_unqie.begin(); it != edge_unqie.end(); it++){
				//	//_edges[t++] = *it;
				//	tempEdge = *it ;
				//	newObj->setEdge( t++, tempEdge.vid(0), tempEdge.vid(1) ) ;
				//	edge_array.push_back(*it);
				//}

				//// copy over temp list to static array
				//vector <edge2f>::iterator first = edge_array.begin();
				//vector <edge2f>::iterator last = edge_array.end();
				for (t = 0; t < newObj->getProperty(CCD_OBJECT_PROPERTY_NUM_TRIANGLE) ; t++) {
					newObj->setTri( t, trilist_temp[t].id0(), trilist_temp[t].id1(), trilist_temp[t].id2() ) ;
					//_tris[t] = trilist_temp[t] ;

				//1	vector <edge2f>::iterator it1 = lower_bound(first, last, edge2f(trilist_temp[t].id0(), trilist_temp[t].id1(), 0));
				//1	vector <edge2f>::iterator it2 = lower_bound(first, last, edge2f(trilist_temp[t].id1(), trilist_temp[t].id2(), 0));
				//1	vector <edge2f>::iterator it3 = lower_bound(first, last, edge2f(trilist_temp[t].id2(), trilist_temp[t].id0(), 0));

				//1	newObj->setTrisEdge( t, it1-first, it2-first, it3-first ) ;
					//_tri_edges[t].set(it1-first, it2-first, it3-first);
				}

			//	cout << "Edge # = " << _num_edge << endl;
			//	cout << "Tri # = " << _num_tri << endl;
			}
			/*********************************************************************************/

			else // otherwise: skip all further
				NULL;
		}

		// PLY parsing ended, clean up vertex buffer and close the file		
		ply_close(ply);
		fclose(fp);
	}

//	printf("# of Vtxs : %d, # of edges : %d, # of Tris : %d \n", newObj->getProperty(CCD_OBJECT_PROPERTY_NUM_VERTEX), newObj->getProperty(CCD_OBJECT_PROPERTY_NUM_EDGE), newObj->getProperty(CCD_OBJECT_PROPERTY_NUM_TRIANGLE) ) ;
	printf("# of Vtxs : %d, # of Tris : %d \n", newObj->getProperty(CCD_OBJECT_PROPERTY_NUM_VERTEX), newObj->getProperty(CCD_OBJECT_PROPERTY_NUM_TRIANGLE) ) ;
	newObj->endObject() ;

	return newObj ;
}