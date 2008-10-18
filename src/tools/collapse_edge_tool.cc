/*
 * collapse_edge_tool.cc
 *
 *  Created on: Sep 12, 2008
 *      Author: david.morris
 */

#include "collapse_edge_tool.h"
#include "../MainWindow.h"
class MainWindow;

CollapseEdgeTool::CollapseEdgeTool(QWidget *parent) {
  parent_ = parent;
  layout_ = new QGridLayout;
  layout_->setVerticalSpacing(1);
  layout_->setHorizontalSpacing(1);

  QLabel *no_options_label_ =
      new QLabel(tr("No Options for this tool."));
  layout_->addWidget(no_options_label_, 0, 0);

  layout_->setRowStretch(1, 1);
  layout_->setColumnStretch(1, 1);
  widget_ = new QWidget;
  widget_->setWindowTitle(tr("Collapse Edge"));
  widget_->setLayout(layout_);

  action_ = new QAction(tr("Collapse Edge"), parent_);
  action_->setIcon(QIcon(":/images/collapse_edge.png"));
  action_->setCheckable(true);
  action_->setStatusTip(tr("Collapse Edge"));
  action_->setToolTip(tr("Collapse Edge"));
  connect(action_, SIGNAL(triggered()), this, SLOT(Activate()));
}

void CollapseEdgeTool::Activate() {
  ((MainWindow*) parent_)->setToolOptions(widget_);
  ((MainWindow*) parent_)->setMode(MainWindow::CollapseEdge);
}

void CollapseEdgeTool::RetranslateUi(){

}
