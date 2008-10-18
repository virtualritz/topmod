/*
 * subdivide_edge_tool.cc
 *
 *  Created on: Sep 12, 2008
 *      Author: david.morris
 */

#include "subdivide_edge_tool.h"
#include "../MainWindow.h"
class MainWindow;

// Constructor.
SubdivideEdgeTool::SubdivideEdgeTool(QWidget *parent) {
  parent_ = parent;
  layout_ = new QGridLayout;
  layout_->setVerticalSpacing(1);
  layout_->setHorizontalSpacing(1);

  //number of subdivisions spinbox
  num_subdivs_label_ = new QLabel(this);
  num_subdivs_spin_ = createDoubleSpinBox(layout_, num_subdivs_label_, tr(
      "# Subdivisions"), 1, 10, 1, 1, 0, 0, 0);
  connect(num_subdivs_spin_, SIGNAL(valueChanged(double)), ((MainWindow*) parent_), SLOT(changeNumSubDivs(double)));

  layout_->setRowStretch(2, 1);
  layout_->setColumnStretch(2, 1);
  widget_ = new QWidget;
  widget_->setWindowTitle(tr("Subdivide Edge"));
  widget_->setLayout(layout_);

  action_ = new QAction(tr("Subdivide Edge"), parent_);
  action_->setIcon(QIcon(":/images/subdivide_edge.png"));
  action_->setCheckable(true);
  action_->setStatusTip(tr("Subdivide Edge"));
  action_->setToolTip(tr("Subdivide Edge"));
  connect(action_, SIGNAL(triggered()), this, SLOT(Activate()));
}

void SubdivideEdgeTool::Activate() {
  ((MainWindow*) parent_)->setToolOptions(widget_);
  ((MainWindow*) parent_)->setMode(MainWindow::SubdivideEdge);
}

void SubdivideEdgeTool::RetranslateUi() {

}
