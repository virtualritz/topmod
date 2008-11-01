/*** ***/

// Non-inline function definitions for class DLFLObject
// Subroutines that are related to connections between edges/faces

#include "hermite_connect_faces.h"
#include <DLFLConnect.h>
#include <DLFLCore.h>
#include <DLFLCoreExt.h>

namespace DLFL {

  static Handle* Handle::instance_ = NULL;
  /*static*/ Handle* Handle::instance() {
    return instance_;
  }

  /*static*/ Handle* Handle::instance(
      DLFLObjectPtr obj) {
    if (!instance_) {
      instance_ = new Handle(obj);
    }
    return instance_;
  }

  Handle::Handle(DLFLObjectPtr obj) {
    obj_ = obj;

    // Set default values for the parameters.
    numsegs_ = 10;
    wt1_ = 25.0;
    wt2_ = 25.0;
    maxconn_ = -1;
    numtwists_ = 0;
    pinch_ = 1;
    pinch_center_ = 0.5;
    bubble_ = 1.0;
    triangulate_newface_ = false;
    scherk_collins_ = false;
    sc_pinch_ = 1.0;
    sc_pinch_center_ = 0.5;
    sc_pinch_width_ = 1.0;
    hole_twist_ = 1;
    init_skip_ = 1;
    skip_ = 0;
    hole_seg_skip_ = 0;
    pinch_num_segs_ = 2;
  }

  void Handle::Create() {
    if ( numsegs_ == 1 ) {
      DLFLEdgePtrArray new_edges = connectFaces(obj_,fvptr1_,fvptr2_);
      ScherkCollinsHandle(obj_, new_edges, sc_pinch_, sc_pinch_center_, sc_pinch_width_,
                          hole_twist_, init_skip_, skip_, pinch_num_segs_);
      return;
    }

    // Do hermite interpolated handle. Centroid is interpolated using hermite interpolation
    // Vertices are computed in relation to the centroid with linear interpolation from first
    // face to second face

    // Reorder both faces so that they start at the selected corners
    // Reverse the second face before reordering for proper connection
    DLFLFacePtr f1, f2;
    f1 = fvptr1_->getFacePtr();
    f2 = fvptr2_->getFacePtr();

    // Instead of return from here, subdivide the longest edge in the face
    // with smaller size.
    if (f1->size() != f2->size()) {
      make_faces_same_size(obj_, f1, f2);
    }

    f1->reorder(fvptr1_);
    f2->reverse();
    f2->reorder(fvptr2_);
  
    // Get the coordinates of the vertices of the 2 faces
    f1->getVertexCoords(verts1_);
    f2->getVertexCoords(verts2_);

    Vector3dArray polygon_1 = verts1_;
    Vector3dArray polygon_2 = verts2_;

    // Get the normals to the 2 faces. Second face is already reversed
    n1_ = f1->computeNormal();
    n2_ = f2->computeNormal();

    // Reverse second face to take it back to it's original order
    f2->reverse();
    f2->reorder(fvptr2_);
  
    //--- Find interpolation parameters ---//
    // Find and store centroid of 2 polygons
    Vector3d cen1 = centroid(polygon_1);
    Vector3d cen2 = centroid(polygon_2);

    // Translate both polygons to origin
    translate(polygon_1, Vector3d(0, 0, 0)); 
    translate(polygon_2, Vector3d(0, 0, 0));

    // The plane to rotate both polygons to is found as follows.
    // Find the vector from the centroid of the second polygon to
    // the centroid of the first polygon. This will be the normal
    // to the plane
    Vector3d rotplane(cen2-cen1);
    normalize(rotplane);
    rotate(polygon_1, n1_, rotplane);
    rotate(polygon_2, n2_, rotplane);
  
    // The reference X and Y axes are chosen as follows
    // x-axis : vector from centroid (origin) to the midpoint of the last edge in the first polygon
    // y-axis : use x-axis and normal to the plane (rotplane) to find y axis
    // Note that both polygons are already centered at the origin and on the rotation plane
    Vector3d xref, yref;
    int numverts = polygon_1.size();

    xref = polygon_1[0] + polygon_1[numverts-1];
    normalize(xref);
    yref = rotplane % xref;

    // If either of the 2 polygons has a normal pointing in exactly the opposite direction
    // as the normal to the rotation plane, it can't be rotated to the rotation plane
    // and hence the handle can't be created without additional information
    if (Abs(n1_ * rotplane + 1.0) < ZERO || Abs(n2_ * rotplane + 1.0) < ZERO) {
      cout << "Normals are opposite. Can't create handle!" << endl;
      return;
    }

    // Resolve both polygons using xref and yref
    DoubleArray angle1, angle2, distance1, distance2;

    // resolvePolygon does the resize for the arrays
    resolvePolygon(polygon_1, xref, yref, angle1, distance1);
    resolvePolygon(polygon_2, xref, yref, angle2, distance2);

    // If the angle for the first point in the second polygon is greater than
    // the angle for the first point in the first polygon, and the difference
    // is greater than pi, subtract 2pi from all angles in second polygon
    // to avoid more than 1 twist
    if (angle2[0] - angle1[0] > M_PI) {
      for (int i=0; i < angle2.size(); ++i) angle2[i] -= 2.0*M_PI;
    }

    // If we want any extra twists, add multiples of 2pi to second polygon angles
    if (numtwists_ > 0) {
      for (int i=0; i < angle2.size(); ++i) angle2[i] += 2.0 * M_PI * numtwists_;
    }
       
    // Now use the resolution parameters to find intermediate polygons
    double t, dt = 1.0 / double(numsegs_);
    Vector3dArray verts;
    DoubleArray angle, distance;
    Vector3d p,v,dvdt;
    DLFLFacePtr nf2,nf3;
    DLFLFaceVertexPtr nfvp1,nfvp2,nfvp3;
    DLFLFacePtrList::reverse_iterator rfirst;
    DLFLMaterialPtr matl = f1->material();
    int numconnected = 0;

    //Fenghui: it makes no sense not to connect them all.
    //if ( maxconn_ == -1 )
    maxconn_ = numsegs_;

    nfvp1 = fvptr1_;
    verts.resize(numverts); angle.resize(numverts); distance.resize(numverts);

    // Multiply the normals by the weights for hermite interpolation
    Vector3d n1 = n1_ * wt1_;
    Vector3d n2 = n2_ * wt2_;
    double curve_length = hermiteCurveLength(cen1,n1,cen2,n2,8*numsegs_);
    double segment_length = curve_length / double(numsegs_);

    double pinch_scaling=1;
    double bubbleExp = (1/bubble_);
    double t2=0; double w0, w1, w2;
  
    t = 0.0; p = cen1; v = n1; dvdt = 6.0*(cen2-cen1) - 4.0*n1 - 2.0*n2;

    // pinchCenter,  bubble_
    int offset = 0;
    int num_segments_processed = 0;
    for (int i=1; i < numsegs_; ++i) {
      //dt = segment_length/(norm(v) + norm(dvdt)*t);
      t = double(i)*dt;
    
      //pinching factor
      //calculate t2 for bezier curve in pinching
      if(t <= pinch_center_) {
        //use the bezier equation for the first portion of the handle
        t2 = (t/pinch_center_); //t2 goes from 0 to 1
        t2 = pow(t2, bubbleExp); //the bubble factor affects the speed we travel on the bezier curve
        w0 = (1-t2)*(1-t2); //weight 0
        w1 = (2*(1-t2)*t2); //weight 1
        w2 = t2*t2; //weight 2
        pinch_scaling = w0 + (w1 * pinch_) + (w2 * pinch_); //weighted sum
      }
      else {
        //use the bezier equation for the second portion of the handle
        t2 = 1-((t-pinch_center_)/(1-pinch_center_)); //t2 goes from 0 to 1
        t2 = pow(t2, bubbleExp); //the bubble factor affects the speed we travel on the bezier curve
        w0 = (1-t2)*(1-t2); //weight 0
        w1 = (2*(1-t2)*t2); //weight 1
        w2 = t2*t2; //weight 2
        pinch_scaling = w0 + (w1 * pinch_) + (w2 * pinch_); //weighted sum
      }
    
      // Find linearly interpolated resolution parameters
      linearInterpolate(angle1,angle2,angle,t);
      linearInterpolate(distance1,distance2,distance,t);

      // Since angles are stored as differences, except for first angle
      // they have to be added to get correct angles
      //for (int j=1; j < numverts; ++j)
      //angle[j] += angle[j-1];

      // Reconstruct interpolated polygon using interpolated resolution parameters
      reconstructPolygon(verts,xref,yref,angle,distance);

      // Do hermite interpolation on centroids and normals to find interpolated point and normal
      hermiteInterpolate(cen1,n1,cen2,n2,p,v,dvdt,t);
      //scale reconstructed polygon by pinching factor
      //pinch_scaling=(1-t)*(1-t) + pinch*2*(1-t)*t + t*t;
      scale(verts,pinch_scaling);

      // Rotate reconstructed polygon from rotation plane (XY plane) to plane with v as normal
      // Rotation is done around centroid of polygon
      rotate(verts,rotplane,v);
       
      // Translate rotated polygon from origin to p
      translate(verts,p);

      // Create a new face using computed coordinates
      DLFLFacePtrArray new_faces = obj_->createFace(verts,matl);

      DLFLVertexPtrArray new_vertices = new_faces[0]->getVertices();
      segments_.push_back(new_vertices);
      // Get the pointers to the newly inserted faces
      rfirst = obj_->rbeginFace( );
      nf2 = (*rfirst); ++rfirst; nf3 = (*rfirst);

      // Get pointers to the first FaceVertex in each of these faces
      nfvp2 = nf2->firstVertex(); nfvp3 = nf3->firstVertex();

      // Connect nf1 and nf2
      if ( numconnected < maxconn_ ) {
        ++numconnected;
        if (triangulate_newface_) dualConnectFaces(obj_, nfvp1, nfvp2);
        else {
          DLFLEdgePtrArray new_edges = connectFaces(obj_,nfvp1,nfvp2);
        }
      }
      ++num_segments_processed;
       
      // Make nf3 be the new nf1
      nfvp1 = nfvp3;
    }
    // Make the last connection
    nf2 = f2;
    nfvp2 = fvptr2_;
    if ( numconnected < maxconn_ ) {
      if (triangulate_newface_) dualConnectFaces(obj_, nfvp1, nfvp2);
      else {
        DLFLEdgePtrArray new_edges = connectFaces(obj_,nfvp1,nfvp2);
      }
    }
  }

  /*
   * Caculate the positions of each new vertices in the handle and position
   * them.
   */
  void Handle::HermitePositionVertices() {
    if ( numsegs_ == 1 ) {
      // Nothing to position here.
      return;
    }
  
    //--- Find interpolation parameters ---//
    // Find and store centroid of 2 polygons
    Vector3dArray polygon_1 = verts1_;
    Vector3dArray polygon_2 = verts2_;

    Vector3d cen1 = centroid(polygon_1);
    Vector3d cen2 = centroid(polygon_2);

    // Translate both polygons to origin
    translate(polygon_1, Vector3d(0, 0, 0)); 
    translate(polygon_2, Vector3d(0, 0, 0));

    // The plane to rotate both polygons to is found as follows.
    // Find the vector from the centroid of the second polygon to
    // the centroid of the first polygon. This will be the normal
    // to the plane
    Vector3d rotplane(cen2-cen1);
    normalize(rotplane);
    rotate(polygon_1, n1_, rotplane);
    rotate(polygon_2, n2_, rotplane);
  
    // The reference X and Y axes are chosen as follows
    // x-axis : vector from centroid (origin) to the midpoint of the last edge in the first polygon
    // y-axis : use x-axis and normal to the plane (rotplane) to find y axis
    // Note that both polygons are already centered at the origin and on the rotation plane
    Vector3d xref, yref;
    int numverts = polygon_1.size();

    xref = polygon_1[0] + polygon_1[numverts-1];
    normalize(xref);
    yref = rotplane % xref;

    // If either of the 2 polygons has a normal pointing in exactly the opposite direction
    // as the normal to the rotation plane, it can't be rotated to the rotation plane
    // and hence the handle can't be created without additional information
    if (Abs(n1_ * rotplane + 1.0) < ZERO || Abs(n2_ * rotplane + 1.0) < ZERO) {
      cout << "Normals are opposite. Can't create handle!" << endl;
      return;
    }

    // Resolve both polygons using xref and yref
    DoubleArray angle1, angle2, distance1, distance2;

    // resolvePolygon does the resize for the arrays
    resolvePolygon(polygon_1, xref, yref, angle1, distance1);
    resolvePolygon(polygon_2, xref, yref, angle2, distance2);

    // If the angle for the first point in the second polygon is greater than
    // the angle for the first point in the first polygon, and the difference
    // is greater than pi, subtract 2pi from all angles in second polygon
    // to avoid more than 1 twist
    if (angle2[0] - angle1[0] > M_PI) {
      for (int i=0; i < angle2.size(); ++i) angle2[i] -= 2.0*M_PI;
    }

    // If we want any extra twists, add multiples of 2pi to second polygon angles
    if (numtwists_ > 0) {
      for (int i=0; i < angle2.size(); ++i) angle2[i] += 2.0 * M_PI * numtwists_;
    }
       
    // Now use the resolution parameters to find intermediate polygons
    double t, dt = 1.0 / double(numsegs_);
    Vector3dArray verts;
    DoubleArray angle, distance;
    Vector3d p,v,dvdt;
    DLFLFacePtr nf2,nf3;
    DLFLFaceVertexPtr nfvp1,nfvp2,nfvp3;
    DLFLFacePtrList::reverse_iterator rfirst;
    int numconnected = 0;

    //Fenghui: it makes no sense not to connect them all.
    //if ( maxconn_ == -1 )
    maxconn_ = numsegs_;

    nfvp1 = fvptr1_;
    verts.resize(numverts); angle.resize(numverts); distance.resize(numverts);

    // Multiply the normals by the weights for hermite interpolation
    Vector3d n1 = n1_ * wt1_;
    Vector3d n2 = n2_ * wt2_;
    double curve_length = hermiteCurveLength(cen1,n1,cen2,n2,8*numsegs_);
    double segment_length = curve_length / double(numsegs_);

    double pinch_scaling=1;
    double bubbleExp = (1/bubble_);
    double t2=0; double w0, w1, w2;
  
    t = 0.0; p = cen1; v = n1; dvdt = 6.0*(cen2-cen1) - 4.0*n1 - 2.0*n2;

    // pinchCenter,  bubble_
    int offset = 0;
    int num_segments_processed = 0;
    for (int i=1; i < numsegs_; ++i) {
      //dt = segment_length/(norm(v) + norm(dvdt)*t);
      t = double(i)*dt;
    
      //pinching factor
      //calculate t2 for bezier curve in pinching
      if(t <= pinch_center_) {
        //use the bezier equation for the first portion of the handle
        t2 = (t/pinch_center_); //t2 goes from 0 to 1
        t2 = pow(t2, bubbleExp); //the bubble factor affects the speed we travel on the bezier curve
        w0 = (1-t2)*(1-t2); //weight 0
        w1 = (2*(1-t2)*t2); //weight 1
        w2 = t2*t2; //weight 2
        pinch_scaling = w0 + (w1 * pinch_) + (w2 * pinch_); //weighted sum
      }
      else {
        //use the bezier equation for the second portion of the handle
        t2 = 1-((t-pinch_center_)/(1-pinch_center_)); //t2 goes from 0 to 1
        t2 = pow(t2, bubbleExp); //the bubble factor affects the speed we travel on the bezier curve
        w0 = (1-t2)*(1-t2); //weight 0
        w1 = (2*(1-t2)*t2); //weight 1
        w2 = t2*t2; //weight 2
        pinch_scaling = w0 + (w1 * pinch_) + (w2 * pinch_); //weighted sum
      }
    
      // Find linearly interpolated resolution parameters
      linearInterpolate(angle1,angle2,angle,t);
      linearInterpolate(distance1,distance2,distance,t);

      // Since angles are stored as differences, except for first angle
      // they have to be added to get correct angles
      //for (int j=1; j < numverts; ++j)
      //angle[j] += angle[j-1];

      // Reconstruct interpolated polygon using interpolated resolution parameters
      reconstructPolygon(verts,xref,yref,angle,distance);

      // Do hermite interpolation on centroids and normals to find interpolated point and normal
      hermiteInterpolate(cen1,n1,cen2,n2,p,v,dvdt,t);
      //scale reconstructed polygon by pinching factor
      //pinch_scaling=(1-t)*(1-t) + pinch*2*(1-t)*t + t*t;
      scale(verts,pinch_scaling);

      // Rotate reconstructed polygon from rotation plane (XY plane) to plane with v as normal
      // Rotation is done around centroid of polygon
      rotate(verts,rotplane,v);
       
      // Translate rotated polygon from origin to p
      translate(verts,p);

      // Place vertices in segments_[i] to the position in verts.
      DLFLVertexPtrArray vertices = segments_[i - 1];
      int j = 0;
      for (DLFLVertexPtrArray::iterator it = vertices.begin();
          it != vertices.end(); ++it) {
        (*it)->setCoords(verts[j]);
        ++j;
      }
    }
  }
  
  void Handle::Reset() {
    segments_.clear();
  }

} // end namespace
