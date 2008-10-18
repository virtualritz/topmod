/*
 * topmod_tool.cc
 *
 *  Created on: Sep 12, 2008
 *      Author: david.morris
 */



//#include "../MainWindow.h"
#include "topmod_tool.h"

TopModTool::TopModTool() {
}

//TopModTool::TopModTool(QWidget *parent) {
// parent_ = parent;
// layout_ = new QGridLayout;
// layout_->setVerticalSpacing(1);
// layout_->setHorizontalSpacing(1);
//}

QDoubleSpinBox *TopModTool::createDoubleSpinBox(QGridLayout *layout,
                                                QLabel *label, QString s,
                                                double low, double high,
                                                double step, double value,
                                                double decimals, int row,
                                                int col) {
  label->setText(s);
  QDoubleSpinBox *spinbox = new QDoubleSpinBox(this);
  spinbox->setAccelerated(true);
  spinbox->setRange(low, high);
  spinbox->setSingleStep(step);
  spinbox->setValue(value);
  spinbox->setDecimals(decimals);
  spinbox->setMaximumSize(75, 25);
  layout->addWidget(label, row, col);
  layout->addWidget(spinbox, row, col + 1);

  return spinbox;
}

void TopModTool::Activate() {
  //((MainWindow*) parent_)->setToolOptions(widget_);
}

void TopModTool::RetranslateUi(){

}
