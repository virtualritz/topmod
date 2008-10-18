/*** ***/

/**
 * \file DLFLVertex.h
 */

#ifndef _DLFL_VERTEX_HH_
#define _DLFL_VERTEX_HH_

// Class to store a vertex for the DLFLObject class
// Based on the OBJVertex class

#include "DLFLCommon.h"
#include "FreeList.h"

namespace DLFL {
  
  class DLFLVertex : public NextOnFreeList {
  private:
    static NextOnFreeList *vertex_pool;
  public :
    static void setLastID( uint id );

    // Override new operator
    void* operator new(size_t size);
    // Override delete operator
    void operator delete(void* to_be_deleted);
    void operator delete(void* to_be_deleted, size_t size);

    static void deleteVertexPool();

  private:
    enum {EXPANSION_SIZE = 1024};
    static void expandVertexPool();

  protected :
    // class variable
    // To assign distinct ID for each instance, the last assigned ID is
    // stored in this static variable.
    static uint suLastID;

    // Generate a new unique ID
    static uint newID(void);
     
  public :
    Vector3d coords; // Coordinates of vertex.
    uint ismarked; // flag for traverse - ozgur.
    uint isvisited; // Variable for general use to store flags, etc.
    unsigned long flags;
    uint CHullIndex; // Index used for convex hull - Esan.

  protected :
    uint uID; // ID for this DLFLVertex
    uint index; // Index for use in file output
    Vector3d auxcoords; // Coords for use during subdivs, etc.
    Vector3d auxnormal; // Extra storage for normal
    Vector3d normal; // Average normal at this vertex
    DLFLFaceVertexPtrList fvpList; // List of DLFLFaceVertexes which
    // refer to this DLFLVertex
    DLFLVertexType vtType; // For use in subdivision surfaces

    // Assign a unique ID for this instance
    void assignID(void);

  public :
    // Default constructor
    DLFLVertex();
    
    // 1 argument constructor
    DLFLVertex(const Vector3d& vec);

    // 3 argument constructor
    DLFLVertex(double x, double y, double z);
  
    // Copy constructor
    DLFLVertex(const DLFLVertex& dv);
  
    // Destructor
    ~DLFLVertex();

    // Assignment operator
    DLFLVertex& operator = (const DLFLVertex& dv);

    DLFLVertexPtr copy(void) const;

    // Dump contents of this object
    void dump( ostream& o ) const;

    void reset(void);

    void makeUnique(void);

    friend void makeVertexUnique(DLFLVertexPtr dvp);
    //--- Query functions ---//

    DLFLVertexType getType(void) const;

    Vector3d getCoords(void) const;

    DLFLFaceVertexPtrList getFaceVertexList(void) const;

    // Number of Edges incident on this Vertex = no. of Faces adjacent to this Vertex
    // = size of the FaceVertex list = valence of Vertex
    uint numEdges(void) const;
     
    uint numFaces(void) const;

    uint valence(void) const;
     
    uint getID(void) const;

    uint getIndex(void) const;

    Vector3d getAuxCoords(void) const;

    Vector3d getAuxNormal(void) const;
     
    Vector3d getNormal(void) const;

    //--- Mutative functions ---//

    void setType(DLFLVertexType type);

    void resetType(void);

    // Reset type of vertex, all face-vertices and edges connected to this vertex
    void resetTypeDeep(void);

    friend void resetVertexType(DLFLVertexPtr dvp);

    void setFaceVertexList(const DLFLFaceVertexPtrList& list);

    void setCoords(const Vector3d& p);

    // Set the aux. coords
    void setAuxCoords(const Vector3d& p);

    void setAuxNormal(const Vector3d& n);
     
    // Add to the aux coords
    void addToAuxCoords(const Vector3d& p);

    void addToAuxNormal(const Vector3d& n);
     
    // Reset the aux coords
    void resetAuxCoords(void);

    void resetAuxNormal(void);
           
    // Set the texture coordinates for all FaceVertexes referring to this vertex
    void setTexCoords(const Vector2d& texcoord);

    // Set the color values for all FaceVertexes referring to this vertex
    void setColor(const RGBColor& color);

    // Set the normal for this vertex directly
    void setNormal(const Vector3d& n);

    // If flag is true recompute normals for all FaceVertexes referring to this vertex,
    // Update the vertex normal. Returns the vertex normal
    Vector3d updateNormal(bool recompute=true);

    Vector3d computeNormal(bool set=false);

    // Retained for backward compatibility
    // Same as updateNormal but doesn't recompute the corner normals
    Vector3d averageNormal(void);

    // Get normals of all corners referring to this vertex. Returns average normal
    Vector3d getNormals(Vector3dArray& normals);

    // Set tex coordinates, color and normal info for all FaceVertexes referring to this vertex
    void setFaceVertexProps(const Vector2d& texcoord, const RGBColor& color, const Vector3d& normal);

    // Update the DLFLFaceVertexList by adding a new DLFLFaceVertexPtr
    void addToFaceVertexList(DLFLFaceVertexPtr fvptr);

    void deleteFromFaceVertexList(DLFLFaceVertexPtr fvptr);

    // DLFL Vertex Trace
    void vertexTrace(void) const;
    friend void vertexTrace(DLFLVertexPtr vertexptr);
    friend void vertexTrace(const DLFLVertex& vertex);

    // Create an array of Edge's incident on this Vertex. The number of Edges
    // is returned. Memory will be allocated inside this function, which should
    // be freed by the caller. Pass a pointer to the array (DLFLEdgePtr **)
    // Equivalent to a vertex trace
    int getEdges(DLFLEdgePtr ** edges) const;

    // Same as above but uses the STL Array
    void getEdges(DLFLEdgePtrArray& edges) const;

    // Get the Edge incident on this Vertex which connects to given Vertex
    // If no such edge exists, returns NULL
    DLFLEdgePtr getEdgeTo(DLFLVertexPtr vp);

    // Create an array of FaceVertexes which share this Vertex.
    void getFaceVertices(DLFLFaceVertexPtrArray& fvparray);
    void getOrderedFaceVertices(DLFLFaceVertexPtrArray& fvparray);
     
    void getCorners(DLFLFaceVertexPtrArray& fvparray);
    void getOrderedCorners(DLFLFaceVertexPtrArray& fvparray);
     
    // Get the aux coords from all corners which share this Vertex
    void getCornerAuxCoords(Vector3dArray& coords) const;
    void getOrderedCornerAuxCoords(Vector3dArray& coords) const;

    // Create an array of Faces which share this Vertex
    void getFaces(DLFLFacePtrArray& fparray);

    // Get the FaceVertex belonging to the given face. If only 1 face-vertex
    // is there in the list, return that. If more than 1 exist but nothing
    // belongs to given face, return NULL
    DLFLFaceVertexPtr getFaceVertexInFace(DLFLFacePtr fp);

    // Get the FaceVertex which has the given Vertex before/after it in it's Face
    // If only 1 FaceVertex refers to this Vertex, will return that
    // If there are more than 1 and none of them satisfies the condition, returns NULL
    DLFLFaceVertexPtr getFaceVertexWithPrev(DLFLVertexPtr vp);
    DLFLFaceVertexPtr getFaceVertexWithNext(DLFLVertexPtr vp);

    // Get the FaceVertex which has the 'backface' flag set
    // If no such FaceVertex is found, returns NULL
    DLFLFaceVertexPtr getBackFaceVertex(void);

    // Apply a transformation specified by the matrix to the coordinates
    void transform(const Matrix4x4& tmat);
    // Print out this Vertex
    void print(void) const;

    // Write this vertex in DLFL format and set it's index value
    void writeDLFL(ostream& o, uint newindex);

    // Read a vertex from an input stream.
    // The 3 coordinates should be specified separated by spaces (as in OBJ format)
    friend istream& operator >> (istream& i, DLFLVertex& dv);
    // Write a vertex to an output stream. Writes in OBJ format
    friend ostream& operator << (ostream& o, const DLFLVertex& dv);
  };

  void makeVertexUnique(DLFLVertexPtr dvp);
  void resetVertexType(DLFLVertexPtr dvp);
  void vertexTrace(DLFLVertexPtr vertexptr);
  void vertexTrace(const DLFLVertex& vertex);
  istream& operator >> (istream& i, DLFLVertex& dv);
  ostream& operator << (ostream& o, const DLFLVertex& dv);

} // end namespace

#endif /* _DLFL_VERTEX_HH_ */
