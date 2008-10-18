/*
 * splice_corners_tool.cc
 *
 *  Created on: Sep 12, 2008
 *      Author: david.morris
 */

#include "splice_corners_tool.h"
#include "../MainWindow.h"
class MainWindow;

SpliceCornersTool::SpliceCornersTool(QWidget *parent) {
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
  widget_->setWindowTitle(tr("Splice Corners"));
  widget_->setLayout(layout_);

  action_ = new QAction(tr("Splice Corners"), parent_);
  action_->setIcon(QIcon(":/images/splice_corners.png"));
  action_->setCheckable(true);
  action_->setStatusTip(tr("Splice Corners"));
  action_->setToolTip(tr("Splice Corners"));
  connect(action_, SIGNAL(triggered()), this, SLOT(Activate()));
}

void SpliceCornersTool::Activate() {
  ((MainWindow*) parent_)->setToolOptions(widget_);
  ((MainWindow*) parent_)->setMode(MainWindow::SpliceCorners);
}

void SpliceCornersTool::RetranslateUi(){

}
