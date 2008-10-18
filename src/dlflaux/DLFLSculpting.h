/*** ***/

#include <DLFLObject.h>
#include <DLFLCore.h>
#include <DLFLCoreExt.h>
#include "DLFLConvexHull.h"

namespace DLFL {

  static int s = 0;
  static DLFLConvexHull * convexhull = NULL;

  //new changes; these needs to be declared outside the function for local-cut
  static DLFLVertexPtr * sverts;
  static DLFLEdgePtr * sedges;
  static int cutcount;
	static DLFLFaceVertexPtr * newcorners;
	static DLFLEdgePtr * edges2del;
	// DLFLVertexPtr * sverts;
	// DLFLEdgePtr * sedges;
	// int cutcount;
	static int e2dsize, ncsize;
	
  void createConvexHull( DLFLObjectPtr obj );

  void createDualConvexHull( DLFLObjectPtr obj );
  DLFLObjectPtr createDualConvexHull( DLFLObjectPtr obj, const Vector3dArray &ovarray);

  void peelByPlane( DLFLObjectPtr obj, Vector3d normal,Vector3d P0);
	void localCut(DLFLObjectPtr obj, DLFLVertexPtr vp,Vector3d normal,Vector3d P0);
		
  void performCutting( DLFLObjectPtr obj, int type,float offsetE,float offsetV,bool global,bool selected) ;
	void cutSelectedFaces( DLFLObjectPtr obj, float offsetE,float offsetV,bool global=false,bool selected=false);
	void cutSelectedEdges( DLFLObjectPtr obj, float offsetE,float offsetV,bool global=false,bool selected=false);
	void cutSelectedVertices( DLFLObjectPtr obj, float offsetE,float offsetV,bool global=false,bool selected=false);
	
	int isMarked(DLFLVertexPtr vp);
	void autoMarkEdges(DLFLObjectPtr obj);
	int getCutIndex(DLFLVertexPtr vp);
	int getCutIndex(DLFLEdgePtr ep);
	
} // end namespace
