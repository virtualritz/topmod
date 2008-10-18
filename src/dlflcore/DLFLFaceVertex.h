/*** ***/

/**
 * \file DLFLFaceVertex.h
 */

#ifndef _DLFL_FACE_VERTEX_HH_
#define _DLFL_FACE_VERTEX_HH_

// Class for a face vertex (corner) for the DLFLObject class
// Based on OBJFaceVertex class

// Contains a pointer to a Vertex and a pointer to the Edge which *starts* at this Face Vertex
// NOTE : The same physical point may be the start of many edges, but for a given face
// only 1 Edge can start at a physical point
// 'start' implies existence of a rotation order for the face
// Each FaceVertex also contains a pointer to the Face to which it belongs

// The FaceVertex class can now also be used directly as a node in a list
// It contains pointers to the next and previous nodes of the list

#include "DLFLCommon.h"
#include "DLFLCoreExt.h"
#include "DLFLEdge.h"
#include "DLFLVertex.h"
#include "FreeList.h"

namespace DLFL {

class DLFLFaceVertex {
 private:
  static NextOnFreeList *face_pool;
 public:
  static void setLastID( uint id );

  // Override new operator
  void* operator new(size_t size);
  // Override delete operator
  void operator delete(void* to_be_deleted);
  void operator delete(void* to_be_deleted, size_t size);

  static void deleteEdgePool();

 private:
  enum {EXPANSION_SIZE = 1024};
  static void expandEdgePool();

 protected:
  static uint suLastID;

  static uint newID( );

 public :
  // Associated vertex pointer
  DLFLVertexPtr vertex;
  Vector3d normal;
  RGBColor color;
  // Texture coordinate
  Vector2d texcoord;
  // Flag indicating this is part of a "back" face. Used in reading OBJ files.
  // Default value is 'false'.
  bool backface;

 protected :
  // Id for face vertex
  uint uID;
  // Index for use in file output
  uint index;
  // Pointer to the Edge
  DLFLEdgePtr epEPtr;
  // Pointer to the Face
  DLFLFacePtr fpFPtr;
   
  // List node data
  // Pointer to next FaceVertex
  DLFLFaceVertexPtr  fvpNext;
  // Pointer to previous FaceVertex
  DLFLFaceVertexPtr  fvpPrev;
  // For use in subdivision surfaces
  DLFLFaceVertexType fvtType;
  // Coords for use during subdivs, extrude, etc.
  Vector3d auxcoords;
  // Additional storage for normal
  Vector3d auxnormal;
  // The following member variables are only used in Patchmode.
  // Fenghui: remove it, create them only when necessary.
  // Vector3d ds2coords[4];// Level-2 Doo Sabin coordinates
  // Level-2 Doo Sabin coordinates (used in patch mode)
  Vector3d *ds2coords;
  //TMPatchPtr tmpp;// Pointer to the TMPatch corresponding to this corner

  void assignID( ) ;

public :
  // Default constructor
  DLFLFaceVertex(bool bf=false);
  // 2 arg-constructor - copy the pointers
  DLFLFaceVertex(DLFLVertexPtr vptr, DLFLEdgePtr eptr, bool bf=false);
  // Copy constructor
  DLFLFaceVertex(const DLFLFaceVertex& dfv);
  // Destructor
  ~DLFLFaceVertex();

  // Assignment operator
  DLFLFaceVertex& operator=(const DLFLFaceVertex& dfv);
  bool operator==(const DLFLFaceVertex &other) const; 

  DLFLFaceVertexPtr copy( ) const;

  // Dump contents of this object
  void dump(ostream& o) const;

  // Average 2 facevertexes
  // Average the coordinates, normals, color and texture coordinates
  // Assumes that all 3 have valid vertex pointers
  friend void average(const DLFLFaceVertex& dfv1, const DLFLFaceVertex& dfv2, DLFLFaceVertex& ave);
  friend void average(DLFLFaceVertexPtr dfvp1, DLFLFaceVertexPtr dfvp2, DLFLFaceVertexPtr avep);

  // Query Functions
  uint getIndex( ) const ;
  uint getID( ) ;
  DLFLFaceVertexType getType( ) const ;
  DLFLVertexType getVertexType( ) const ;
  DLFLVertexPtr getVertexPtr( ) const ;
  DLFLEdgePtr getEdgePtr( ) const ;
  DLFLFaceVertexPtr getOppositeCorner( ) ;
  DLFLFacePtr getFacePtr( ) const ;
  uint getVertexID( ) const ;
  uint getFaceID( ) const;
  uint getEdgeID( ) const ;
  Vector3d getNormal( ) const ;
  Vector3d getVertexCoords( ) const ;
  Vector2d getTexCoords( ) const ;
  Vector3d getAuxCoords( ) const ;
  Vector3d getAuxNormal( ) const ;
  void getDS2Coords(Vector3d& dsc0, Vector3d& dsc1, Vector3d& dsc2, Vector3d& dsc3) const ;
  Vector3d getDS2Coord(uint index) const ;
   
  //--- Mutative Functions ---//

  void setType(DLFLFaceVertexType type) ;
  void resetType( ) ;
   
  // Reset to original state
  void reset( ) ;

  void setVertexPtr(DLFLVertexPtr vptr) ;
  void setEdgePtr(DLFLEdgePtr eptr) ;
  void setFacePtr(DLFLFacePtr fptr) ;
  void setNormal(const Vector3d& n) ;

  /* Check if this corner is a concave corner or not
   * Will not work properly for non-planar faces.
   * Will not work properly for faces which have edges which belong entirely to that face.
   * Will not work properly for highly skewed geometry */
  bool isConcaveCorner( ) const;

  /* Check if this corner is a winged corner or not
   * A winged corner is a degenerate corner 
   * i.e. the angle at the corner is 180 degrees */
  bool isWingedCorner( ) const;

  /* Find the next non-winged corner starting from this corner
   * If none can be found, returns NULL */
  DLFLFaceVertexPtr nextNonWingedCorner( ) const;

  /* Find the CLOSEST non-winged corner starting from this corner
   * If none can be found, returns NULL
   * Search both forward and backward till we find one. */
  DLFLFaceVertexPtr closestNonWingedCorner( ) const;

  // Get a vector along the edge starting at this corner
  Vector3d getEdgeVector( ) const;

  /* Get 2 vectors along the 2 edges which form this corner
   * Both vectors will originate at this corner */
  void getEdgeVectors(Vector3d& pvec, Vector3d& nvec);

  /* Compute the normal using adjacent vertices in this face
   * Normal will be adjusted to account for concave corners */
  void updateNormal( );
  
  Vector3d computeNormal( ) ;

  void setVertexCoords(const Vector3d& vec) ;
  void setVertexCoords(double x, double y, double z) ;

  void setTexCoords(const Vector2d& tc) ;
  void setAuxCoords(const Vector3d& p) ;
  void setAuxNormal(const Vector3d& n) ;

  void setDS2Coords(const Vector3d& dsc0, const Vector3d& dsc1, const Vector3d& dsc2,
      const Vector3d& dsc3);

  void setDS2Coord0(const Vector3d& dsc0);
  void setDS2Coord1(const Vector3d& dsc1);
  void setDS2Coord2(const Vector3d& dsc2);
  void setDS2Coord3(const Vector3d& dsc3);
   
  // Update the DLFLFaceVertexList of the DLFLVertex referred to by this
  // DLFLFaceVertex.
  void addSelfToVertex( );
  void deleteSelfFromVertex( );

  // Update the DLFLFaceVertexPtr of the DLFLEdge starting at this DLFLFaceVertex
  void addSelfToEdge( );
  void deleteSelfFromEdge( );

  // Remove the DLFLFaceVertexPtr from the DLFLFace to which it belongs
  void deleteSelfFromFace( );

  // List related functions

private:
  // This function by itself can leave the DLFL object in an invalid state
  // It should be used with extreme care, ideally as part of a face reversal only.
  // To prevent unintended misuse, this is made private and classes which need
  // access to this function are made friends
  void reverse( ) ;

  friend class DLFLFace;

public :

  void setNext(DLFLFaceVertexPtr fvptr) ;
  void setPrev(DLFLFaceVertexPtr fvptr) ;

  DLFLFaceVertexPtr next( ) const ;
  DLFLFaceVertexPtr& next( ) ;

  DLFLFaceVertexPtr prev( ) const ;
  DLFLFaceVertexPtr& prev( ) ;

  /* Next/prev facevertex around the vertex pointed to by this facevertex
   * Use the EdgePtr to do a 1-step vertex-trace
   * Returns this facevertex if EdgePtr is NULL */
  DLFLFaceVertexPtr vnext( );
  DLFLFaceVertexPtr vprev( );

  /* Make given DLFLFaceVertexPtr point to the next DLFLFaceVertexPtr
   * Used in list traversal */
  friend void advance(DLFLFaceVertexPtr& fvp);
/*
  friend void advance(DLFLFaceVertexPtr& fvp) {
    fvp = fvp->fvpNext;
  };
*/
  /* Make given DLFLFaceVertexPtr point to the previous DLFLFaceVertexPtr
   * Used in list traversal */
  friend void backward(DLFLFaceVertexPtr& fvp);

  void print(bool printadjacent = false) const;

  void printPointers( ) const ;

  // Write this face vertex in DLFL format and set it's index value
  void writeDLFL(ostream& o, uint newindex);

  /* Read normal, texcoord and color info for this face vertex from an input stream
   * in DLFL format. Returns the vertex index */
  uint readDLFL(istream& i);

  /*
  // "Render" this vertex. Simple calls the render() function for the vertex pointer
  // This is used for selection
  void render( ) const;

  // Do a glVertex on this vertex, using the normal and with color
  friend void glVertexCN(const DLFLFaceVertex& dfv);

  // Do a glVertex on this vertex, using the normal and without color
  friend void glVertexN(const DLFLFaceVertex& dfv);
   
  // Do a glVertex on this vertex, without using the normal and with color
  friend void glVertexC(const DLFLFaceVertex& dfv);
   
  // Do a glVertex on this face vertex, without using the normal and without color
  // This function is not named glVertex to allow using it in the
  // STL for_each algorithm, since glVertex is overloaded for DLFLVertex also
  friend void glVertexOFV(const DLFLFaceVertex& dfv);

  //-- Same as above functions but with texture coordinates also --//

  friend void glVertexCNT(const DLFLFaceVertex& dfv);
  friend void glVertexNT(const DLFLFaceVertex& dfv);
  friend void glVertexCT(const DLFLFaceVertex& dfv);
  friend void glVertexOFVT(const DLFLFaceVertex& dfv);

  //-- Versions of above functions which take pointers are arguments --//
  friend void glVertexCNp(DLFLFaceVertexPtr dfvp);
  friend void glVertexNp(DLFLFaceVertexPtr dfvp);
  friend void glVertexCp(DLFLFaceVertexPtr dfvp);
  friend void glVertexOFVp(DLFLFaceVertexPtr dfvp);

  friend void glVertexCNTp(DLFLFaceVertexPtr dfvp);
  friend void glVertexNTp(DLFLFaceVertexPtr dfvp);
  friend void glVertexCTp(DLFLFaceVertexPtr dfvp);
  friend void glVertexOFVTp(DLFLFaceVertexPtr dfvp);*/
}; // end class DLFLFaceVertex

bool coFacial(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2);
bool coFacial(const DLFLFaceVertex& fv1, const DLFLFaceVertex& fv2);

bool areEqual(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2);
bool areEqual(const DLFLFaceVertex& fv1, const DLFLFaceVertex& fv2);

void advance(DLFLFaceVertexPtr& fvp);
void backward(DLFLFaceVertexPtr& fvp);



} // end namespace

#endif /* _DLFL_FACE_VERTEX_HH_ */

