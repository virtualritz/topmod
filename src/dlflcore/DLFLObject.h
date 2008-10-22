/*** ***/

/**
 * \file DLFLObject.h
 */

#ifndef _DLFL_OBJECT_HH_
#define _DLFL_OBJECT_HH_

#include <map>
/**
* TRANSLATOR DLFL::DLFLObject
*   
* Necessary for lupdate.
*/

/**
* \brief DLFLObject class - The Doubly Linked Face List object classs
* 
* The Doubly Linked Face List object classs
* Contains a vertex list, edge list and a face list
* The lists actually store pointerss
* 
*/

#include "DLFLCommon.h"
#include "DLFLVertex.h"
#include "DLFLFaceVertex.h"
#include "DLFLEdge.h"
#include "DLFLFace.h"
#include "DLFLMaterial.h"
#include "Transform.h"

namespace DLFL {
class DLFLObject {
 public:
  /// Constructor
  DLFLObject();

  /// Destructor
  ~DLFLObject();

 protected:
  // Distinct ID for each instance
  static uint suLastID;
  // Generate a new unique ID
  static uint newID();

 public:
  static Transformation tr;                         // For doing GL transformations

  DLFLVertexPtrArray sel_vptr_array; // List of selected DLFLVertex pointers

  DLFLEdgePtrArray sel_eptr_array; // List of selected DLFLEdge pointers
  DLFLFacePtrArray sel_fptr_array; // List of selected DLFLFace pointers
  DLFLFaceVertexPtrArray sel_fvptr_array; // List of selected DLFLFaceVertex pointers

  void clearSelected();

  HashMap faceMap;
  HashMap edgeMap;

  static DLFLVertexPtrArray vparray;                // For selection
  static DLFLEdgePtrArray   eparray;                // For selection
  static DLFLFacePtrArray   fparray;                // For selection
  static DLFLFaceVertexPtrArray fvparray;           // For selection

  Vector3d           position;                      // Position of object
  Vector3d           scale_factor;                  // Scale of object
  Quaternion         rotation;                      // Rotation of object

  void removeVertex(DLFLVertexPtr vp);
  void removeEdge(DLFLEdgePtr ep);
  void removeFace(DLFLFacePtr fp);

  void computeNormals();

protected :
  DLFLVertexPtrList          vertex_list;           // The vertex list
  DLFLEdgePtrList            edge_list;             // The edge list
  DLFLFacePtrList            face_list;             // The face list
  DLFLMaterialPtrList        matl_list;             // Material list (for rendering)

  map<DLFLVertexPtr, DLFLVertexPtrList::iterator> vertex_idx;
  map<DLFLFacePtr, DLFLFacePtrList::iterator> face_idx;
  map<DLFLEdgePtr, DLFLEdgePtrList::iterator> edge_idx;
  //TMPatchFacePtrList patch_list;     // List of patch faces
  //int patchsize;         // Size of each patch
     
  uint uID;                                      // ID for this object
  char *mFilename;
  char *mDirname;
  // Assign a unique ID for this instance
  void assignID();

  // Free all the pointers in the lists and clear the lists
  void clearLists();

private :
  /// Copy Constructor - make proper copy, don't just copy pointers
  DLFLObject(const DLFLObject& dlfl);

  // Assignment operator
  DLFLObject& operator=(const DLFLObject& dlfl);

public :
  // Dump contents of this object
  void dump(ostream& o) const;

  //--- Query Functions ---//

  // Compute the genus of the mesh using Euler formula
  int genus() const;
  uint getID() const;
  size_t num_vertices() const;
  size_t num_edges() const;
  size_t num_faces() const;
  size_t num_materials() const;

  // Probably should switch to just using these in the future
  // then you could get rid of all these accessors below.
  const DLFLVertexPtrList& getVertexList() const;
  const DLFLEdgePtrList& getEdgeList() const;
  DLFLFacePtrList& getFaceList();

  map<DLFLFacePtr, DLFLFacePtrList::iterator>& getFaceIdx();
  map<DLFLEdgePtr, DLFLEdgePtrList::iterator>& getEdgeIdx();
  map<DLFLVertexPtr, DLFLVertexPtrList::iterator>& getVertexIdx();

  //-- List based access to the 3 lists --//
  DLFLVertexPtr firstVertex();
  DLFLEdgePtr firstEdge();
  DLFLFacePtr firstFace();
  DLFLMaterialPtr firstMaterial();

  DLFLVertexPtr lastVertex();
  DLFLEdgePtr lastEdge();
  DLFLFacePtr lastFace();
  DLFLMaterialPtr lastMaterial();

  DLFLVertexPtr findVertex(const uint vid);
  DLFLEdgePtr findEdge(const uint eid);
  DLFLFacePtr findFace(const uint fid);
  DLFLFaceVertexPtr findFaceVertex(const uint fvid);

  DLFLVertexPtrList::iterator beginVertex();
  DLFLVertexPtrList::iterator endVertex();

  DLFLEdgePtrList::iterator beginEdge();
  DLFLEdgePtrList::iterator endEdge();

  DLFLFacePtrList::iterator beginFace();
  DLFLFacePtrList::iterator endFace();

  DLFLMaterialPtrList::iterator beginMaterial();
  DLFLMaterialPtrList::iterator endMaterial();

  DLFLFacePtrList::reverse_iterator rbeginFace();
  DLFLFacePtrList::reverse_iterator rendFace();

  //--- Access the lists through arrays ---//
  void getVertices(DLFLVertexPtrArray& vparray);

  void getEdges(DLFLEdgePtrArray& eparray);

  void getFaces(DLFLFacePtrArray& face_array);

  //--- Terminal printing functions : useful for debugging ---//
  void print() const;

  void printVertexList() const;

  void printEdgeList() const;

  void printFaceList() const;
     
  void printFaces() const;
     
  //--- Mutative Functions ---//
  // Reset the whole object
  void reset();

  void makeVerticesUnique();

  void makeEdgesUnique();

  void makeFacesUnique();

  void makeUnique();

  void destroy();

  void assignTileTexEdgeFlags(int n);
  void assignTileTexCoords(int n);
  void randomAssignTexCoords();

  // Combine two DLFLObject instances into 1 object
  // Lists of second object are cleared. Otherwise when that object is destroyed,
  // pointers in this object will become invalid.
  void splice(DLFLObject& object);

  // Reverse the orientation of all faces in the object
  // This also requires reversing all edges in the object
  void reverse();

  void addVertex(const DLFLVertex& vertex);         // Insert a copy
  void addVertex(DLFLVertexPtr vertexptr);          // Insert a copy
  void addVertexPtr(DLFLVertexPtr vertexptr);

  void addEdge(const DLFLEdge& edge);               // Insert a copy
  void addEdge(DLFLEdgePtr edgeptr);                // Insert a copy
  void addEdgePtr(DLFLEdgePtr edgeptr);

  void addFace(const DLFLFace& face);               // Insert a copy
  void addFace(DLFLFacePtr faceptr);                // Insert a copy
  void addFacePtr(DLFLFacePtr faceptr);

  DLFLVertexPtr getVertexPtr(uint index) const;
     
  DLFLVertexPtr getVertexPtrID(uint id) const;

  void updateEdgeList() ;

  // Check if an edge exists between two given vertices
  bool edgeExists(DLFLVertexPtr vptr1, DLFLVertexPtr vptr2);

  // Check if an edge exists between vertices given by two corners
  // Simply calls above function
  bool edgeExists(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2) ;

  // Check if a given edge exists in the edge list. If it does pointer is set to that edge
  bool edgeExists(const DLFLEdge& e, DLFLEdgePtr& eptr) ;

  void addEdges(DLFLEdge * edges, int num_edges) ;

  void addEdgesWithoutCheck(DLFLEdge * edges, int num_edges) ;

  void updateFaceList() ;

  DLFLFacePtrArray createFace(const Vector3dArray& verts,
      DLFLMaterialPtr matl = NULL, bool set_type = false);

  DLFLFaceVertexPtr createPointSphere(
      const Vector3d& v, DLFLMaterialPtr matl = NULL);
  void removePointSphere(DLFLFaceVertexPtr fvp);

  // Compute the bounding box for this object
  void boundingBox(Vector3d& min, Vector3d& max) const;
  bool boundaryWalkID(uint faceId);
  void walk(uint faceId, vector<int> &verts, vector<int> &edges);
  //vector<int> vertWalk(uint faceId);
  //vector<int> edgeWalk(uint faceId);
  void boundaryWalk(uint face_index);
  void vertexTrace(uint vertex_index);

  void readObject(istream& i, istream &imtl = *static_cast<istream*>(NULL));
  void readObjectAlt(istream& i);
  void readDLFL(istream& i, istream &imtl = *static_cast<istream*>(NULL),
      bool clearold = true);
  bool readMTL(istream &i);
  bool writeMTL(ostream& o);
  
  void writeObject(ostream& o, ostream &omtl = *static_cast<ostream*>(NULL),
      bool with_normals = true, bool with_tex_coords = true);
  void writeDLFL(ostream& o, ostream &omtl = *static_cast<ostream*>(NULL),
      bool reverse_faces = false);
  void writeSTL(ostream& o);
  //!< added by dave - for LiveGraphics3D support to embed 3d models into html
  void writeLG3d(ostream& o, bool select = false);
  void setFilename(const char *filename) ;
  char* getFilename();
  
  void setDirname(const char *dirname) ;

  DLFLMaterialPtr findMaterial(const RGBColor& color) ;

  DLFLMaterialPtr findMaterial(const char *mtlname) ;

  void clearMaterials();

  DLFLMaterialPtr addMaterial(RGBColor color);

  void setColor(const RGBColor& col) ;;

  //-- Geometric Transformations --//
  
  // Freeze any stored transformations and reset the transformations
  // Order of application : rotation, scale, translate
  void freezeTransformations(void);

  // Apply GL transformations before rendering
  void transform(void) const;

  // Apply the transformations for this object on the given vector
  void transform(Vector3d& p);

}; // end class DLFLObject.
} // end namespace.
#endif /* _DLFL_OBJECT_HH_ */
