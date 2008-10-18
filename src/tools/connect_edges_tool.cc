/*
 * connect_edges_tool.cc
 *
 *  Created on: Sep 12, 2008
 *      Author: david.morris
 */

#include "connect_edges_tool.h"
#include "../MainWindow.h"
class MainWindow;

ConnectEdgesTool::ConnectEdgesTool(QWidget *parent) {
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
  widget_->setWindowTitle(tr("Connect Edges"));
  widget_->setLayout(layout_);

  action_ = new QAction(tr("Connects Edges"), parent_);
  action_->setIcon(QIcon(":/images/connect_edges.png"));
  action_->setCheckable(true);
  action_->setStatusTip(tr("Connect Edges"));
  action_->setToolTip(tr("Connect Edges"));
  connect(action_, SIGNAL(triggered()), this, SLOT(Activate()));
}

void ConnectEdgesTool::Activate() {
  ((MainWindow*) parent_)->setToolOptions(widget_);
  ((MainWindow*) parent_)->setMode(MainWindow::ConnectEdges);
}

void ConnectEdgesTool::RetranslateUi(){

}
