/*
 * dup_component_tool.cc
 *
 *  Created on: Sep 12, 2008
 *      Author: david.morris
 */

#include <algorithm>
#include <queue>

#include "dup_component_tool.h"

// Implementation of the class DupComponentTool.
// Constructor.
DupComponentTool::DupComponentTool(QWidget *parent) {
  parent_ = parent;
  layout_ = new QGridLayout;
  layout_->setVerticalSpacing(1);
  layout_->setHorizontalSpacing(1);

  apply_button_ = new QPushButton(tr("&Duplicate"));
  connect(apply_button_, SIGNAL(clicked()), this, SLOT(Apply()));
  layout_->addWidget(apply_button_, 1, 0, 1, 1);

  layout_->setRowStretch(1, 1);
  layout_->setColumnStretch(1, 1);
  widget_ = new QWidget;
  widget_->setWindowTitle(tr("DupComponent Mode"));
  widget_->setLayout(layout_);

  action_ = new QAction(tr("Duplicate Component"), parent_);
  action_->setIcon(QIcon(":/images/transforms.png"));
  action_->setCheckable(true);
  action_->setStatusTip(tr("Enter DupComponent Mode"));
  action_->setToolTip(tr("DupComponent Mode"));
  connect(action_, SIGNAL(triggered()), this, SLOT(Activate()));
}

// Apply the current changes.
void DupComponentTool::Apply() {
  // Duplicate the current selected connected component.
  GLWidget *glw = ((MainWindow*) parent_)->getActive();
  DLFLObjectPtr object = ((MainWindow*) parent_)->GetObject();
  // Collect all selected vertices.
  DLFLVertexPtrSet old_vertices;
  DLFLEdgePtrSet old_edges;
  DLFLFacePtrSet old_faces;

  DLFLVertexPtrArray selected_vertices = glw->getSelectedVertices();
  DLFLEdgePtrArray selected_edges = glw->getSelectedEdges();
  DLFLFacePtrArray selected_faces = glw->getSelectedFaces();

  if (selected_vertices.size() > 0) {
    old_vertices.insert(*selected_vertices.begin());
  } else if (selected_edges.size() > 0) {
    DLFLVertexPtr u, v;
    (*selected_edges.begin())->getVertexPointers(u, v);
    old_vertices.insert(u);
  } else if (selected_faces.size() > 0) {
    DLFLFaceVertexPtr current = (*selected_faces.begin())->front()->next();
    old_vertices.insert((*selected_faces.begin())->front()->getVertexPtr());
  }
  if (old_vertices.size() <= 0) return;

  ((MainWindow*)parent_)->undoPush();

  //get all faces, edges, and vertices in the connected component.
  std::queue<DLFLVertexPtr> Q;
  Q.push(*old_vertices.begin());

  while (!Q.empty()) {
    DLFLVertexPtr u = Q.front();
    Q.pop();
    DLFLEdgePtrArray edges;
    u->getEdges(edges);
    //loop through edges, select all vertices connected to these edges
    for (DLFLEdgePtrArray::iterator eit = edges.begin(); eit != edges.end(); eit++) {
      old_edges.insert(*eit);
      DLFLFacePtr face1, face2;
      (*eit)->getFacePointers(face1, face2);
      old_faces.insert(face1);
      old_faces.insert(face2);

      DLFLVertexPtr vp1, vp2;
      (*eit)->getVertexPointers(vp1, vp2);
      if (old_vertices.count(vp1) <= 0) {
        old_vertices.insert(vp1);
        Q.push(vp1);
      }
      if (old_vertices.count(vp2) <= 0) {
        old_vertices.insert(vp2);
        Q.push(vp2);
      }
    }
  }
  double min_old_x = std::numeric_limits<float>::max();
  double max_old_x = std::numeric_limits<float>::min();
  for (DLFLVertexPtrSet::iterator it = old_vertices.begin(); it
      != old_vertices.end(); ++it) {
    Vector3d p = (*it)->getCoords();
    min_old_x = min(min_old_x, p.getCArray()[0]);
  }
  DLFLVertexPtrArray all_vertices(object->beginVertex(), object->endVertex());
  for (DLFLVertexPtrArray::iterator it = all_vertices.begin(); it
      != all_vertices.end(); ++it) {
    Vector3d p = (*it)->getCoords();
    max_old_x = max(max_old_x, p.getCArray()[0]);
  }

  // Now we have all the vertices, edges and faces.
  // Add vertices first, we need a map to reconstruct the new component.
  map<DLFLVertexPtr, DLFLVertexPtr> old2new_vertices;
  vector<DLFLVertexPtr> new_vertices;
  for (DLFLVertexPtrSet::iterator it = old_vertices.begin(); it
      != old_vertices.end(); ++it) {
    DLFLVertexPtr new_vertex = new DLFLVertex((*it)->getCoords());
    object->addVertexPtr(new_vertex);
    old2new_vertices[*it] = new_vertex;
    new_vertices.push_back(new_vertex);
  }

  map<int, map<int, DLFLEdgePtr> > new_edges;
  // Trace all faces to insert edges and corners.
  for (DLFLFacePtrSet::iterator it = old_faces.begin(); it != old_faces.end(); ++it) {
    DLFLFacePtr face = *it;

    DLFLFaceVertexPtr face_vertex = face->front();
    if (face_vertex == NULL) continue; // Ignore empty faces.
    DLFLFacePtr new_face = new DLFLFace;

    // New vertices.
    DLFLVertexPtr u, v;
    // New corner to create.
    DLFLFaceVertexPtr u_corner;
    do {
      u = old2new_vertices[face_vertex->getVertexPtr()];
      v = old2new_vertices[face_vertex->next()->getVertexPtr()];

      // Create corners for new_v.
      u_corner = new DLFLFaceVertex;
      u_corner->vertex = u;
      new_face->addVertexPtr(u_corner);
      DLFLEdgePtr new_edge = NULL;
      int uid = u->getID();
      int vid = v->getID();
      if (new_edges.find(uid) != new_edges.end() && new_edges[uid].find(vid)
          != new_edges[uid].end()) new_edge = new_edges[uid][vid];

      if (new_edge == NULL) { // Insert the new edge.
        new_edge = new DLFLEdge;
        // Does the order matter?
        // new_edge->setFaceVertexPointers(u_corner, v_corner);
        new_edge->setFaceVertexPtr1(u_corner, false);
        new_edges[uid][vid] = new_edge;
        new_edges[vid][uid] = new_edge;
      } else {
        new_edge->setFaceVertexPtr2(u_corner, false);
        new_edge->updateFaceVertices();
        object->addEdgePtr(new_edge);
      }

      face_vertex = face_vertex->next();
    } while (face_vertex != face->front());

    new_face->setMaterial(face->material());
    new_face->updateFacePointers();
    new_face->addFaceVerticesToVertices();
    object->addFacePtr(new_face);
  }

  glw->recomputeNormals();
  ((MainWindow*) parent_)->redraw();

  GeometricTool * geo_tool = GeometricTool::GetInstance(parent_);
  geo_tool->Activate();

  ((MainWindow*) parent_)->clearSelected();
  for (vector<DLFLVertexPtr>::iterator it = new_vertices.begin(); it
      != new_vertices.end(); ++it) {
    ((MainWindow*) parent_)->selectVertex(*it);
  }
  geo_tool->SetTranslation(max_old_x + 2 - min_old_x, 0, 0);
}

// This is called when the user activate this tool.
void DupComponentTool::Activate() {
  ((MainWindow*) parent_)->setToolOptions(widget_);
}

void DupComponentTool::RetranslateUi(){

}
