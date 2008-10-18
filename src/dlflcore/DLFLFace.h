/*** ***/

/**
 * \file DLFLFace.h
 */

#ifndef _DLFL_FACE_HH_
#define _DLFL_FACE_HH_

// Class for a Face to be used in the DLFLObject class
// Based on the OBJFace class

// A DLFLFace is just a list of DLFLFaceVertexes
// The ordering of the DLFLFaceVertexes is important
// A DLFLFace can be implemented as a 2-3 Tree or as a linked list

#include "DLFLCommon.h"
#include "DLFLFaceVertex.h"
#include "DLFLMaterial.h"
#include "FreeList.h"
//#include <Light.h>

namespace DLFL {

class DLFLFace {
private:
  static NextOnFreeList *face_pool;

public :
  static void setLastID(uint id) ;
  // Override new operator
  void* operator new(size_t size) ;
  // Override delete operator
  void operator delete(void* to_be_deleted) ;
  void operator delete(void* to_be_deleted, size_t size) ;

  static void deleteFacePool() ;

private:
  enum {EXPANSION_SIZE = 1024};
  static void expandFacePool() ;
protected :
  //!< Distinct ID for each instance
  static uint suLastID;

  static uint newID(void);
  //!< ID for this Face
  uint uID;
  //!< Head of list of face-vertex pointers
  DLFLFaceVertexPtr head;
  //!< Pointer to material for this face
  DLFLMaterialPtr matl_ptr;
  //!< For use in subdivision surfaces
  DLFLFaceType ftType;
  //!< Coords for use during subdivs, etc.
  Vector3d auxcoords;
  //!< Extra storage for normal
  Vector3d auxnormal;

public :
  //!< Centroid of this face (not always current)
  Vector3d centroid;
  //!< Normal to this face. Not always correct/current
  Vector3d normal;
  //!< Variable for general use to store flags, etc.
  unsigned long flags;
  uint ismarked;

protected :

   // Assign a unique ID for this instance
  void assignID(void);

  // Advance the pointer by given number of nodes. Similar to STL advance
  // algorithm.
  DLFLFaceVertexPtr advance(DLFLFaceVertexPtr ptr, uint num) const;

  // Copy all face-vertices from another face specified by it's head pointer
  void copy(DLFLFaceVertexPtr ptr);

public :

  // Default and 1 arg constructor
  DLFLFace(DLFLMaterialPtr tmp = NULL);

  // Copy constructor
  DLFLFace(const DLFLFace& face);

  // Destructor
  ~DLFLFace();

  // Assignment operator
  DLFLFace& operator = (const DLFLFace& face);

  DLFLFacePtr copy(void) const;

  // Dump contents of object
  void dump(ostream& o) const;
      
  Vector3d geomSum(void) const;

  // Sum of texture coordinates
  Vector2d textureSum(void) const;

  // Color sum
  RGBColor colorSum(void) const;

  // Normal sum
  Vector3d normalSum(void) const;

  // Get all sums
  void getSums(
      Vector3d& sumg, Vector2d& sumtc, RGBColor& sumc, Vector3d& sumn) const;

  // Update the centroid of this face
  void updateCentroid(void);
   
  // Update the centroid of this face and return it
  Vector3d geomCentroid(void);

  // Compute the texture centroid
  Vector2d textureCentroid(void) const;

  // Compute the color centroid
  RGBColor colorCentroid(void) const;

  // Update the normal of this face
  void updateNormal(void);
   
  // Compute the normal centroid
  Vector3d normalCentroid(void);

  // Compute the normal for this face and send it to the face-vertices
  Vector3d computeNormal(void);

  // Compute the normal for each face-vertex and store it in the face-vertex
  // itself. Don't do any averaging, etc.
  void storeNormals(void);
   
  // Compute all centroids
  void getCentroids(Vector3d& cen, Vector2d& texc, RGBColor& colc,
                    Vector3d& nc) const;

  //--- Query Functions ---//

  uint getID(void) const;

  DLFLFaceType getType(void) const;

  DLFLMaterialPtr material(void) const;

  Vector3d getAuxCoords(void) const;

  Vector3d getAuxNormal(void) const;
   
  Vector3d getNormal(bool update=false);

  Vector3d getCentroid(void) const;

  //--- Mutative Functions ---//

  void makeUnique(void);

  // Delete all the face-vertices of this face
  void destroy(void);
   
  void reset(void);
  // Insert a copy
  DLFLFaceVertexPtr addVertex(const DLFLFaceVertex& dfv);
  // Insert a copy
  DLFLFaceVertexPtr addVertex(DLFLFaceVertexPtr dfvp); 
  // Insert the pointer
  void addVertexPtr(DLFLFaceVertexPtr dfvp);

  // Delete given FaceVertex from face. Adjust pointers of adjacent
  // face-vertices. Memory for given face-vertex is not freed. Given
  // FaceVertex must belong to this Face. If given face-vertex is the
  // only face-vertex in this face, nothing is done (for now)
  void deleteVertexPtr(DLFLFaceVertexPtr dfvp);

  // Same as above, but more correct function name
  void deleteFaceVertexPtr(DLFLFaceVertexPtr dfvp);
   
  // Get the coordinates of the vertices of the face
  void getVertexCoords(Vector3dArray& verts);

  // STL list type access to the Face
  DLFLFaceVertexPtr front(void);

  DLFLFaceVertexPtr back(void);

  // Return the first/last element of the list
  DLFLFaceVertexPtr firstVertex(void);

  DLFLFaceVertexPtr lastVertex(void);

  // Set the material pointer
  void setMaterial(DLFLMaterialPtr mptr);

  void setType(DLFLFaceType type);

  void resetType(void);

  void setAuxCoords(const Vector3d& p);

  void setAuxNormal(const Vector3d& n);
   
  void addToAuxCoords(const Vector3d& p);

  void addToAuxNormal(const Vector3d& n);
   
  void resetAuxCoords(void);

  void resetAuxNormal(void);
  // No. of vertices in this face
  uint size(void) const;
  uint numFaceVertexes(void) const;
  // Reset type of Face and all FaceVertexes in this Face
  void resetTypeDeep(void);

  // Compute color for each corner using given light and using material of
  // Face.
  //void computeLighting(LightPtr lightptr);

  // Update the DLFLFacePtr for each DLFLFaceVertex
  void updateFacePointers(void);
  void resetFacePointers(void);
   
  // Update the DLFLFaceVertexList for each DLFLVertex referred to by each
  // DLFLFaceVertex.
  void addFaceVerticesToVertices(void);
  void deleteFaceVerticesFromVertices(void);

  // Update the DLFLFaceVertexPtr for each DLFLEdge connected to each
  // DLFLFaceVertex.
  void addFaceVerticesToEdges(void);
  void deleteFaceVerticesFromEdges(void);

  // Update DLFLEdges and DLFLVertices - above functions combined for
  // efficiency.
  void addFaceVerticesToVerticesAndEdges(void);
  void deleteFaceVerticesFromVerticesAndEdges(void);
   
  // Create an array of DLFLEdges for this DLFLFace. Number of DLFLEdges in
  // the array is returned. Memory will be allocated inside this function,
  // which should be freed by the caller. Pass a pointer to the array
  // (DLFLEdge **).
  int getEdges(DLFLEdge ** edges);

  // Get the edges in the face. Different from previous function. Doesn't
  // create new edges, simply stores existing edge pointer from the face
  // vertices into the STL array or STL linked list.
  void getEdges(DLFLEdgePtrArray& edges);
  void getEdges(DLFLEdgePtrList& edges);

  // Get the corners in the face. Stores the existing DLFLFaceVertexPtr's in
  // the face into the array.
  void getCorners(DLFLFaceVertexPtrArray& corners);

  // Helpful for python interface
  DLFLFaceVertexPtr findFaceVertex(uint vertexId) const;
  DLFLFaceVertexPtr findFaceVertexByID(uint faceVertexId) const;

  // Get the corners and the coordinates
  void getCornersAndCoords(
      DLFLFaceVertexPtrArray& corners, Vector3dArray& coords);

  // Does this Face contain the given face-vertex?
  bool contains(DLFLFaceVertexPtr dfvp);

  // Does this face share only one vertex with the given face?
  bool sharesOneVertex(DLFLFacePtr dfp);

  void getNeighboringFaces(DLFLFacePtrArray& fparray);

  // Find the DLFLFaceVertex following the given DLFLFaceVertex in this DLFLFace
  // If only one DLFLFaceVertex is there in the DLFLFace returns itself
  DLFLFaceVertexPtr nextFaceVertex(DLFLFaceVertexPtr fvptr);

  // Find the DLFLFaceVertex preceding the given DLFLFaceVertex in this DLFLFace
  // If only one DLFLFaceVertex is there in the DLFLFace returns itself
  DLFLFaceVertexPtr prevFaceVertex(DLFLFaceVertexPtr fvptr);

  // Find the FaceVertex which is closest to the given point
  DLFLFaceVertexPtr findClosest(const Vector3d& p);

  // Find the two corners which are closest to each other in the two faces
  // The passed DLFLFaceVertex pointers will be set to the closest corners,
  // the first one from the first face and the second one from the second face
  static void findClosestCorners(DLFLFacePtr fp1, DLFLFacePtr fp2,
         DLFLFaceVertexPtr& fvp1, DLFLFaceVertexPtr& fvp2);

  // Add a new DLFLFaceVertex after/before a given DLFLFaceVertex in this DLFLFace
  DLFLFaceVertexPtr insertAfter(
      DLFLFaceVertexPtr fvptr, DLFLFaceVertexPtr new_fvp, bool copy=true);
  DLFLFaceVertexPtr insertBefore(
      DLFLFaceVertexPtr fvptr, DLFLFaceVertexPtr new_fvp, bool copy=true);

  // Reorder the face-vertices so that the given face-vertex is the first one
  // Given face vertex should belong to this face
  void reorder(DLFLFaceVertexPtr fvptr);

public :
  // Reverse the face - reverses the order of the face-vertices
  // WARNING!! This operation invalidates the DLFL structure. All edges
  // associated with this face will be in an invalid state.
  void reverse(void);

protected :
  // Classes which need to access the reverse method should be declared as
  // friends here.
  friend class DLFLObject;

public :
  // Output the boundary walk of the face
  vector<int> vertexWalk() const;
  vector<int> edgeWalk() const;
  void boundaryWalk(void) const;
  void print(void) const;

  friend void boundaryWalk(DLFLFacePtr faceptr);
  friend void boundaryWalk(const DLFLFace& face);
   
  // For debugging
  void printPointers(void) const;
   
  // Write out the Face in OBJ format to an output stream - source for more info
  // min_id is the minimum ID value. It will subtracted from the ID before
  // output.
  void objWrite(ostream& o, uint min_id) const;
  void objWriteWithNormals(ostream& o, uint min_id, uint& normal_id_start) const;
  void objWriteWithTexCoords(ostream& o, uint min_id, uint& tex_id_start) const;
  void objWriteWithNormalsAndTexCoords(
      ostream& o, uint min_id, uint& normal_id_start, uint& tex_id_start) const;

  // Write out the normals for each vertex in the Face in OBJ format
  void objWriteNormals(ostream& o) const;

  // Write out the texture coordinates for each vertex in the Face in OBJ format
  void objWriteTexCoords(ostream& o) const;

  // Write out the face in DLFL format
  void writeDLFL(ostream& o) const;

  // Write out the face in DLFL format but in reverse order. Useful for crust
  // modeling.
  void writeDLFLReverse(ostream& o) const;
   
  // Access the face-vertex specified by index. No range checks done
  DLFLFaceVertexPtr facevertexptr(uint index);
  DLFLFaceVertex& facevertex(uint index);
  DLFLFaceVertex facevertex(uint index) const;

  // Access the vertex specified by index. No range checks done
  DLFLVertexPtr vertexptr(uint index);
  DLFLVertex& vertex(uint index);
  DLFLVertex vertex(uint index) const;

  // Access the coordinates of the vertex specified by index. No range checks
  // done.
  Vector3d& vertex_coords(uint index);
  Vector3d vertex_coords(uint index) const;

  // Access color and normal of face vertex specified by index
  RGBColor& fv_color(uint index);
  RGBColor fv_color(uint index) const;
   
  Vector3d& fv_normal(uint index);
  Vector3d fv_normal(uint index) const;

  // Set the color of all face vertices of this face
  void setColor(const RGBColor& col);
  void setColor(double r, double g, double b);

  // FOR QUADS ONLY - Randomly assign texture coordinates to the 4 corners to
  // match a 1,1 square.
  void randomAssignTexCoords(void);
   
  void updateMaterial(void);

  float getArea();

  //--- Rendering subroutines ---//


  void for_each(void (*func)(DLFLFaceVertexPtr)) const;

  friend ostream& operator << (ostream& o, const DLFLFace& face);
};

void boundaryWalk(DLFLFacePtr faceptr);
void boundaryWalk(const DLFLFace& face);


} // end namespace

#endif /* #ifndef _DLFL_FACE_HH_ */

