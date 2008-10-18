/*
 * insert_edge_tool.cc
 *
 *  Created on: Sep 12, 2008
 *      Author: david.morris
 */

#include "delete_edge_tool.h"
#include "../MainWindow.h"
class MainWindow;

// Implementation of the class InsertEdgeTool.
// Constructor.
DeleteEdgeTool::DeleteEdgeTool(QWidget *parent) {
  parent_ = parent;
  layout_ = new QGridLayout;
  layout_->setVerticalSpacing(1);
  layout_->setHorizontalSpacing(1);

  //cleanup checkbox
  cleanup_check_ = new QCheckBox(tr("Cleanup"),this);
  connect(cleanup_check_, SIGNAL(stateChanged(int)),
          ((MainWindow*)parent_), SLOT(toggleDeleteEdgeCleanupFlag(int)));
  layout_->addWidget(cleanup_check_,0,0);

  layout_->setRowStretch(1, 1);
  layout_->setColumnStretch(1, 1);
  widget_ = new QWidget;
  widget_->setWindowTitle(tr("Delete Edge"));
  widget_->setLayout(layout_);

  action_ = new QAction(tr("Delete Edge"), parent_);
  action_->setIcon(QIcon(":/images/delete_edge.png"));
  action_->setCheckable(true);
  action_->setStatusTip(tr("Delete Edge"));
  action_->setToolTip(tr("Delete Edge"));
  connect(action_, SIGNAL(triggered()), this, SLOT(Activate()));
}

void DeleteEdgeTool::Activate() {
  ((MainWindow*) parent_)->setToolOptions(widget_);
  ((MainWindow*) parent_)->setMode(MainWindow::DeleteEdge);
}

void DeleteEdgeTool::RetranslateUi(){

}
