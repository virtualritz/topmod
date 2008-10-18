/*
 * topmod_tool.h
 *
 *  Created on: Sep 12, 2008
 *      Author: david.morris
 */

#include <QWidget>
#include <QGridLayout>
#include <QAction>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QString>

#ifndef TOPMOD_TOOL_H
#define TOPMOD_TOOL_H

class TopModTool: public QWidget {
  Q_OBJECT // does it have to be a Q_OBJECT?

public:
  QAction *action_;
  QWidget *widget_;
  QGridLayout *layout_;

  //TopModTool(QWidget *parent);
  TopModTool();
  virtual void RetranslateUi();

  QDoubleSpinBox *createDoubleSpinBox(QGridLayout *layout, QLabel *label, QString s,
    double low, double high, double step, double value, double decimals, int row, int col);

protected:
  QWidget *parent_;

public slots:

  virtual void Activate();

};
#endif /* TOPMOD_TOOL_H_ */
