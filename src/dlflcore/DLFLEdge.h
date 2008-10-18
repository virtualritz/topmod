/*** ***/

/**
 * \file DLFLEdge.h
 */

#ifndef _DLFL_EDGE_HH_
#define _DLFL_EDGE_HH_

// Class to define an edge
//
// And edge contains 2 pointers - one to each end of the edge. The pointers are
// to FaceVertex structures. An edge appears twice in the system unless it is a
// boundary edge of a 2D surface. Each of the pointers points to the beginning
// Vertex of the edge in each of the 2 cases. So each of the pointers will be
// to Face Nodes in different faces. Definition of class FaceVertexPtr is not
// required for this class since only the pointer is stored.

#include "DLFLCommon.h"
#include "FreeList.h"

namespace DLFL {

class DLFLEdge {
 private:
  static NextOnFreeList *edge_pool;

 public :
  static void setLastID( uint id ) ;
  // Override new operator
  void* operator new(size_t size) ;
  // Override delete operator
  void operator delete(void* to_be_deleted) ;
  void operator delete(void* to_be_deleted, size_t size) ;
  static void deleteEdgePool() ;

private:
  enum {EXPANSION_SIZE = 1024};
  static void expandEdgePool() ;

protected :
  // A pointer to the memory pool for DLFLEdges.
  // Distinct ID for each instance
  static uint suLastID;
  // The last assigned ID is stored in this
  // class variable
   
  DLFLFaceVertexPtr fvpV1;
  DLFLFaceVertexPtr fvpV2;
  // ID for this Edge
  uint uID;
  // For use in subdivision surfaces
  DLFLEdgeType etType;
  // Coords for use during subdivs, etc.
  Vector3d auxcoords;
  // Extra storage for normal
  Vector3d auxnormal;
  // Midpoint of edge (not always current)
  Vector3d midpoint;
  // Edge normal (at midpoint, not always current)
  Vector3d normal;

public :
  // Variable for general use to store flags, etc.
  unsigned long flags;
  uint ismarked;
  uint isdummy;
  uint istodel;
  uint isvisited;

protected:
  // Generate a new unique ID
  static uint newID(void);
  // Assign a unique ID for this instance
  void assignID();
  // Update the mid point for this edge
  void updateMidPoint(void);
  // Update the edge normal - average of normals at the 4 corners adjacent to this edge
  void updateNormal(void);

public :
  // Default constructor
  DLFLEdge();
  // 2 & 3 argument constructor
  DLFLEdge(DLFLFaceVertexPtr fvp1, DLFLFaceVertexPtr fvp2, bool update=true);
  // Copy constructor
  DLFLEdge(const DLFLEdge& e);
  // Destructor
  ~DLFLEdge();
  // Assignment operator
  DLFLEdge& operator = (const DLFLEdge& e);
  // Copy function 
  DLFLEdgePtr copy(void) const;
  // Dump contents of object
  void dump(ostream& o) const;
  // Get/set the attribute flags
  DLFLEdgeType getType(void) const;

  Vector3d getAuxCoords(void) const;

  Vector3d getAuxNormal(void) const;
   
  void setType(DLFLEdgeType type);

  void resetType(void);

  void setAuxCoords(const Vector3d& p);

  void setAuxNormal(const Vector3d& n);
   
  void addToAuxCoords(const Vector3d& p);

  void addToAuxNormal(const Vector3d& n);
   
  void resetAuxCoords(void);

  void resetAuxNormal(void);
         

  friend void resetEdgeType(DLFLEdgePtr dep);

  // Query functions
  DLFLFaceVertexPtr getFaceVertexPtr1(void) const;

  DLFLFaceVertexPtr getFaceVertexPtr2(void) const;

  // Get the face-vertex belonging to given face
  // Returns NULL if edge is not adjacent to given face
  DLFLFaceVertexPtr getFaceVertexPtr(DLFLFacePtr fptr);

  DLFLFaceVertexPtr getOtherFaceVertexPtr(DLFLFaceVertexPtr fvptr);

  void getFaceVertexPointers(
      DLFLFaceVertexPtr& fvptr1, DLFLFaceVertexPtr& fvptr2) const;

  void getCorners(DLFLFaceVertexPtr& fvp1, DLFLFaceVertexPtr& fvp2) const;

  uint getID(void) const;

  Vector3d getMidPoint(bool update=false);

  Vector3d getNormal(bool update=false);

  void getEndPoints(Vector3d& p1, Vector3d& p2) const;
  double length(void) const;

  // Get a vector along the direction of the edge from endpoint 1 to endpoint 2
  Vector3d getEdgeVector(void) const;

  // Get the pointers to the 2 faces that this edge is adjacent to
  void getFacePointers(DLFLFacePtr& fptr1, DLFLFacePtr& fptr2);

  // Get pointer to face adjacent to this edge which is not the given face
  DLFLFacePtr getOtherFacePointer(DLFLFacePtr fptr);

  // Get the pointers to the 2 DLFLVertices that define this edge
  void getVertexPointers(DLFLVertexPtr& vp1, DLFLVertexPtr& vp2);

  // Get pointer to vertex which is at opposite end to given vertex pointer
  DLFLVertexPtr getOtherVertexPointer(DLFLVertexPtr vptr);

  // Get all 4 corners adjacent to this edge in CCW order such that those
  // corners will make a face (EF = Edge Face)
  void getEFCorners(DLFLFaceVertexPtrArray& corners);

  // Get the aux coords of all 4 corners adjacent to this edge in CCW order such
  // that those corners make a face (EF = Edge Face)
  void getEFCornersAuxCoords(Vector3dArray& coords);

  // NOTE : All the 4 functions below will return the distance from one
  // of the end points when the point is not in the rectangular region
  // parallel to the line segment
   
  // Distance of a point from the DLFLEdge in 3D
  double distFrom(double x, double y, double z) const;

  // For hit calculations with projections
  double distFromXY(double x, double y) const;
  double distFromYZ(double y, double z) const;
  double distFromZX(double z, double x) const;

  // Find distance between midpoints of two given edges
  friend double distBetween(DLFLEdgePtr ep1, DLFLEdgePtr ep2);

  // Get the Edge normals - vectors along the surface perpendicular to the edge
  // Calculated as cross-product of surface normal at corner and edge vector
  // Will be opposite in direction for the two corners
  // Specify the corner where the normal is to be calculated. This defines the origin
  // for the edge vector. Checks to see if given corner belongs to Edge. If not returns
  // a zero vector as the normal
  Vector3d getEdgeNormal(DLFLFaceVertexPtr fvptr) const;

  // Compute and return the average of the normal at the two end vertices
  Vector3d averageVertexNormal(void) const;

  // Compute and return the average of the normal at the two end corners
  Vector3d averageCornerNormal(void) const;

  // Mutative functions
  void setFaceVertexPtr1(DLFLFaceVertexPtr fvptr, bool update=true);

  void setFaceVertexPtr2(DLFLFaceVertexPtr fvptr, bool update=true);

  void setFaceVertexPointers(
      DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2, bool update=true);
   
  void makeUnique(void);

  friend void makeEdgeUnique(DLFLEdgePtr dep);

private :
   
  // Reverse the edge. Updates the face-vertex pointers appropriately.
  // WARNING! This function will leave the object in an invalid state
  // unless ALL other edges and faces are also reversed. To prevent unintended
  // problems, this is made private and classes which need access to this
  // function are made friends
  void reverse(void);

  friend class DLFLObject;

public :

  // Update the DLFLEdgePtr field of the 2 FaceVertex members
  void updateFaceVertices(void);

  // Update the FaceVertexPtr fields selectively

  // If any of the FaceVertexPtr fields are NULL set it to the given FaceVertexPtr
  void setNullFaceVertexPtr(DLFLFaceVertexPtr fvptr);

  // If any of the FaceVertexPtr fields matches the given FaceVertexPtr, set it to NULL
  void resetFaceVertexPtr(DLFLFaceVertexPtr fvptr);
   
  // Equality operator - two edges are equal if they have the same 2 end-points
  friend bool operator == (const DLFLEdge& e1, const DLFLEdge& e2);

  // Check if the DLFLEdge is a 'valid' edge - both FaceVertexes shouldn't be from the
  // same Face
  bool isValid(void) const;
   
  // Is this Edge a self-loop? Are the 2 end points the same?
  bool isSelfLoop(void) const;

  // Check if 2 edges intersect
  friend bool checkIntersection(DLFLEdgePtr ep1, DLFLEdgePtr ep2);

  // Print out the DLFLEdge
  friend ostream& operator << (ostream& o, const DLFLEdge& e);

  // Print out the Face IDs of the 2 FaceVertices
  void printFaceIDs(void) const;
  void printPointers(void) const;
  void print(void) const;

  // Write out the edge in DLFL format
  void writeDLFL(ostream& o) const;

  // Write out the edge in DLFL format but in reverse order. Useful for crust modeling
  void writeDLFLReverse(ostream& o) const;

};

  bool coFacial(DLFLEdgePtr ep1, DLFLEdgePtr ep2);
  void resetEdgeType(DLFLEdgePtr dep);
  void makeEdgeUnique(DLFLEdgePtr dep);
} // end namespace

#endif /* #ifndef _DLFL_EDGE_HH_ */

