/*** ***/

/**
 * \file DLFLVertex.cc
 */

// Non-inline function definitions for DLFLVertex class

#include "DLFLVertex.h"
#include "DLFLFace.h"
#include "DLFLEdge.h"

namespace DLFL {
  NextOnFreeList *DLFLVertex::vertex_pool = NULL;
  uint DLFLVertex::suLastID = 0;

    // Override new operator
    void* DLFLVertex::operator new(size_t size) {
      if(vertex_pool == NULL) {
        expandVertexPool();
      }
      NextOnFreeList *head = vertex_pool;
      vertex_pool = vertex_pool->next;
      return head;
    };

    // Override delete operator
    void DLFLVertex::operator delete(void* to_be_deleted) {
      NextOnFreeList *head = static_cast<NextOnFreeList *>(to_be_deleted);
      head->next = vertex_pool;
      vertex_pool = head;
    };

    void DLFLVertex::operator delete(void* to_be_deleted, size_t size) {
      NextOnFreeList *head = static_cast<NextOnFreeList *>(to_be_deleted);
      head->next = vertex_pool;
      vertex_pool = head;
    };

    /*static*/ void DLFLVertex::deleteVertexPool() {
      NextOnFreeList *head;
      while(vertex_pool != NULL) {
        head = vertex_pool;
        vertex_pool = vertex_pool->next;
        delete[] head;
      }
    };

    /*static*/ void DLFLVertex::expandVertexPool() {
      //Allocate an object large enough to hold both objects
      size_t size = (sizeof(DLFLVertex) > sizeof(NextOnFreeList)) ? sizeof(DLFLVertex) :
                    sizeof(NextOnFreeList);
      NextOnFreeList *runner = (NextOnFreeList *)malloc(size);
      vertex_pool = runner;
      for(int i = 0; i < EXPANSION_SIZE; i++) {
        runner->next = (NextOnFreeList *)malloc(size);
        runner = runner->next;
      }
      runner->next = NULL;
    };
  // Dump contents of this object to an output stream
  void DLFLVertex::dump(ostream& o) const {
    o << "DLFLVertex" << endl
      //    << "  ID : " << uID << endl
      //    << "  Coords : " << coords << endl
      //    << "  Type : " << vtType << endl
      << "  fvpList" << endl;

    DLFLFaceVertexPtrList::const_iterator first, last;
    int i=0;
  
    first = fvpList.begin(); last = fvpList.end();
    while ( first != last ) {
      o << "    " << i++ << " : " << (*first) << endl;
      ++first;
    }
    o << endl;
  }

  // Reset type of vertex, face-vertices and edges associated with this vertex
  void DLFLVertex::resetTypeDeep(void) {
    // Reset vertex type
    resetType();

    // Go through face-vertex list and reset type of each face-vertex
    // and the edge starting at that face-vertex
    DLFLFaceVertexPtrList::iterator first, last;
    DLFLFaceVertexPtr fvp;
    first = fvpList.begin(); last = fvpList.end();
    while ( first != last ) {
      fvp = (*first); ++first;
      fvp->resetType();
      fvp->getEdgePtr()->resetType();
    }
  }

  // Set the texture coordinates for all FaceVertexes referring to this vertex
  void DLFLVertex::setTexCoords(const Vector2d& texcoord) {
    DLFLFaceVertexPtrList::iterator first, last;

    first = fvpList.begin(); last = fvpList.end();
    while ( first != last ) {
      (*first)->texcoord = texcoord;
      ++first;
    }
  }

  // Set the color values for all FaceVertexes referring to this vertex
  void DLFLVertex::setColor(const RGBColor& color) {
    DLFLFaceVertexPtrList::iterator first, last;

    first = fvpList.begin(); last = fvpList.end();
    while ( first != last ) {
      (*first)->color = color;
      ++first;
    }
  }

  // Set the normals for all FaceVertexes referring to this vertex
  Vector3d DLFLVertex::computeNormal( bool set ) {
    DLFLFaceVertexPtrList::iterator first, last;
    Vector3d normal;
    int i=0;

    first = fvpList.begin(); last = fvpList.end();
    while ( first != last ) {
      normal += (*first)->computeNormal();
      ++first; ++i;
    }
    normal /= i;
    if ( set ) {
      first = fvpList.begin();
      while ( first != last ) {
        (*first)->normal = normal;
        ++first;
      }
    }
    return normal;
  }

  // Compute the normals for all FaceVertexes referring to this vertex
  // Update the vertex normal and return it
  Vector3d DLFLVertex::updateNormal(bool recompute) {
    DLFLFaceVertexPtrList::iterator first, last;

    first = fvpList.begin(); last = fvpList.end();
    normal.reset();
    if ( recompute )
      while ( first != last ) {
        normal += (*first)->computeNormal();
        ++first;
      }
    else
      while ( first != last ) {
        normal += (*first)->normal;
        ++first;
      }
     
    normalize(normal);
    return normal;
  }

  Vector3d DLFLVertex::getNormals(Vector3dArray& normals) {
    // Return normals at all corners in an Vector3dArray
    Vector3d avenormal;
    int numnormals = fvpList.size();
    if ( numnormals > 0 ) {
      normals.clear(); normals.reserve(numnormals);

      DLFLFaceVertexPtrList::const_iterator first, last;
      DLFLFaceVertexPtr fvp = NULL;
      first = fvpList.begin(); last = fvpList.end();
      while ( first != last ) {
  fvp = (*first); ++first;
  normals.push_back(fvp->normal); avenormal += fvp->normal;
      }
      avenormal /= numnormals;
    }
    return avenormal;
  }

  // Set tex coordinates, color and normal info for all FaceVertexes referring to this vertex
  void DLFLVertex::setFaceVertexProps(
      const Vector2d& texcoord, const RGBColor& color, const Vector3d& n) {
    DLFLFaceVertexPtrList::iterator first, last;

    first = fvpList.begin(); last = fvpList.end();
    while ( first != last ) {
      (*first)->texcoord = texcoord;
      (*first)->color = color;
      (*first)->normal = n;
      ++first;
    }
  }

  void DLFLVertex::vertexTrace(void) const {
    // Output all edges incident on this DLFLVertex in the specific rotation order

    // Pick a DLFLFaceVertex from the list - first one
    DLFLFaceVertexPtr fvptr1 = fvpList.front();

    if ( fvptr1 == NULL ) return;

    DLFLEdgePtr e1,e2;
    DLFLFaceVertexPtr fvptr2;
    DLFLFacePtr f;

    // Find Edge starting at this DLFLFaceVertex
    e1 = fvptr1->getEdgePtr();
    e2 = e1;

    do {
      cout << (*e2) << " ";

      // Find the other DLFLFaceVertex for this DLFLEdge
      fvptr2 = e2->getOtherFaceVertexPtr(fvptr1);
      if ( fvptr2 == NULL ) return;

      // Find the Face to which the second DLFLFaceVertex belongs
      f = fvptr2->getFacePtr();

      // Find the DLFLFaceVertex following the second DLFLFaceVertex in this Face
      fvptr1 = f->nextFaceVertex(fvptr2);

      // Find Edge starting at this DLFLFaceVertex
      e2 = fvptr1->getEdgePtr();
    }
    while ( e2 != e1 );

    cout << endl;
  }

  int DLFLVertex::getEdges(DLFLEdgePtr ** edges) const {
    // Create an array of DLFLEdge's incident on this DLFLVertex. The number of DLFLEdges
    // is returned. Memory will be allocated inside this function, which should
    // be freed by the caller. Pass a pointer to the array (DLFLEdge ** or DLFLEdgePtr *)
    // Equivalent to a vertex trace
    // In case of an error -1 is returned, but memory is not freed

    int num_edges = fvpList.size();
    int i = 0;

    if ( num_edges <= 0 ) return -1;

    // Allocate memory for the DLFLEdgePtr array
    *edges = new DLFLEdgePtr[num_edges];
  
    DLFLFaceVertexPtrList::const_iterator first, last;
    DLFLFaceVertexPtr fvp = NULL;
    first = fvpList.begin(); last = fvpList.end();
    while ( first != last ) {
      fvp = (*first); ++first;
      edges[0][i++] = fvp->getEdgePtr();
    }
    return num_edges;
  }

  void DLFLVertex::getEdges(DLFLEdgePtrArray& edges) const {
    // Return edges in an DLFLEdgePtrArray
    // int size;
    // try {
    //   throw fvpList.size();
    // }
    // catch (char c){
    //   std::cout << c << " \t char exception\n";      
    // }    
    // catch (int cause){
    //   std::cout << cause << " \t int exception\n";      
    // }
    // catch (...){
    //   std::cout << "default exception\n";
    // }

    if (fvpList.size() > 0){
      edges.clear(); edges.reserve(fvpList.size());

      DLFLFaceVertexPtrList::const_iterator first, last;
      DLFLFaceVertexPtr fvp = NULL;
      first = fvpList.begin(); last = fvpList.end();
      while ( first != last ) {
        fvp = (*first); ++first;
        edges.push_back(fvp->getEdgePtr());
      }
    }
  }

  DLFLEdgePtr DLFLVertex::getEdgeTo(DLFLVertexPtr vp) {
    // Get the Edge incident on this Vertex which connects to given Vertex
    // If no such edge exists, returns NULL
    DLFLEdgePtr ep, retep = NULL;
    DLFLFaceVertexPtrList::iterator first, last;
    DLFLFaceVertexPtr fvp = NULL, ofvp;
    first = fvpList.begin(); last = fvpList.end();
    while ( first != last ) {
      fvp = (*first); ++first;
      ep = fvp->getEdgePtr();
      if ( ep != NULL ) {
        ofvp = ep->getOtherFaceVertexPtr(fvp);
        if ( ofvp && ofvp->vertex == vp ) {
          retep = ep; break;
        }
      }
    }
    return retep;
  }
  
  void DLFLVertex::getFaceVertices(DLFLFaceVertexPtrArray& fvparray) {
    // Go through the face-vertex-pointer list and add each
    // face vertex pointer to the array
    fvparray.clear(); fvparray.reserve(fvpList.size());
    DLFLFaceVertexPtrList::iterator first, last;
    DLFLFaceVertexPtr fvp = NULL;
    first = fvpList.begin(); last = fvpList.end();
    while ( first != last ) {
      fvp = (*first); ++first;
      fvparray.push_back(fvp);
    }
  }

  void DLFLVertex::getOrderedFaceVertices(DLFLFaceVertexPtrArray& fvparray) {
    // Get the face vertices associated with this vertex in the clockwise rotation order
    fvparray.clear();

    DLFLFaceVertexPtr fvpstart = fvpList.front();
    if ( fvpstart == NULL ) return;

    fvparray.reserve(fvpList.size());
    DLFLFaceVertexPtr fvp = fvpstart;

    do {
      fvparray.push_back(fvp);
      fvp = fvp->vnext();
    } while ( fvp != fvpstart );
  }


  void DLFLVertex::getCornerAuxCoords(Vector3dArray& coords) const {
    coords.clear(); coords.reserve(fvpList.size());
    DLFLFaceVertexPtrList::const_iterator first, last;
    DLFLFaceVertexPtr fvp = NULL;
    first = fvpList.begin(); last = fvpList.end();
    while ( first != last ) {
      fvp = (*first); ++first;
      coords.push_back(fvp->getAuxCoords());
    }
  }

  void DLFLVertex::getOrderedCornerAuxCoords(Vector3dArray& coords) const {
    // Get the aux coords of face vertices associated with this vertex in the
    // clockwise rotation order.
    coords.clear();

    DLFLFaceVertexPtr fvpstart = fvpList.front();
    if ( fvpstart == NULL ) return;

    coords.reserve(fvpList.size());
    DLFLFaceVertexPtr fvp = fvpstart;

    do {
      coords.push_back(fvp->getAuxCoords());
      fvp = fvp->vnext();
    } while ( fvp != fvpstart );
  }


  void DLFLVertex::getFaces(DLFLFacePtrArray& fparray) {
    // Go through the face-vertex-pointer list and add
    // face pointer of each face vertex pointer to the array
    fparray.clear(); fparray.reserve(fvpList.size());
    DLFLFaceVertexPtrList::iterator first, last;
    DLFLFaceVertexPtr fvp = NULL;
    first = fvpList.begin(); last = fvpList.end();
    while ( first != last ) {
      fvp = (*first); ++first;
      fparray.push_back(fvp->getFacePtr());
    }
  }

  DLFLFaceVertexPtr DLFLVertex::getFaceVertexInFace(DLFLFacePtr fp) {
    // Get the FaceVertex belonging to the given face. If only 1 face-vertex
    // is there in the list, return that. If more than 1 exist but nothing
    // belongs to given face, return NULL
    DLFLFaceVertexPtrList::iterator first, last;
    DLFLFaceVertexPtr fvp, retfvp = NULL;
    first = fvpList.begin(); last = fvpList.end();
    if ( fvpList.size() == 1 )
      retfvp = (*first);
    else {
      while ( first != last ) {
  fvp = (*first); ++first;
  if ( fvp->getFacePtr() == fp ) {
    retfvp = fvp;
    break;
  }
      }
    }
    return retfvp;
  }

  DLFLFaceVertexPtr DLFLVertex::getFaceVertexWithPrev(DLFLVertexPtr vp) {
    // Get the FaceVertex which has the given Vertex before it in it's Face
    // If only 1 FaceVertex refers to this Vertex, will return that
    // If there are more than 1 and none of them satisfies the condition, returns NULL
    DLFLFaceVertexPtrList::iterator first, last;
    DLFLFaceVertexPtr fvp, retfvp = NULL;
    first = fvpList.begin(); last = fvpList.end();
    if ( fvpList.size() == 1 ) 
      retfvp = (*first);
    else {
      while ( first != last ) {
        fvp = (*first); ++first;
        if ( fvp->prev()->vertex == vp ) {
          retfvp = fvp;
          break;
        }
      }
    }
    return retfvp;
  }

  DLFLFaceVertexPtr DLFLVertex::getFaceVertexWithNext(DLFLVertexPtr vp) {
    // Get the FaceVertex which has the given Vertex after it in it's Face
    // If only 1 FaceVertex refers to this Vertex, will return that
    // If there are more than 1 and none of them satisfies the condition, returns NULL
    DLFLFaceVertexPtrList::iterator first, last;
    DLFLFaceVertexPtr fvp, retfvp = NULL;
    first = fvpList.begin(); last = fvpList.end();
    if ( fvpList.size() == 1 ) 
      retfvp = (*first);
    else {
      while ( first != last ) {
        fvp = (*first); ++first;
        if ( fvp->next()->vertex == vp ) {
          retfvp = fvp;
          break;
        }
      }
    }
    return retfvp;
  }

  DLFLFaceVertexPtr DLFLVertex::getBackFaceVertex(void) {
    // Get the FaceVertex which has the 'backface' flag set
    // If no such FaceVertex is found, returns NULL
    DLFLFaceVertexPtrList::iterator first, last;
    DLFLFaceVertexPtr fvp, retfvp = NULL;
    first = fvpList.begin(); last = fvpList.end();
    while ( first != last ) {
      fvp = (*first); ++first;
      if ( fvp->backface == true ) {
        retfvp = fvp; break;
      }
    }
    return retfvp;
  }

  void makeVertexUnique(DLFLVertexPtr dvp)
  {
    dvp->assignID();
  }

  void resetVertexType(DLFLVertexPtr dvp)
  {
    dvp->resetType();
  }

  void vertexTrace(DLFLVertexPtr vertexptr)
  {
    vertexptr->vertexTrace();
  }

  void vertexTrace(const DLFLVertex& vertex)
  {
    vertex.vertexTrace();
  }

  istream& operator >> (istream& i, DLFLVertex& dv)
  {
    // Read x,y,z coordinates.
    double x,y,z;
    i >> x >> y >> z;
    dv.coords.set(x,y,z);
    return i;
  }

  ostream& operator << (ostream& o, const DLFLVertex& dv)
  {
    // Only coordinates are written.
    double x,y,z;
    dv.coords.get(x,y,z);
    o << "v " << x << ' ' << y << ' ' << z << endl;
    return o;
  }

  /*static*/ void DLFLVertex::setLastID( uint id ) {
    if( id > suLastID )
      suLastID = id;
  };

  // Generate a new unique ID
  /*static*/ uint DLFLVertex::newID(void) {
    uint temp = suLastID;
    suLastID++;
    return temp;
  };
   
  void DLFLVertex::assignID(void) {
    uID = DLFLVertex :: newID();
    index = 0;
    ismarked = 0;
    isvisited = 0;
  };

  // Default constructor
  DLFLVertex::DLFLVertex() : coords(), flags(0), fvpList(), vtType(VTNormal),
                 auxcoords(), auxnormal(), normal() {
    assignID();
  }
  
  // 1 argument constructor
  DLFLVertex::DLFLVertex(const Vector3d& vec) : coords(vec), flags(0), fvpList(),
      vtType(VTNormal), auxcoords(), auxnormal(), normal() {
    assignID();
  }

  // 3 argument constructor
  DLFLVertex::DLFLVertex(double x, double y, double z) : coords(x,y,z), flags(0),
      fvpList(), vtType(VTNormal), auxcoords(), auxnormal(), normal() {
    assignID();
  }

  // Copy constructor
  DLFLVertex::DLFLVertex(const DLFLVertex& dv) : coords(dv.coords), flags(dv.flags),
      uID(dv.uID), index(dv.index), fvpList(dv.fvpList), vtType(dv.vtType),
      auxcoords(dv.auxcoords), auxnormal(dv.auxnormal), normal(dv.normal) {}

  // Destructor
  DLFLVertex::~DLFLVertex() {}

  // Assignment operator
  DLFLVertex& DLFLVertex::operator = (const DLFLVertex& dv) {
    coords = dv.coords;
    flags = dv.flags;
    uID = dv.uID;
    index = dv.index;
    fvpList = dv.fvpList;
    vtType = dv.vtType;
    auxcoords = dv.auxcoords;
    auxnormal = dv.auxnormal;
    normal = dv.normal;
    return (*this);
  }

  DLFLVertexPtr DLFLVertex::copy(void) const {
    DLFLVertexPtr newdv = new DLFLVertex(*this);
    return newdv;
  }

  void DLFLVertex::reset(void) {
    coords.reset(); flags = 0; fvpList.clear(); vtType = VTNormal;
    auxcoords.reset(); auxnormal.reset(); normal.reset();
  }

  void DLFLVertex::makeUnique(void) {
    assignID();
  }

  //--- Query functions ---//

  DLFLVertexType DLFLVertex::getType(void) const {
    return vtType;
  }

  Vector3d DLFLVertex::getCoords(void) const {
    return coords;
  }

  DLFLFaceVertexPtrList DLFLVertex::getFaceVertexList(void) const {
    return fvpList;
  }

  // Number of Edges incident on this Vertex = no. of Faces adjacent to this Vertex
  // = size of the FaceVertex list = valence of Vertex
  uint DLFLVertex::numEdges(void) const {
    return fvpList.size();
  }
   
  uint DLFLVertex::numFaces(void) const {
    return fvpList.size();
  }

  uint DLFLVertex::valence(void) const {
    return fvpList.size();
  }
   
  uint DLFLVertex::getID(void) const {
    return uID;
  }

  uint DLFLVertex::getIndex(void) const {
    return index;
  }

  Vector3d DLFLVertex::getAuxCoords(void) const {
    return auxcoords;
  }

  Vector3d DLFLVertex::getAuxNormal(void) const {
    return auxnormal;
  }
   
  Vector3d DLFLVertex::getNormal(void) const {
    return normal;
  }

  //--- Mutative functions ---//

  void DLFLVertex::setType(DLFLVertexType type) {
    vtType = type;
  }

  void DLFLVertex::resetType(void) {
    vtType = VTNormal;
  }

  void DLFLVertex::setFaceVertexList(const DLFLFaceVertexPtrList& list) {
    fvpList = list;
  }

  void DLFLVertex::setCoords(const Vector3d& p) {
    coords = p;
  }

  // Set the aux. coords
  void DLFLVertex::setAuxCoords(const Vector3d& p) {
    auxcoords = p;
  }

  void DLFLVertex::setAuxNormal(const Vector3d& n) {
    auxnormal = n;
  }
   
  // Add to the aux coords
  void DLFLVertex::addToAuxCoords(const Vector3d& p) {
    auxcoords += p;
  }

  void DLFLVertex::addToAuxNormal(const Vector3d& n) {
    auxnormal += n;
  }
   
  // Reset the aux coords
  void DLFLVertex::resetAuxCoords(void) {
    auxcoords.reset();
  }

  void DLFLVertex::resetAuxNormal(void) {
    auxnormal.reset();
  }
         
  // Set the normal for this vertex directly
  void DLFLVertex::setNormal(const Vector3d& n) {
    normal = n; normalize(normal);
  }

  // Retained for backward compatibility
  // Same as updateNormal but doesn't recompute the corner normals
  Vector3d DLFLVertex::averageNormal(void) {
    return updateNormal(false);
  }

  // Update the DLFLFaceVertexList by adding a new DLFLFaceVertexPtr
  void DLFLVertex::addToFaceVertexList(DLFLFaceVertexPtr fvptr) {
    cout << fvpList.size();
    fvpList.push_back(fvptr);
  }

  void DLFLVertex::deleteFromFaceVertexList(DLFLFaceVertexPtr fvptr) {
    //cout << "removing corner " << fvptr << " from " << this << endl;
    fvpList.remove(fvptr);
    //cout << " .... done!"<< endl;
  }

  void DLFLVertex::getCorners(DLFLFaceVertexPtrArray& fvparray) {
    getFaceVertices(fvparray);
  }

  void DLFLVertex::getOrderedCorners(DLFLFaceVertexPtrArray& fvparray) {
    getOrderedFaceVertices(fvparray);
  }

  // Apply a transformation specified by the matrix to the coordinates
  void DLFLVertex::transform(const Matrix4x4& tmat) {
    Vector4d tp(coords); tp[3] = 1.0;
    tp = tmat * tp;
    tp /= tp[3];
    coords = tp;
  }

  // Print out this Vertex
  void DLFLVertex::print(void) const {
    cout << "DLFLVertex " << uID << "," << vtType << "," << valence() << " : "
         << coords << endl;
  }

  // Write this vertex in DLFL format and set it's index value
  void DLFLVertex::writeDLFL(ostream& o, uint newindex) {
    double x,y,z;
    coords.get(x,y,z);
    o << "v " << x << ' ' << y << ' ' << z << endl;
    index = newindex;
  }
} // end namespace
