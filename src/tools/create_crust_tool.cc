/*
 * create_crust_tool.cc
 *
 *  Created on: Sep 12, 2008
 *      Author: david.morris
 */

#include "create_crust_tool.h"


static CreateCrustTool * CreateCrustTool::instance = NULL;

/* static */ CreateCrustTool * CreateCrustTool::GetInstance(QWidget *parent) {
  if (instance == NULL) {
    instance = new CreateCrustTool(parent);
  }
  return instance;
}

// Implementation of the class CreateCrustTool.
// Constructor.
CreateCrustTool::CreateCrustTool(QWidget *parent) {
//	mRindModelingThicknessLayout = new QGridLayout;
//		// mRindModelingThicknessLayout->setMargin(0);
//		mRindModelingThicknessLayout->setVerticalSpacing(1);
//		mRindModelingThicknessLayout->setHorizontalSpacing(1);
//
//		//thickness
//		rindModelingThicknessLabel = new QLabel(this);
//		rindModelingThicknessSpinBox = createDoubleSpinBox(mRindModelingThicknessLayout, rindModelingThicknessLabel, tr("Thickness:"), -1.0, 1.0, 0.01, 0.5, 3, 0,0);
//		connect(rindModelingThicknessSpinBox, SIGNAL(valueChanged(double)), ((MainWindow*)mParent), SLOT(changeCrustThickness(double)));
//		//symmetric weights checkbox
//		rindModelingThicknessCleanupCheckBox = new QCheckBox(tr("Cleanup when peeling"),this);
//		rindModelingThicknessCleanupCheckBox->setChecked(Qt::Unchecked);
//		connect(rindModelingThicknessCleanupCheckBox, SIGNAL(stateChanged(int)), this, SLOT(toggleCrustCleanupFlag(int)));
//		mRindModelingThicknessLayout->addWidget(rindModelingThicknessCleanupCheckBox, 1,0,1,2);
//		//create crust button
//		rindModelingThicknessCreateButton = new QPushButton(tr("Create Crust"), this);
//		connect(rindModelingThicknessCreateButton, SIGNAL(clicked()), ((MainWindow*)mParent), SLOT(crustModeling3()));
//		mRindModelingThicknessLayout->addWidget(rindModelingThicknessCreateButton,2,0,1,2);
//		//set layout and add stretch
//		mRindModelingThicknessLayout->setRowStretch(3,1);
//		mRindModelingThicknessLayout->setColumnStretch(2,1);
//		mRindModelingThicknessWidget->setWindowTitle(tr("Rind Modeling (Thickness)"));
//		mRindModelingThicknessWidget->setLayout(mRindModelingThicknessLayout);
  parent_ = parent;
  layout_ = new QGridLayout;
  layout_->setVerticalSpacing(1);
  layout_->setHorizontalSpacing(1);

  translation_x_label_ = new QLabel(parent_);
  translation_x_spin_ = createDoubleSpinBox(
      layout_, translation_x_label_, tr("X-translate"),
      -100.0, 100.0, 0.1, 0.0, 3, 1, 0);
  connect(translation_x_spin_, SIGNAL(valueChanged(double)), this,
          SLOT(DoAction(double)));

  translation_y_label_ = new QLabel(parent_);
  translation_y_spin_ = createDoubleSpinBox(
      layout_, translation_y_label_, tr("Y-translate"),
      -100.0, 100.0, 0.1, 0.0, 3, 2, 0);
  connect(translation_y_spin_, SIGNAL(valueChanged(double)), this,
          SLOT(DoAction(double)));

  translation_z_label_ = new QLabel(parent_);
  translation_z_spin_ = createDoubleSpinBox(
      layout_, translation_z_label_, tr("Z-translate"),
      -100.0, 100.0, 0.1, 0.0, 3, 3, 0);
  connect(translation_z_spin_, SIGNAL(valueChanged(double)), this,
          SLOT(DoAction(double)));

  rotation_x_label_ = new QLabel(parent_);
  rotation_x_spin_ = createDoubleSpinBox(
      layout_, rotation_x_label_, tr("X-Rotate"),
      0, 360, 10, 0.0, 0, 4, 0);
  connect(rotation_x_spin_, SIGNAL(valueChanged(double)), this,
          SLOT(DoAction(double)));

  rotation_y_label_ = new QLabel(parent_);
  rotation_y_spin_ = createDoubleSpinBox(
      layout_, rotation_y_label_, tr("Y-Rotate"),
      0, 360, 10, 0.0, 0, 5, 0);
  connect(rotation_y_spin_, SIGNAL(valueChanged(double)), this,
          SLOT(DoAction(double)));

  rotation_z_label_ = new QLabel(parent_);
  rotation_z_spin_ = createDoubleSpinBox(
      layout_, rotation_z_label_, tr("Z-Rotate"),
      0, 360, 10, 0.0, 0, 6, 0);
  connect(rotation_z_spin_, SIGNAL(valueChanged(double)), this,
          SLOT(DoAction(double)));

  scaling_x_label_ = new QLabel(parent_);
  scaling_x_spin_ = createDoubleSpinBox(
      layout_, scaling_x_label_, tr("X-Scale"),
      0.1, 10.0, 0.1, 1.0, 3, 7, 0);
  connect(scaling_x_spin_, SIGNAL(valueChanged(double)), this,
          SLOT(DoAction(double)));

  scaling_y_label_ = new QLabel(parent_);
  scaling_y_spin_ = createDoubleSpinBox(
      layout_, scaling_y_label_, tr("Y-Scale"),
      0.1, 10.0, 0.1, 1.0, 3, 8, 0);
  connect(scaling_y_spin_, SIGNAL(valueChanged(double)), this,
          SLOT(DoAction(double)));

  scaling_z_label_ = new QLabel(parent_);
  scaling_z_spin_ = createDoubleSpinBox(
      layout_, scaling_z_label_, tr("Z-Scale"),
      0.1, 10.0, 0.1, 1.0, 3, 9, 0);
  connect(scaling_z_spin_, SIGNAL(valueChanged(double)), this,
          SLOT(DoAction(double)));

  reset_button_ = new QPushButton(tr("&Reset"));
  connect(reset_button_, SIGNAL(clicked()), this, SLOT(Reset()));
  layout_->addWidget(reset_button_, 10, 0, 1, 2);

  apply_button_ = new QPushButton(tr("&Apply"));
  connect(apply_button_, SIGNAL(clicked()), this, SLOT(Apply()));
  layout_->addWidget(apply_button_, 11, 0, 1, 2);

  layout_->setRowStretch(11,1);
  layout_->setColumnStretch(2,1);
  widget_ = new QWidget;
  widget_->setWindowTitle(tr("Geometry Mode"));
  widget_->setLayout(layout_);

  action_ = new QAction(tr("Geometry"), parent_);
  action_->setIcon(QIcon(":/images/transforms.png"));
  action_->setCheckable(true);
  action_->setStatusTip(tr("Enter Geometry Mode"));
  action_->setToolTip(tr("Geometry Mode"));
  connect(action_, SIGNAL(triggered()), this, SLOT(Activate()));

  active_ = true;
}

// Do the action, show the changes on screen.
void CreateCrustTool::DoAction() {

//	createCrust(true);
//	redraw();
  if (!active_) return;
  double translate_x = translation_x_spin_->value();
  double translate_y = translation_y_spin_->value();
  double translate_z = translation_z_spin_->value();
  Vector3d translate(translate_x, translate_y, translate_z);

  double scaling_x = scaling_x_spin_->value();
  double scaling_y = scaling_y_spin_->value();
  double scaling_z = scaling_z_spin_->value();
  Vector3d scaling(scaling_x, scaling_y, scaling_z);

  double rotate_x = rotation_x_spin_->value() * M_PI / 180;
  double rotate_y = rotation_y_spin_->value() * M_PI / 180;
  double rotate_z = rotation_z_spin_->value() * M_PI / 180;
  // cout << "Scaling is " << scaling << endl;

  GLWidget *glw = ((MainWindow*)parent_)->getActive();
  // Collect all select vertices.
  DLFLVertexPtrArray selected_vertices = glw->getSelectedVertices();
  DLFLEdgePtrArray selected_edges = glw->getSelectedEdges();
  DLFLFacePtrArray selected_faces = glw->getSelectedFaces();

  for (DLFLEdgePtrArray::iterator it = selected_edges.begin();
      it != selected_edges.end(); ++it) {
    DLFLVertexPtr u, v;
    (*it)->getVertexPointers(u, v);
    if (std::find(selected_vertices.begin(), selected_vertices.end(), u)
        == selected_vertices.end())
      selected_vertices.push_back(u);
    if (std::find(selected_vertices.begin(), selected_vertices.end(), v)
        == selected_vertices.end())
      selected_vertices.push_back(u);
  }

  for (DLFLFacePtrArray::iterator it = selected_faces.begin();
      it != selected_faces.end(); ++it) {
    DLFLFaceVertexPtr current = (*it)->front()->next();
    if (std::find(selected_vertices.begin(), selected_vertices.end(), (*it)->front()->getVertexPtr())
        == selected_vertices.end())
      selected_vertices.push_back((*it)->front()->getVertexPtr());
    while (current != (*it)->front()) {
      DLFLVertexPtr u = current->getVertexPtr();
      if (std::find(selected_vertices.begin(), selected_vertices.end(), u)
          == selected_vertices.end())
        selected_vertices.push_back(u);
      current = current->next();
    }
  }
  if (selected_vertices.size() <= 0) return;

  Vector3d centroid(0.0, 0.0, 0.0);
  for (DLFLVertexPtrArray::iterator it = selected_vertices.begin();
       it != selected_vertices.end(); ++it) {
    Vector3d p = (*it)->getCoords();
    if (init_positions_.find(*it) == init_positions_.end()) {
      init_positions_[*it] = p;
    }
    centroid += p;
  }
  Vector3d real_centroid = centroid / selected_vertices.size() + translate;
  for (DLFLVertexPtrArray::iterator it = selected_vertices.begin();
       it != selected_vertices.end(); ++it) {
    Vector3d p = (*it)->getCoords();
    // Translate them.
    p = init_positions_[*it] + translate;

    // Rotate them around the centroid.
    p = p * cos(rotate_x) + (1 - cos(rotate_x)) * (p * Vector3d(1, 0, 0)) * Vector3d(1, 0, 0) +
        sin(rotate_x) *  (p % Vector3d(1, 0, 0));
    p = p * cos(rotate_y) + (1 - cos(rotate_y)) * (p * Vector3d(0, 1, 0)) * Vector3d(0, 1, 0) +
        sin(rotate_y) *  (p % Vector3d(0, 1, 0));
    p = p * cos(rotate_z) + (1 - cos(rotate_z)) * (p * Vector3d(0, 0, 1)) * Vector3d(0, 0, 1) +
        sin(rotate_z) *  (p % Vector3d(0, 0, 1));

    // Scale them around the centroid.
    p = real_centroid + product(p - real_centroid, scaling);
    // Update the coordinates.
    (*it)->setCoords(p);
  }
  glw->recomputeNormals();
  ((MainWindow*)parent_)->redraw();
}

void CreateCrustTool::SetTranslation(double x, double y, double z) {
  translation_x_spin_->setValue(x);
  translation_y_spin_->setValue(y);
  translation_z_spin_->setValue(z);
}

// Apply the current changes.
void CreateCrustTool::Apply() {
  active_ = false;
  init_positions_.clear();
  Reset();
  active_ = true;
}
// Discard the current changes.
void CreateCrustTool::Reset() {
  translation_x_spin_->setValue(0.0);
  translation_y_spin_->setValue(0.0);
  translation_z_spin_->setValue(0.0);
  rotation_x_spin_->setValue(0.0);
  rotation_y_spin_->setValue(0.0);
  rotation_z_spin_->setValue(0.0);
  scaling_x_spin_->setValue(1.0);
  scaling_y_spin_->setValue(1.0);
  scaling_z_spin_->setValue(1.0);
}
// This is called when the user activate this tool.
void CreateCrustTool::Activate() {
  ((MainWindow*)parent_)->setToolOptions(widget_);
  ((MainWindow*)parent_)->setMode(MainWindow::NormalMode);
}

void CreateCrustTool::RetranslateUi(){

}
