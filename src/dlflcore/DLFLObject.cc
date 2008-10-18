/*** ***/
/**
 * \file DLFLObject.cc
 */

#include "DLFLObject.h"

namespace DLFL {

  uint DLFLObject::suLastID = 0;
  Transformation DLFLObject::tr;

  /// Constructor
  DLFLObject::DLFLObject()
    : position(), scale_factor(1), rotation(),
      vertex_list(), edge_list(), face_list(), /* patch_list(), patchsize(4)*/ 
      vertex_idx(), edge_idx(), face_idx() {
    assignID();
    // Add a default material
    matl_list.push_back(new DLFLMaterial("default",0.5,0.5,0.5));
    mFilename = NULL;
    mDirname = NULL;
  };

  /// Destructor
  DLFLObject::~DLFLObject() {
    clearLists();
    if(mFilename) { delete [] mFilename; mFilename = NULL; }
    if(mDirname) { delete [] mDirname; mDirname = NULL; }
  };

  // Generate a new unique ID
  /*static*/ uint DLFLObject::newID() {
    uint temp = suLastID;
    suLastID++;
    return temp;
  };

  void DLFLObject::clearSelected() {
    sel_vptr_array.clear();
    sel_eptr_array.clear();
    sel_fptr_array.clear();
    sel_fvptr_array.clear();
  };

  void DLFLObject::removeVertex(DLFLVertexPtr vp) {
    if (vertex_idx.find(vp) != vertex_idx.end()) vertex_list.erase(vertex_idx[vp]); 
    else vertex_list.remove(vp);
    vertex_idx.erase(vp);
  };

  void DLFLObject::removeEdge(DLFLEdgePtr ep) {
    edgeMap.erase(ep->getID());
    if (edge_idx.find(ep) != edge_idx.end()) edge_list.erase(edge_idx[ep]); 
    else edge_list.remove(ep);
    edge_idx.erase(ep);
  };

  void DLFLObject::removeFace(DLFLFacePtr fp) {
    faceMap.erase(fp->getID());
    if (face_idx.find(fp) != face_idx.end()) face_list.erase(face_idx[fp]); 
    else face_list.remove(fp);
    face_idx.erase(fp); 
  };

  void DLFLObject::assignID() {
    uID = DLFLObject::newID();
  };

  /// Copy Constructor - make proper copy, don't just copy pointers
  DLFLObject::DLFLObject(const DLFLObject& dlfl)
    : position(dlfl.position), scale_factor(dlfl.scale_factor), rotation(dlfl.rotation),
      vertex_list(dlfl.vertex_list), edge_list(dlfl.edge_list), face_list(dlfl.face_list), matl_list(dlfl.matl_list),
      //patch_list(dlfl.patch_list), patchsize(dlfl.patchsize),
      uID(dlfl.uID) {
    for (DLFLVertexPtrList::iterator it = vertex_list.begin();
        it != vertex_list.end(); ++it) vertex_idx[*it] = it;
    for (DLFLEdgePtrList::iterator it = edge_list.begin();
        it != edge_list.end(); ++it) edge_idx[*it] = it;
    for (DLFLFacePtrList::iterator it = face_list.begin();
        it != face_list.end(); ++it) face_idx[*it] = it;
  };

  // Assignment operator
  DLFLObject& DLFLObject::operator=(const DLFLObject& dlfl) {
    position = dlfl.position;
    scale_factor = dlfl.scale_factor;
    rotation = dlfl.rotation;

    // Destroy the existing object
    clearLists();

    // Copy the lists from the new object
    vertex_list = dlfl.vertex_list;
    edge_list = dlfl.edge_list;
    face_list = dlfl.face_list;
    matl_list = dlfl.matl_list;
    //patch_list = dlfl.patch_list;
    //patchsize = dlfl.patchsize;
    for (DLFLVertexPtrList::iterator it = vertex_list.begin();
        it != vertex_list.end(); ++it) vertex_idx[*it] = it;
    for (DLFLEdgePtrList::iterator it = edge_list.begin();
        it != edge_list.end(); ++it) edge_idx[*it] = it;
    for (DLFLFacePtrList::iterator it = face_list.begin();
        it != face_list.end(); ++it) face_idx[*it] = it;
 
    edgeMap = dlfl.edgeMap;
    faceMap = dlfl.faceMap;

    uID = dlfl.uID;
    return (*this);
  };
  // Free all the pointers in the lists and clear the lists
  void DLFLObject::clearLists() {
    clear(vertex_list);
    clear(edge_list);
    clear(face_list);
    clear(matl_list);
    face_idx.clear();
    edge_idx.clear();
    vertex_idx.clear();
    //destroyPatches();
    edgeMap.clear();
    faceMap.clear();
  };

  // Compute the genus of the mesh using Euler formula
  int DLFLObject::genus() const {
    int v = num_vertices();
    int e = num_edges();
    int f = num_faces();
    int g = 1 - (v - e +  f)/2;
    return g;
  };

  void DLFLObject::dump(ostream& o) const {
    o << "DLFLObject" << endl;

    int i=0;

    o << "VertexList" << endl;
  
    DLFLVertexPtrList::const_iterator vfirst=vertex_list.begin(), vlast=vertex_list.end();
  
    while (vfirst != vlast) {
      o << i << " : " << (*vfirst) << endl;
      (*vfirst)->dump(o);
      ++vfirst; ++i;
    }

    i = 0;
    o << "EdgeList" << endl;
    DLFLEdgePtrList::const_iterator efirst=edge_list.begin(), elast=edge_list.end();
    while (efirst != elast) {
      o << i << " : " << (*efirst) << endl;
      (*efirst)->dump(o);
      ++efirst; ++i;
    }
         
    i = 0;
    o << "FaceList" << endl;
    DLFLFacePtrList::const_iterator ffirst=face_list.begin(), flast=face_list.end();
    while (ffirst != flast) {
      o << i << " : " << (*ffirst) << endl;
      (*ffirst)->dump(o);
      ++ffirst; ++i;
    }
  }

  void DLFLObject::splice(DLFLObject& object) {
    // Combine 2 objects. The lists are simply spliced together.
    // Entities must be removed from the second object to prevent dangling pointers
    // when it is destroyed.
    vertex_list.splice(vertex_list.end(),object.vertex_list);
    edge_list.splice(edge_list.end(),object.edge_list);
    face_list.splice(face_list.end(),object.face_list);
    matl_list.splice(matl_list.end(),object.matl_list);
  }

  // Reverse the orientation of all faces in the object
  // This also requires reversing all edges in the object
  void DLFLObject::reverse(void)
  {
    // Reverse the edges first, since they depend on the ordering of the
    // original faces.
    DLFLEdgePtrList::iterator efirst=edge_list.begin(), elast=edge_list.end();
    while (efirst != elast) {
      (*efirst)->reverse();
      ++efirst;
    }
  
    DLFLFacePtrList::iterator ffirst=face_list.begin(), flast=face_list.end();
    while (ffirst != flast) {
      (*ffirst)->reverse();
      ++ffirst;
    }
  }

  bool DLFLObject::edgeExists(DLFLVertexPtr vptr1, DLFLVertexPtr vptr2) {
    // Check if an edge exists between two given vertices
    bool edgeexists = false;

    // Try to find an edge from vptr1 to vptr2
    if (vptr1->getEdgeTo(vptr2) != NULL) edgeexists = true;

    return edgeexists;
  }

  /**
   * Tile Texturing, written by Avneet, Ergun
   */

  void DLFLObject::assignTileTexEdgeFlags(int n) {
    // Assign flags to edge corners based on tiling factor
    // Flags are stored in the first component of the texture coordinate in each face vertex
    int randomvariable1, randomvariable2;
    DLFLEdgePtrList :: const_iterator first = edge_list.begin(), last = edge_list.end();
    while (first != last) {
      DLFLFaceVertexPtr fvp1,fvp2;     
      (*first)->getFaceVertexPointers(fvp1,fvp2);

      randomvariable1 = rand() % (n/2);
      randomvariable2 = rand() % 2;

      fvp1->texcoord[0] = 2*randomvariable1 + randomvariable2;
      fvp2->texcoord[0] = 2*randomvariable1 + ((randomvariable2+1) % 2);

      ++first;
    }
  }

  void DLFLObject::assignTileTexCoords(int n) {
    // Compute the texture coordinates for each corner in each face

    // Assign edge flags first
    assignTileTexEdgeFlags(n);

    // Compute texture coordinates using the edge flags
    DLFLFaceVertexPtrArray corners;
    int flag[10];
    double u,v;
	 
    DLFLFacePtrList :: const_iterator first = face_list.begin(), last = face_list.end();
    while (first != last) {
      (*first)->getCorners(corners);

      for (int i=0; i < corners.size(); ++i) {
				flag[i] = int(corners[i]->texcoord[0]);
      }
      for (int i=0; i < corners.size(); ++i) {
				u = (float)(n*flag[0] + flag[1]  + (int)(((i+1)%4) / 2)) / (n*n);

				// Subtract from 1.0 since image origin is at top-left, instead of bottom-left
				v = 1.0 - (float)(n*flag[2] + flag[3]  + (int)(i/2)) / (n*n); 
		   
				corners[i]->texcoord.set(u,v);
      }
      ++first;
    }
  }

  void DLFLObject::randomAssignTexCoords() {
    // FOR QUADS ONLY - randomly assign texture coordinates from a unit square to the 4 corners
    DLFLFacePtrList :: iterator first, last;
    first = face_list.begin(); last = face_list.end();
    while (first != last) {
      (*first)->randomAssignTexCoords();
      ++first;
    }
  }

  DLFLFacePtrArray DLFLObject::createFace(
      const Vector3dArray& verts, DLFLMaterialPtr matl, bool set_type) {
    // Create 2 new faces with the given vertex coordinates. The 2 faces will have the same
    // vertices and share the same edges, but will have opposite rotation orders.
    // This essentially creates a 2 manifold with 2 faces with no volume.
    int numverts = verts.size();
    DLFLFacePtr newface1, newface2;
    DLFLVertexPtr vptr, tempvptr;
    DLFLFaceVertex fv;
    DLFLFaceVertexPtr fvptr;

    if (matl == NULL) matl = firstMaterial();
  
    newface1 = new DLFLFace; newface2 = new DLFLFace;

    if (set_type) {
      newface1->setType(FTNew); newface2->setType(FTNew);
    }
  
    for (int i=0; i < numverts; ++i) {
      vptr = new DLFLVertex(verts[i]); if (i == 0) tempvptr = vptr;
      addVertexPtr(vptr);
      fv.setVertexPtr(vptr);
      newface1->addVertex(fv); newface2->addVertex(fv);
    }

    // Reverse one of the new faces
    newface2->reverse();

    // These updates have to be done before the reorder
    newface1->addFaceVerticesToVertices(); newface2->addFaceVerticesToVertices();

    // Reorder the second new face so that the first vertex is the first vertex in the array
    DLFLFaceVertexPtrList fvplist = tempvptr->getFaceVertexList();
    fvptr = fvplist.front();
    if (fvptr->getFacePtr() != newface2) fvptr = fvplist.back();
    newface2->reorder(fvptr);
  
    // Add the new faces to the list
    addFacePtr(newface1); addFacePtr(newface2);
    newface1->setMaterial(matl); newface2->setMaterial(matl);

    newface1->computeNormal(); newface2->computeNormal();

    // Create the new edges
    // Traverse the 2 faces in opposite orders
    DLFLFaceVertexPtr head1, current1;
    DLFLFaceVertexPtr head2, current2;
    DLFLEdgePtr eptr;

    head1 = newface1->front(); head2 = newface2->back();

    // Assume that head1 and head2 are not NULL
    current1 = head1; current2 = head2;
    eptr = new DLFLEdge(current1,current2);
    eptr->updateFaceVertices();
    addEdgePtr(eptr);
    current1 = current1->next(); current2 = current2->prev();
    while (current1 != head1 && current2 != head2) {
      eptr = new DLFLEdge(current1,current2,false);
      eptr->updateFaceVertices();
      addEdgePtr(eptr);
      current1 = current1->next(); current2 = current2->prev();
    }

		DLFLFacePtrArray newverts;
		newverts.push_back(newface1);
		newverts.push_back(newface2);

		return newverts;
  }

  DLFLFaceVertexPtr DLFLObject::createPointSphere(const Vector3d& v, DLFLMaterialPtr matl) {
    // Create a point sphere - a face with only 1 vertex
    DLFLFacePtr newface = new DLFLFace();

    if (matl == NULL) matl = firstMaterial();

    DLFLVertexPtr vp = new DLFLVertex(v);
    DLFLFaceVertexPtr fvp = new DLFLFaceVertex(vp,NULL);
    fvp->addSelfToVertex();
    newface->addVertexPtr(fvp);
    newface->setMaterial(matl);

    addVertexPtr(vp);
    addFacePtr(newface);

    return fvp;
  }

	void DLFLObject::removePointSphere(DLFLFaceVertexPtr fvp) {
		if(fvp->getEdgePtr() == NULL) {
      cout << "Removing point sphere" << endl;
			DLFLVertexPtr vp = fvp->getVertexPtr();
			DLFLFacePtr fp = fvp->getFacePtr();

			if(vp->numEdges() > 0)
				return;

      cout << "Removing vertex" << endl;
			removeVertex(vp);
			delete vp;
      cout << "Removing face" << endl;
			removeFace(fp);
			delete fp;

      cout << "Removing corner" << endl;
			delete fvp;
		}
	}

  void DLFLObject::boundingBox(Vector3d& min, Vector3d& max) const {
    DLFLVertexPtrList::const_iterator vf,vl;
    double minx,miny,minz;
    double maxx,maxy,maxz;
    double x,y,z;
    DLFLVertexPtr vptr;
  
    vf = vertex_list.begin(); vl = vertex_list.end();
    vptr = (*vf); ++vf;

    vptr->coords.get(minx,miny,minz);
    maxx = minx; maxy = miny; maxz = minz;
  
    while (vf != vl) {
      vptr = (*vf); ++vf;
      vptr->coords.get(x,y,z);

      (x < minx) ? minx = x : ((x > maxx) ? maxx = x : 1);
      (y < miny) ? miny = y : ((y > maxy) ? maxy = y : 1);
      (z < minz) ? minz = z : ((z > maxz) ? maxz = z : 1);
    }
    min.set(minx,miny,minz); max.set(maxx,maxy,maxz);
  }

  bool DLFLObject::boundaryWalkID(uint faceId) {
    DLFLFacePtr fp = findFace(faceId);
    if(fp) { 
      fp->boundaryWalk();
      return true;
    } else { return false; }
  }

  void DLFLObject::walk(uint faceId, vector<int> &verts, vector<int> &edges) { 
    DLFLFacePtr fp = findFace(faceId);
    //vector<int> verts;
    if(fp) { 
      verts = fp->vertexWalk();
      edges = fp->edgeWalk();
    }
    //return verts;
  }
	/*
		vector<int> DLFLObject::vertWalk(uint faceId) { 
    DLFLFacePtr fp = findFace(faceId);
    vector<int> verts;
    if(fp) { 
		verts = fp->vertexWalk();
    }
    return verts;
		}

		vector<int> DLFLObject::edgeWalk(uint faceId) {
    DLFLFacePtr fp = findFace(faceId);
    vector<int> edges;
    if(fp) { 
		edges = fp->edgeWalk();
    } 
    return edges;
		}
	*/
  void DLFLObject::boundaryWalk(uint face_index) {
    //Find the Face with the given face_index from the FaceList and do a boundary walk on it
    uint i = 0;
    DLFLFacePtrList::iterator first = face_list.begin();
    DLFLFacePtrList::iterator last = face_list.end();
    DLFLFacePtr faceptr = NULL;

    if (face_index > face_list.size()) return;
    while (first != last) {
      if (i == face_index) {
				faceptr = (*first);
				break;
      }
      ++first;

      ++i;
    }
    faceptr->boundaryWalk();
  }

  void DLFLObject::vertexTrace(uint vertex_index) {
    //Find the Vertex with the given index in the VertexList and do a vertex trace on it
    uint i = 0;
    DLFLVertexPtrList::iterator first = vertex_list.begin();
    DLFLVertexPtrList::iterator last = vertex_list.end();
    DLFLVertexPtr vertexptr = NULL;

    if (vertex_index > vertex_list.size()) return;
    while (first != last) {
      if (i == vertex_index) {
				vertexptr = (*first);
				break;
      }
      ++first;

      ++i;
    }
    vertexptr->vertexTrace();
  }

  DLFLVertexPtr DLFLObject::findVertex(const uint vid) {
    // Find a vertex with the given vertex id. Return NULL if none exists
    DLFLVertexPtrList::iterator first = vertex_list.begin(), last = vertex_list.end();
    DLFLVertexPtr sel = NULL;
    while (first != last) {
				if ((*first)->getID() == vid) {
            sel = (*first); break;
          }
				++first;
      }
    return sel;
  }

  DLFLEdgePtr DLFLObject::findEdge(const uint eid) {
    // Find an edge with the given edge id. Return NULL if none exists
    //DLFLEdgePtrList::iterator first = edge_list.begin(), last = edge_list.end();
    DLFLEdgePtr sel = NULL;
    /*while (first != last) {
      if((*first)->getID() == eid) {
				sel = (*first); 
				break;
      }
      ++first;
			}*/
		sel = (DLFLEdgePtr) edgeMap[eid];
    return sel;
  }
  
  DLFLFacePtr DLFLObject::findFace(const uint fid) {
    // Find a face with the given face id. Return NULL if none exists
    //DLFLFacePtrList::iterator first = face_list.begin(), last = face_list.end();
    DLFLFacePtr sel = NULL;
    /*while (first != last) {
			if ((*first)->getID() == fid) {
				sel = (*first); 
				break;
			}
			++first;
			}*/
		sel = (DLFLFacePtr) faceMap[fid];
    return sel;
  }

  DLFLFaceVertexPtr DLFLObject::findFaceVertex(const uint fvid) {
    // Find a face vertex with the given face vertex id. Return NULL if none exists
    DLFLFacePtrList::iterator first = face_list.begin(), last = face_list.end();
    DLFLFaceVertexPtr sel,tmp = NULL;
    while (first != last) {
			tmp = (*first)->findFaceVertexByID(fvid);
			if(tmp != NULL) {
				sel = tmp; 
				//break;
			}
			++first;
    }
    return sel;
  }

  void DLFLObject::addVertex(const DLFLVertex& vertex) {
    addVertexPtr(vertex.copy());
  }
     
  void DLFLObject::addVertex(DLFLVertexPtr vertexptr) {
    addVertexPtr(vertexptr->copy());
  }
     
  void DLFLObject::addEdge(const DLFLEdge& edge) {
    addEdgePtr(edge.copy());
  }
     
  void DLFLObject::addEdge(DLFLEdgePtr edgeptr) {
    addEdgePtr(edgeptr->copy());
  }
     
  void DLFLObject::addFace(const DLFLFace& face) {
    addFacePtr(face.copy());
  }

  void DLFLObject::addFace(DLFLFacePtr faceptr) {
    addFacePtr(faceptr->copy());
  }

  void DLFLObject::computeNormals() {
    DLFLVertexPtrList::iterator first, last;

    first = vertex_list.begin(); last = vertex_list.end();
    while (first != last) {
      (*first)->updateNormal();
      ++first;
    }
    DLFLFacePtrList::iterator ffirst, flast;

    ffirst = face_list.begin(); flast = face_list.end();
    while (ffirst != flast) {
      (*ffirst)->updateNormal();
      ++ffirst;
    }
  }
  /*
		void DLFLObject::deleteVertex(uint vertex_index) {
    // Find the VertexPtr for the given vertex_index from the VertexList and delete it
    if (vertex_index > vertex_list.size()) return;

    DLFLVertexPtrList::iterator i = vertex_list.begin();
    advance(i,vertex_index);
    deleteVertex(*i);
		}
  */

  uint DLFLObject::getID() const { return uID; };
  size_t DLFLObject::num_vertices() const { return vertex_list.size(); };
  size_t DLFLObject::num_edges() const { return edge_list.size(); };
  size_t DLFLObject::num_faces() const { return face_list.size(); };
  size_t DLFLObject::num_materials() const { return matl_list.size(); }

  // Probably should switch to just using these in the future
  // then you could get rid of all these accessors below.
  const DLFLVertexPtrList& DLFLObject::getVertexList() const { return vertex_list; };
  const DLFLEdgePtrList& DLFLObject::getEdgeList() const { return edge_list; };
  //const DLFLFacePtrList& getFaceList() const { return face_list; };
  // needed not const for subdivideAllFaces
  DLFLFacePtrList& DLFLObject::getFaceList() { return face_list; };

  map<DLFLFacePtr, DLFLFacePtrList::iterator>& DLFLObject::getFaceIdx() { return face_idx; };
  map<DLFLEdgePtr, DLFLEdgePtrList::iterator>& DLFLObject::getEdgeIdx() { return edge_idx; };
  map<DLFLVertexPtr, DLFLVertexPtrList::iterator>& DLFLObject::getVertexIdx() { return vertex_idx; };

  //-- List based access to the 3 lists --//
  DLFLVertexPtr DLFLObject::firstVertex() { return vertex_list.front(); }
  DLFLEdgePtr DLFLObject::firstEdge() { return edge_list.front(); }
  DLFLFacePtr DLFLObject::firstFace() { return face_list.front(); }
  DLFLMaterialPtr DLFLObject::firstMaterial() { return matl_list.front(); }

  DLFLVertexPtr DLFLObject::lastVertex() { return vertex_list.back(); };
  DLFLEdgePtr DLFLObject::lastEdge() { return edge_list.back(); };
  DLFLFacePtr DLFLObject::lastFace() { return face_list.back(); }
  DLFLMaterialPtr DLFLObject::lastMaterial() { return matl_list.back(); }

  DLFLVertexPtrList::iterator DLFLObject::beginVertex() { return vertex_list.begin(); }
  DLFLVertexPtrList::iterator DLFLObject::endVertex() { return vertex_list.end(); }

  DLFLEdgePtrList::iterator DLFLObject::beginEdge() { return edge_list.begin(); }
  DLFLEdgePtrList::iterator DLFLObject::endEdge() { return edge_list.end(); }

  DLFLFacePtrList::iterator DLFLObject::beginFace() { return face_list.begin(); }
  DLFLFacePtrList::iterator DLFLObject::endFace() { return face_list.end(); }

  DLFLMaterialPtrList::iterator DLFLObject::beginMaterial() { return matl_list.begin(); };
  DLFLMaterialPtrList::iterator DLFLObject::endMaterial() { return matl_list.end(); };

  DLFLFacePtrList::reverse_iterator DLFLObject::rbeginFace() { return face_list.rbegin(); }
  DLFLFacePtrList::reverse_iterator DLFLObject::rendFace() { return face_list.rend(); }

  //--- Access the lists through arrays ---//
  void DLFLObject::getVertices(DLFLVertexPtrArray& vparray) {
    vparray.clear(); vparray.reserve(vertex_list.size());
    DLFLVertexPtrList::const_iterator first = vertex_list.begin(), last = vertex_list.end();
    while (first != last) {
      vparray.push_back(*first);
      ++first;
    }
  };

  void DLFLObject::getEdges(DLFLEdgePtrArray& eparray) {
    eparray.clear(); eparray.reserve(edge_list.size());
    DLFLEdgePtrList::const_iterator first = edge_list.begin(), last = edge_list.end();
    while (first != last) {
      eparray.push_back(*first);
      ++first;
    }
  };

  void DLFLObject::getFaces(DLFLFacePtrArray& face_array) {
    face_array.clear();
    face_array.reserve(face_list.size());
    for (DLFLFacePtrList::const_iterator it = face_list.begin();
        it != face_list.end(); ++it) {
      face_array.push_back(*it);
    }
  };

  //--- Terminal printing functions : useful for debugging ---//
  void DLFLObject::print() const {
    // Print a summary of the DLFLObject
    cout << "Number of vertices : " << vertex_list.size() << endl;
    cout << "Number of faces : " << face_list.size() << endl;
    cout << "Number of edges : " << edge_list.size() << endl;
    cout << "Number of materials : " << matl_list.size() << endl;
    cout << "Genus : " << genus() << endl;
  };

  void DLFLObject::printVertexList() const {
    cout << "Vertex List" << endl;
    DLFLVertexPtrList::const_iterator first = vertex_list.begin(), last = vertex_list.end();
    while (first != last) {
      (*first)->print();
      ++first;
    }
  };

  void DLFLObject::printEdgeList() const {
    cout << "Edge List" << endl;
    DLFLEdgePtrList::const_iterator first = edge_list.begin(), last = edge_list.end();
    while (first != last) {
      (*first)->print();
      ++first;
    }
  };

  void DLFLObject::printFaceList() const {
    cout << "Face List" << endl;
    
    DLFLFacePtrList::const_iterator first = face_list.begin(), last = face_list.end();
    while (first != last) {
      cout << *(*first) << endl;
      ++first;
      }
  };
     
  void DLFLObject::printFaces() const {
    cout << "Faces" << endl;

    DLFLFacePtrList::const_iterator first = face_list.begin(), last = face_list.end();
    while (first != last) {
      (*first)->print();
      ++first;
    }
  };
     
  //--- Mutative Functions ---//
  // Reset the whole object
  void DLFLObject::reset() {
    position.reset(); scale_factor.set(1,1,1); rotation.reset();
    clearLists();
    // Add a default material
    matl_list.push_back(new DLFLMaterial("default",0.5,0.5,0.5));
  };

  void DLFLObject::makeVerticesUnique() {
    // Make vertices unique
    DLFLVertexPtrList::iterator vfirst=vertex_list.begin(), vlast=vertex_list.end();
    while (vfirst != vlast) {
      (*vfirst)->makeUnique();
      ++vfirst;
    }
  };

  void DLFLObject::makeEdgesUnique() {
    // Make edges unique
    DLFLEdgePtrList::iterator efirst=edge_list.begin(), elast=edge_list.end();
    while (efirst != elast) {
      edgeMap.erase((*efirst)->getID());
      (*efirst)->makeUnique();
      edgeMap[(*efirst)->getID()] = (unsigned int)(*efirst);
      ++efirst;
    }
  };

  void DLFLObject::makeFacesUnique() {
    // Make faces unique
    DLFLFacePtrList::iterator ffirst=face_list.begin(), flast=face_list.end();
    while (ffirst != flast) {
      faceMap.erase((*ffirst)->getID());
      (*ffirst)->makeUnique();
      faceMap[(*ffirst)->getID()] = (unsigned int)(*ffirst);
      ++ffirst;
    }
  };

  void DLFLObject::makeUnique() {
    assignID();
    makeVerticesUnique();
    makeEdgesUnique();
    makeFacesUnique();
  };

  void DLFLObject::destroy() {
    // Clear the DLFL structures
    clearLists();
  };

  void DLFLObject::addVertexPtr(DLFLVertexPtr vertexptr) {
    // Insert the pointer.
    // **** WARNING!!! **** Pointer will be freed when list is deleted
    // vertex_list.push_back(vertexptr);
    vertex_idx[vertexptr] = vertex_list.insert(vertex_list.end(), vertexptr);
  };

  void DLFLObject::addEdgePtr(DLFLEdgePtr edgeptr) {
    // Insert the pointer.
    // **** WARNING!!! **** Pointer will be freed when list is deleted
    // edge_list.push_back(edgeptr);
    edge_idx[edgeptr] = edge_list.insert(edge_list.end(), edgeptr);
    edgeMap[edgeptr->getID()] = (unsigned int)edgeptr;
  };

  void DLFLObject::addFacePtr(DLFLFacePtr faceptr) {
    // Insert the pointer.
    // **** WARNING!!! **** Pointer will be freed when list is deleted
    if (faceptr->material() == NULL)
      // If Face doesn't have a material assigned to it, assign the default material
      faceptr->setMaterial(matl_list.front());
    // face_list.push_back(faceptr);
    face_idx[faceptr] = face_list.insert(face_list.end(), faceptr);
    faceMap[faceptr->getID()] = (unsigned int)faceptr;
  };

  DLFLVertexPtr DLFLObject::getVertexPtr(uint index) const {
    if (index >= vertex_list.size()) return NULL;
    DLFLVertexPtrList::const_iterator i=vertex_list.begin();
    advance(i,index);
    return (*i);
  };
     
  DLFLVertexPtr DLFLObject::getVertexPtrID(uint id) const {
    DLFLVertexPtrList::const_iterator first=vertex_list.begin(), last=vertex_list.end();
    DLFLVertexPtr vptr = NULL;
    while (first != last) {
      if ((*first)->getID() == id) {
        vptr = (*first);
        break;
      }
      ++first;
    }
    return vptr;
  };

  void DLFLObject::updateEdgeList() {
    DLFLEdgePtrList::iterator first=edge_list.begin(), last=edge_list.end();
    while (first != last) {
      (*first)->updateFaceVertices();
      ++first;
    }
  };

  // Check if an edge exists between two given vertices
  bool DLFLObject::edgeExists(DLFLVertexPtr vptr1, DLFLVertexPtr vptr2);

  // Check if an edge exists between vertices given by two corners
  // Simply calls above function
  bool DLFLObject::edgeExists(DLFLFaceVertexPtr fvptr1, DLFLFaceVertexPtr fvptr2) {
    return edgeExists(fvptr1->vertex,fvptr2->vertex);
  };

  // Check if a given edge exists in the edge list. If it does pointer is set to that edge
  bool DLFLObject::edgeExists(const DLFLEdge& e, DLFLEdgePtr& eptr) {
    DLFLEdgePtrList::iterator first=edge_list.begin(), last=edge_list.end();
    while (first != last) {
      if ((*(*first)) == e) {
        eptr = *first;
        return true;
      }
      ++first;
    }
    eptr = NULL;
    return false;
  };

  void DLFLObject::addEdges(DLFLEdge * edges, int num_edges) {
    DLFLEdgePtr eptr;
  
    for (int i=0; i < num_edges; ++i) {
      if (edgeExists(edges[i],eptr) == false) {
        addEdge(edges[i]);
      } else {
        // If Edge already exists, then the second FaceVertexPtr in the Edge must
        // be changed to that from the new Edge with the same ID as the second one
        // in this Edge
        int id2 = (eptr->getFaceVertexPtr2())->getVertexID();
        int eid1 = (edges[i].getFaceVertexPtr1())->getVertexID();

        if (eid1 == id2) eptr->setFaceVertexPtr2(edges[i].getFaceVertexPtr1());
        else eptr->setFaceVertexPtr2(edges[i].getFaceVertexPtr2());
      }
      eptr = NULL;
    }
  };

  void DLFLObject::addEdgesWithoutCheck(DLFLEdge * edges, int num_edges) {
    for (int i=0; i < num_edges; ++i)
      addEdge(edges[i]);
  };

  void DLFLObject::updateFaceList() {
    DLFLFacePtrList::iterator first=face_list.begin(), last=face_list.end();
    while (first != last) {
      (*first)->updateFacePointers();
      (*first)->addFaceVerticesToVertices();
      ++first;
    }
  };

  void DLFLObject::setFilename(const char *filename) { 
    // if(mFilename) { delete [] mFilename; mFilename = NULL; }
    // mFilename = filename; 
     if (filename) {
        mFilename = new char[strlen(filename)+1]; strcpy(mFilename,filename);
      } else {
        mFilename = new char[8]; strcpy(mFilename,"default");
      }
  };

  char* DLFLObject::getFilename() { return mFilename; }
  
  void DLFLObject::setDirname(const char *dirname) { 
     if (dirname) {
        mDirname = new char[strlen(dirname)+1]; strcpy(mDirname,dirname);
      } else {
        mDirname = new char[3]; strcpy(mDirname,"$HOME");
      }
  };

  DLFLMaterialPtr DLFLObject::findMaterial(const RGBColor& color) {
    DLFLMaterialPtr matl = NULL;
    DLFLMaterialPtrList::iterator first, last;
    first = matl_list.begin(); last = matl_list.end();
    while (first != last) {
      if ((*first)->equals(color)) {
        matl = (*first); break;
      }
      ++first;
    }
    return matl;
  };

  DLFLMaterialPtr DLFLObject::findMaterial(const char *mtlname) {
    DLFLMaterialPtr matl = NULL;
    DLFLMaterialPtrList::iterator first, last;
    first = matl_list.begin(); last = matl_list.end();
    while (first != last) {
      if (mtlname && !strcasecmp((*first)->name,mtlname)) {
        matl = (*first); break;
      }
      ++first;
    }
    return matl;
  };


  void DLFLObject::clearMaterials() {
    //iterate through faces...
    // DLFLMaterialPtr mptr = new DLFLMaterial("default",0.5,0.5,0.5);
    // setColor(RGBColor(0.5,0.5,0.5));
    
    DLFLFacePtrList::iterator ffirst=face_list.begin(), flast=face_list.end();
    while (ffirst != flast) {
      (*ffirst)->setMaterial(matl_list.front());
      ++ffirst;
    }    
    //clear materials
    while (matl_list.size() > 1) {
      // if (!((*matl_list.back()) == mptr)) {
      matl_list.pop_back();
      // }
    }
    //add the fresh blank gray material
    // matl_list.push_back(mptr);
  }

  DLFLMaterialPtr DLFLObject::addMaterial(RGBColor color) {
    //first search for the material to see if it exists already or not
    char matl_name[10];
    DLFLMaterialPtr mtl = findMaterial(color);
    
    // No matching material found
    if (mtl == NULL) {
      //add this as a new material
      sprintf(matl_name,"material%d",matl_list.size());
      matl_list.push_back(new DLFLMaterial(matl_name,color));
      mtl = matl_list.back();
      return mtl;
    } 
    else {
      //it already exists... return it
      return mtl;
    }    
  }

  void DLFLObject::setColor(const RGBColor& col) {
    // matl_list[0] is always the default material
    matl_list.front()->setColor(col);
  };

  //-- Geometric Transformations --//
  
  // Freeze any stored transformations and reset the transformations
  // Order of application : rotation, scale, translate
  void DLFLObject::freezeTransformations(void) {
    tr.reset();
    tr.scale(scale_factor);
    tr.rotate(rotation);
    tr.translate(position);
    Matrix4x4 tmat = tr.matrix();
    DLFLVertexPtrList :: iterator vfirst = vertex_list.begin(), vlast = vertex_list.end();
    DLFLVertexPtr vp;
    while (vfirst != vlast) {
      vp = (*vfirst); ++vfirst;
      vp->transform(tmat);
    }
  }

  // Apply GL transformations before rendering
  void DLFLObject::transform(void) const {
    tr.reset();
    tr.scale(scale_factor);
    tr.rotate(rotation);
    tr.translate(position);
    tr.apply();
  }

  // Apply the transformations for this object on the given vector
  void DLFLObject::transform(Vector3d& p) {
    tr.reset();
    tr.scale(scale_factor);
    tr.rotate(rotation);
    tr.translate(position);
    Vector4d tp(p); tp[3] = 1.0;
    tp = tr.matrix() * tp;
    tp /= tp[3];
    p = tp;
  };


  //const DLFLFacePtrList& getFaceList() const { return face_list; };
  // Free the memory allocated for the patches
  /*void destroyPatches() {
    TMPatchFacePtrList::iterator first = patch_list.begin(), last = patch_list.end();
    TMPatchFacePtr pfp = NULL;
    while (first != last) {
      pfp = (*first); ++first;
      delete pfp;
    }
    patch_list.clear();
    };*/
  // Build the list of patch faces
  /*void createPatches() {
    destroyPatches();
    DLFLFacePtrList::iterator ffirst = face_list.begin(), flast = face_list.end();
    DLFLFacePtr fp = NULL;
    TMPatchFacePtr pfp = NULL;
       
    while (ffirst != flast) {
      fp = (*ffirst); ++ffirst;
      pfp = new TMPatchFace(patchsize);
      pfp->setDLFLFace(fp); pfp->createPatches();
      patch_list.push_back(pfp);
      }

    // Adjust the edge points for all patches
    DLFLEdgePtrList::iterator efirst = edge_list.begin(), elast = edge_list.end();
    DLFLEdgePtr ep = NULL;
    DLFLFaceVertexPtr fvp1,fvp2;
    TMPatchPtr pp1, pp2;
    Vector3d p00,p01,p10,p11,ip;
    while (efirst != elast) {
      ep = (*efirst); ++efirst;
      ep->getCorners(fvp1,fvp2);
      pp1 = fvp1->getPatchPtr(); pp2 = fvp2->getPatchPtr();
      
      p00 = pp1->getControlPoint(2,0); p01 = pp2->getControlPoint(2,0);
      p10 = pp1->getControlPoint(3,1); p11 = pp2->getControlPoint(3,1);
      ip = intersectCoplanarLines(p00,p01,p10,p11);
      
      pp1->setControlPoint(3,0,ip); pp2->setControlPoint(3,0,ip);
      pp1->updateGLPointArray(); pp2->updateGLPointArray();
      
      pp1 = fvp1->next()->getPatchPtr(); pp2 = fvp2->next()->getPatchPtr();
      pp1->setControlPoint(0,3,ip); pp2->setControlPoint(0,3,ip);
      pp1->updateGLPointArray(); pp2->updateGLPointArray();
    }
    
    // Adjust the vertex points for 4-valence vertices
    DLFLVertexPtrList::iterator vfirst = vertex_list.begin(), vlast = vertex_list.end();
    DLFLVertexPtr vp = NULL;
    while (vfirst != vlast) {
      vp = (*vfirst); ++vfirst;
      if (vp->valence() == 4) {
  DLFLFaceVertexPtrArray vcorners;
  vp->getOrderedCorners(vcorners);
  pp1 = vcorners[0]->getPatchPtr(); pp2 = vcorners[2]->getPatchPtr();

  p00 = pp1->getControlPoint(1,0); p01 = pp2->getControlPoint(1,0);
  p10 = pp1->getControlPoint(0,1); p11 = pp2->getControlPoint(0,1);
  ip = intersectCoplanarLines(p00,p01,p10,p11);

  for (int i=0; i < 4; ++i) {
    pp1 = vcorners[i]->getPatchPtr();
    pp1->setControlPoint(0,0,ip);
    pp1->updateGLPointArray();
  }
      }
    }
    }
  };*/
} // end namespace
