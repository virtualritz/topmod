/*** ***/


/* Rough outline of planarization algorithm */
#include <DLFLCore.h>
#include <DLFLCoreExt.h>
#include <cmath>
#include <cassert>

namespace DLFL {

  void meshsmooth( DLFLObjectPtr obj ) {
    DLFLFacePtrList::iterator fl_first, fl_last;
    DLFLFacePtr faceptr;
  
    fl_first = obj->beginFace(); fl_last = obj->endFace();

    /* Loop through all faces */
    while ( fl_first != fl_last ) {
      faceptr = (*fl_first); ++fl_first;

      /*
	Find the average normal for the face and the centroid
	Store these in the member variables of the face for later use
      */
      faceptr->normal = faceptr->computeNormal();
      faceptr->centroid = faceptr->geomCentroid();

      /*
	If the normal and point to define the plane are calculated
	using some other method, calculate them and store them in
	the 'normal' and 'centroid' member variables in the face
      */
    }


    DLFLVertexPtrList::iterator vl_first, vl_last;
    DLFLVertexPtr vertexptr;
    DLFLFaceVertexPtrArray fvparray;
    Vector3dArray p_array, n_array; // Arrays of point and normals for each face adjacent to a vertex
    int num_faces;
    Vector3d average, averagenormal; 
  
    vl_first = obj->beginVertex(); vl_last = obj->endVertex();

    /* Loop through all vertices */
    while ( vl_first != vl_last ) {
      vertexptr = (*vl_first); ++vl_first;

      /*
	Get all the corners for this vertex
      */
      fvparray.clear(); // The local array must be cleared before calling getFaceVertices
      vertexptr->getFaceVertices(fvparray);
      num_faces = fvparray.size();
       
      /*
	Go through the array of face-vertices and find the normal and centroid
	for each face
      */
      p_array.clear(); n_array.clear(); // The local arrays must be cleared for each Vertex

      // If we resize the array before filling it, we can avoid unnecessary memory allocation
      // thus speeding up the process a little bit.
      //p_array.resize(num_faces,Vector3d(0)); n_array.resize(num_faces,Vector3d(0));
      p_array.resize(num_faces,Vector3d()); n_array.resize(num_faces,Vector3d());
      for (int i=0; i <num_faces; ++i) {
	DLFLFacePtr fp = fvparray[i]->getFacePtr();

	// Get the normal and centroid for this face (which we calculated above)
	// and store them in our local arrays
	n_array[i] = fp->normal;
	p_array[i] = fp->centroid;
	average+=p_array[i];
	averagenormal+=n_array[i];
	    
      }
      average/=num_faces; normalize(averagenormal);
      Vector3d new_pos = vertexptr->coords; // This is the old position

      new_pos+=(average-new_pos)-((average-new_pos)*averagenormal)*averagenormal;
               
      vertexptr->coords = new_pos;
      average-=average; 
    }
  }

} // end namespace
