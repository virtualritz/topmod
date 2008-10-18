/*
 * create_torus_tool.cc
 *
 *  Created on: Sep 12, 2008
 *      Author: david.morris
 */

#include "create_torus_tool.h"
#include "../MainWindow.h"
class MainWindow;

// Implementation of the class CreateTorusTool.
// We should move it to a file named create_torus_tool.cc later.
// Constructor.
CreateTorusTool::CreateTorusTool(QWidget *parent) {
  parent_ = parent;
  layout_ = new QGridLayout;
  layout_->setVerticalSpacing(1);
  layout_->setHorizontalSpacing(1);

  QLabel *createTorusNumSegmentsLabel = new QLabel(this);
  num_segment_spin_ = createDoubleSpinBox(layout_, createTorusNumSegmentsLabel,
      tr("# Segments"), 3, 100, 1, 8, 0, 0, 0);

  QLabel *createTorusRadiusLabel = new QLabel(this);
  radius_spin_ = createDoubleSpinBox(layout_, createTorusRadiusLabel, tr(
      "Radius"), 0.01, 10, 0.1, 4.0, 3, 1, 0);

  QLabel *createTorusSizeCrossSectionLabel = new QLabel(this);
  size_cross_section_spin_ = createDoubleSpinBox(layout_,
      createTorusSizeCrossSectionLabel, tr("#Vertices in CS\n(Cross Sectoin)"),
      3, 64, 1, 8, 0, 2, 0);
  connect(size_cross_section_spin_, SIGNAL(valueChanged(double)),
      this, SLOT(ChangeSizeCrossSection(double)));

  QLabel *createTorusCrossSectionRadiusLabel = new QLabel(this);
  cross_section_radius_spin_ = createDoubleSpinBox(layout_,
      createTorusCrossSectionRadiusLabel, tr("Radius of CS"), 0.1, 5, 0.1, 2.0,
      3, 3, 0);

  QLabel *createTorusCrossSectionRotationLabel = new QLabel(this);
  cross_section_rotation_spin_ = createDoubleSpinBox(layout_,
      createTorusCrossSectionRotationLabel, tr("Rotation of CS"), 0, 360, 10,
      0, 0, 4, 0);

  QLabel *createTorusNumTwistLabel = new QLabel(this);
  num_twist_spin_ = createDoubleSpinBox(layout_, createTorusNumTwistLabel, tr(
      "# Twists"), 0, 10, 1, 0, 0, 5, 0);

  QLabel *createTorusNumPartialTwistLabel = new QLabel(this);
  num_partial_twist_spin_ = createDoubleSpinBox(layout_,
      createTorusNumPartialTwistLabel, tr("# Partial twist"), 0, 7, 1, 0, 0, 6,
      0);

  triangulate_new_faces_check_
      = new QCheckBox(tr("Triangulate New Faces"), this);
  connect(triangulate_new_faces_check_, SIGNAL(stateChanged(int)), this,
      SLOT(ToggleTriangulateNewFace(int)));
  layout_->addWidget(triangulate_new_faces_check_, 7, 0, 1, 2);

  scherk_collins_check_ = new QCheckBox(tr("Handles in Handles"), this);
  connect(scherk_collins_check_, SIGNAL(stateChanged(int)), this,
      SLOT(ToggleScherkCollins(int)));
  layout_->addWidget(scherk_collins_check_, 8, 0, 1, 2);

  QLabel *createTorusPinchLabel = new QLabel(this);
  pinch_spin_ = createDoubleSpinBox(layout_, createTorusPinchLabel,
      tr("Pinch"), -1, 10, 0.1, 1.0, 2, 9, 0);

  QLabel *createTorusPinchCenterLabel = new QLabel(this);
  pinch_center_spin_ = createDoubleSpinBox(layout_,
      createTorusPinchCenterLabel, tr("Pinch Center"), 0.1, 1, 0.1, 0.5, 3, 10,
      0);

  QLabel *createTorusPinchWidthLabel = new QLabel(this);
  pinch_width_spin_ = createDoubleSpinBox(layout_, createTorusPinchWidthLabel,
      tr("Pinch Width"), 0.001, 10, 0.1, 1.0, 3, 11, 0);

  QLabel *createTorusHoleNumSegmentsLabel = new QLabel(this);
  hole_num_seg_spin_ = createDoubleSpinBox(layout_,
      createTorusHoleNumSegmentsLabel, tr("#segs in holes"), 1, 10, 1, 2.0, 0,
      12, 0);

  QLabel *createTorusHoleInitSkipLabel = new QLabel(this);
  hole_init_skip_spin_ = createDoubleSpinBox(layout_,
      createTorusHoleInitSkipLabel, tr("Init skip"), 0, 10, 1, 1.0, 0, 13, 0);

  QLabel *createTorusHoleSkipLabel = new QLabel(this);
  hole_skip_spin_ = createDoubleSpinBox(layout_, createTorusHoleSkipLabel, tr(
      "Skip"), 0, 10, 1, 0, 0, 14, 0);

  QLabel *createTorusHoleTwistLabel = new QLabel(this);
  hole_twist_spin_ = createDoubleSpinBox(layout_, createTorusHoleTwistLabel,
      tr("Twist of holes"), 0, 10, 1, 1.0, 0, 15, 0);

  QLabel *createTorusHoleSegSkipLabel = new QLabel(this);
  hole_seg_skip_spin_ = createDoubleSpinBox(layout_,
      createTorusHoleSegSkipLabel, tr("Skip in segments"), 0, 10, 1, 0.0, 0,
      16, 0);

  apply_button_ = new QPushButton(tr("&Create Torus"));
  connect(apply_button_, SIGNAL(clicked()), this, SLOT(Apply()));
  layout_->addWidget(apply_button_, 17, 0, 1, 2);

  layout_->setRowStretch(17, 1);
  layout_->setColumnStretch(2, 1);
  widget_ = new QWidget;
  widget_->setWindowTitle(tr("Create Torus"));
  widget_->setLayout(layout_);

  action_ = new QAction(tr("Create Torus"), parent_);
  action_->setIcon(QIcon(":/images/prim_torus.png"));
  action_->setCheckable(true);
  action_->setStatusTip(tr("Enter Geometry Mode"));
  action_->setToolTip(tr("Geometry Mode"));
  connect(action_, SIGNAL(triggered()), this, SLOT(Activate()));

  pinch_spin_->setEnabled(false);
  pinch_center_spin_->setEnabled(false);
  pinch_width_spin_->setEnabled(false);
  hole_num_seg_spin_->setEnabled(false);
  hole_init_skip_spin_->setEnabled(false);
  hole_skip_spin_->setEnabled(false);
  hole_twist_spin_->setEnabled(false);
  hole_seg_skip_spin_->setEnabled(false);
}

void CreateTorusTool::ToggleTriangulateNewFace(int state) {
  if ((bool) state) {
    scherk_collins_check_->setChecked(Qt::Unchecked);
  }
}

void CreateTorusTool::ToggleScherkCollins(int state) {
  if ((bool) state) {
    triangulate_new_faces_check_->setChecked(Qt::Unchecked);
    pinch_spin_->setEnabled(true);
    pinch_center_spin_->setEnabled(true);
    pinch_width_spin_->setEnabled(true);
    hole_num_seg_spin_->setEnabled(true);
    hole_init_skip_spin_->setEnabled(true);
    hole_skip_spin_->setEnabled(true);
    hole_twist_spin_->setEnabled(true);
    hole_seg_skip_spin_->setEnabled(true);
  } else {
    pinch_spin_->setEnabled(false);
    pinch_center_spin_->setEnabled(false);
    pinch_width_spin_->setEnabled(false);
    hole_num_seg_spin_->setEnabled(false);
    hole_init_skip_spin_->setEnabled(false);
    hole_skip_spin_->setEnabled(false);
    hole_twist_spin_->setEnabled(false);
    hole_seg_skip_spin_->setEnabled(false);
  }
}

void CreateTorusTool::ChangeSizeCrossSection(double value) {
  num_partial_twist_spin_->setRange(0, value - 1);
  if ((int) value % 2 != 0) {
    scherk_collins_check_->setEnabled(false);
    pinch_spin_->setEnabled(false);
    pinch_center_spin_->setEnabled(false);
    pinch_width_spin_->setEnabled(false);
    hole_num_seg_spin_->setEnabled(false);
    hole_init_skip_spin_->setEnabled(false);
    hole_skip_spin_->setEnabled(false);
    hole_twist_spin_->setEnabled(false);
    hole_seg_skip_spin_->setEnabled(false);
  } else {
    scherk_collins_check_->setEnabled(true);
    pinch_spin_->setEnabled(true);
    pinch_center_spin_->setEnabled(true);
    pinch_width_spin_->setEnabled(true);
    hole_num_seg_spin_->setEnabled(true);
    hole_init_skip_spin_->setEnabled(true);
    hole_skip_spin_->setEnabled(true);
    hole_twist_spin_->setEnabled(true);
    hole_seg_skip_spin_->setEnabled(true);
  }
}

void CreateTorusTool::Activate() {
  ((MainWindow*) parent_)->setToolOptions(widget_);
}

// This function contains the core logic for creating a torus. This logic
// should be moved to a class called CreateTorusOperation later.
void CreateTorusTool::Apply() {
  MainWindow *main_window = (MainWindow*) parent_;
  DLFLObjectPtr object = main_window->GetObject();

  GLWidget *active = main_window->getActive();

  active->createPatchObject();
  // The next six parameters define properties of the torus to be created
  // Note: CS is an abbreviation for Cross Section
  int segmentCount = num_segment_spin_->value();
  int CS_VertCount = size_cross_section_spin_->value();
  int CS_Rotation = cross_section_rotation_spin_->value();
  float CS_Radius = cross_section_radius_spin_->value();
  float torusRadius = radius_spin_->value();
  int numTwists = num_twist_spin_->value();
  // numPartialTwists allows finer twisting control, allowed by
  // rotationally-symmetric cross sections.
  // <CS_VerCount> partial twists is equivalent to one full twist.
  int numPartialTwists = num_partial_twist_spin_->value();
  bool triangulate_new_faces = triangulate_new_faces_check_->isChecked();
  bool scherk_collins = scherk_collins_check_->isChecked();
  float scherk_collins_pinch = pinch_spin_->value();
  float scherk_collins_pinch_center = pinch_center_spin_->value();
  float scherk_collins_pinch_width = pinch_width_spin_->value();
  int scherk_collins_hole_init_skip = hole_init_skip_spin_->value();
  int scherk_collins_hole_skip = hole_skip_spin_->value();
  int scherk_collins_hole_seg_skip = hole_seg_skip_spin_->value();
  int scherk_collins_hole_num_segs = hole_num_seg_spin_->value();
  int scherk_collins_hole_twist = hole_twist_spin_->value();

  main_window->setModified(false);
  main_window->clearSelected();

  // these variables are used to communicate state between loop bodies
  vector<Vector3d> CS_VertList(CS_VertCount);

  // calculate vertex positions of a cross section centered at origin
  float ca = cos(CS_Rotation * M_PI / 360);
  float sa = sin(CS_Rotation * M_PI / 360);
  for (int i = 0; i < CS_VertCount; ++i) {
    float degree = 2 * M_PI * i / CS_VertCount;
    float y = CS_Radius * cos(degree);
    float z = CS_Radius * sin(degree);
    float y_real = y * ca - z * sa;
    float z_real = y * sa + z * ca;
    CS_VertList[i].set(0.0, y_real, z_real);
    // cout << CS_VertList[i] << endl;
  }

  // create cross section faces at regular intervals along torus
  object->reset();
  DLFLFacePtrArray basic_faces;
  DLFLVertexPtr first_vertex, prev_vertex, cur_vertex;
  for (int i = 0; i < segmentCount; ++i) {
    float c = cos(2 * M_PI * i / segmentCount);
    float s = sin(2 * M_PI * i / segmentCount);
    vector<Vector3d> CS_Trans(CS_VertCount);
    for (int j = 0; j < CS_VertCount; ++j) {
      // build about-to-be-transformed cross section vertex list
      CS_Trans[j].set(CS_VertList[j][0], CS_VertList[j][1], CS_VertList[j][2]);
      // rotate a bit more about x-axis to generate twist
      float degree = (numTwists + (float) numPartialTwists / CS_VertCount) * 2
          * M_PI * i / segmentCount;
      float cT = cos(degree);
      float sT = sin(degree);
      CS_Trans[j].set(CS_Trans[j][0],
          CS_Trans[j][1] * cT + CS_Trans[j][2] * sT, -CS_Trans[j][1] * sT
              + CS_Trans[j][2] * cT);
      // translate cross section center by torus radius
      CS_Trans[j].set(CS_Trans[j][0], CS_Trans[j][1], CS_Trans[j][2]
          + torusRadius);
      // rotate cross section about z axis
      CS_Trans[j].set(CS_Trans[j][0] * c - CS_Trans[j][2] * s, CS_Trans[j][1],
          CS_Trans[j][0] * s + CS_Trans[j][2] * c);
    }
    DLFLFacePtrArray new_faces = object->createFace(CS_Trans);
    basic_faces.push_back(new_faces[0]);
    basic_faces.push_back(new_faces[1]);
  }
  // connect each of the cross section faces with connectCorners() to create the torus
  // Some thing might go wrong if we have odd number of faces.
  int twist = 0;
  int num_segments_processed = 0;
  for (int i = 0; i < basic_faces.size() / 2; ++i) {
    DLFLFacePtr f1 = basic_faces[2 * i + 1];
    DLFLFacePtr f2 = basic_faces[(2 * i + 2) % basic_faces.size()];
    DLFLFaceVertexPtr fvp1 = f1->firstVertex();
    DLFLFaceVertexPtr fvp2 = f2->firstVertex();
    // dualConnectFaces(&object, f1->firstVertex(), f2->firstVertex());
    if ((2 * i + 2) % basic_faces.size() == 0) for (int j = 0; j < CS_VertCount
        - numPartialTwists; ++j)
      fvp1 = fvp1->next();
    if (triangulate_new_faces)
      dualConnectFaces(object, fvp1, fvp2);
    else {
      DLFLEdgePtrArray new_edges = connectFaces(object, fvp1, fvp2);
      if (scherk_collins && num_segments_processed
          % (scherk_collins_hole_seg_skip + 1) == 0) {
        ScherkCollinsHandle(object, new_edges, scherk_collins_pinch,
            scherk_collins_pinch_center, scherk_collins_pinch_width, twist,
            scherk_collins_hole_init_skip, scherk_collins_hole_skip,
            scherk_collins_hole_num_segs);
        twist += scherk_collins_hole_twist;
      }
    }
    ++num_segments_processed;
  }
  if (active->isInPatchMode()) {
    cout << "computing patches for torus..." << endl;
    active->recomputePatches();
  }
  active->recomputeNormals();
  active->redraw();
}

void CreateTorusTool::RetranslateUi(){

}
