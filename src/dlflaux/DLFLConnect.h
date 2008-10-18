/*** ***/

#ifndef _DLFLCONNECT_H_
#define _DLFLCONNECT_H_

// Non-inline function definitions for class DLFLObject
// Subroutines that are related to connections between edges/faces

#include <DLFLObject.h>

namespace DLFL {

  void connectEdges( DLFLObjectPtr obj, DLFLEdgePtr eptr1, DLFLFacePtr fptr1,
         DLFLEdgePtr eptr2, DLFLFacePtr fptr2, bool check=true);
  void connectEdgesWithoutLoopCheck( DLFLObjectPtr obj, DLFLEdgePtr eptr1, DLFLFacePtr fptr1,
             DLFLEdgePtr eptr2, DLFLFacePtr fptr2, bool check=true);
  DLFLEdgePtrArray connectFaces( DLFLObjectPtr obj, DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2);
  void dualConnectFaces( DLFLObjectPtr obj, DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2);
  void connectFaces( DLFLObjectPtr obj, DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2, 
         int numsegs, int maxconn = -1, double pinch=1, double pinchCenter=0.5, double bubble=1.0 );
  void connectFaces( DLFLObjectPtr obj, DLFLFacePtr fp1, DLFLFacePtr fp2, int numsegs = 1);
  void ScherkCollinsHandle(DLFLObjectPtr obj, DLFLEdgePtrArray new_edges,
      double pinch, double pinch_center, double pinch_width, int twist = 1, int init_skip = 1,
      int skip = 0, int pinch_num_segs = 2);
  void hermiteConnectFaces( DLFLObjectPtr obj, DLFLFaceVertexPtr fvptr1,
      DLFLFaceVertexPtr fvptr2, int numsegs, double wt1, double wt2,
      int maxconn=-1, int numtwists=0, double pinch=1, double pinchCenter=0.5,
      double bubble=1.0, bool triangulate_newface = false,
      bool scherk_collins = false, double sc_pinch = 1.0,
      double sc_pinch_center = 0.5, double sc_pinch_width = 1.0,
      int hole_twist = 1, int init_skip = 1, int skip = 0,
      int hole_seg_skip = 0, int pinch_num_segs = 2);
  //void hermiteConnectFaces( DLFLObjectPtr obj, DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2,
  //        int numsegs, double wt1, double wt2, int maxconn=-1, int numtwists=0,
  //      double pinch=1, double pinchCenter=0.5, double bubble=1.0);
  void bezierConnectFaces( DLFLObjectPtr obj, DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2,
         int numsegs, double wt1, double wt2);
} // end namespace

#endif // _DLFLCONNECT_H_
