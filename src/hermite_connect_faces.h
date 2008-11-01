/*** ***/

#ifndef _HERMITE_CONNECT_FACES_H_
#define _HERMITE_CONNECT_FACES_H_

// Non-inline function definitions for class DLFLObject
// Subroutines that are related to connections between edges/faces

#include <DLFLObject.h>

namespace DLFL {

// This class wraps up the Handle Creation operation. We should refactor this
// class later.
class Handle {
  // Constructor.
  Handle(DLFLObjectPtr obj);
  static Handle *instance_;

 public:

  static Handle* instance();
  static Handle* instance(DLFLObjectPtr obj);
  // Create the topological handle.
  void Create();
  // We have done with the current handle, clear all the status and tmp variables.
  void Reset();

  // Position the vertices in the handle.
  void HermitePositionVertices();

 // TODO: Make the variables protected and add setter/getter for them.
 //
 // protected:
  DLFLObjectPtr obj_;
  DLFLFaceVertexPtr fvptr1_;
  DLFLFaceVertexPtr fvptr2_;
  int numsegs_;
  double wt1_;
  double wt2_;

  int maxconn_;
  int numtwists_;
  double pinch_;
  double pinch_center_;

  double bubble_;
  bool triangulate_newface_;

  bool scherk_collins_;
  double sc_pinch_;

  double sc_pinch_center_;
  double sc_pinch_width_;

  int hole_twist_;
  int init_skip_;
  int skip_;

  int hole_seg_skip_;
  int pinch_num_segs_;

 private:
  // New vertices created for this handle.
  vector<DLFLVertexPtrArray> segments_;
  // Vertices coords of the two initial faces.
  Vector3dArray verts1_, verts2_;
  // The normals of the two initial faces.
  Vector3d n1_, n2_;
};

} // end namespace

#endif // _HERMITE_CONNECT_FACES_H_
