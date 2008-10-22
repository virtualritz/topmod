/*
 * create_crust_tool.h
 *
 *  Created on: Sep 12, 2008
 *      Author: david.morris
 */

#ifndef CREATE_CRUST_TOOL_H_
#define CREATE_CRUST_TOOL_H_

#include <QWidget>
#include <QGridLayout>
#include <QAction>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QString>
#include <QPushButton>

#include "topmod_tool.h"
#include "../MainWindow.h"
class MainWindow;

// Declaration of the class GeometricTool.
class CreateCrustTool : public TopModTool {
  Q_OBJECT // does it have to be a Q_OBJECT?

 public:
  QAction *action_;
  QWidget *widget_;
  QGridLayout *layout_;

  static CreateCrustTool * GetInstance(QWidget *parent);
  void SetTranslation(double x, double y, double z);
  void RetranslateUi();
 protected:
  CreateCrustTool(QWidget *parent);

  static CreateCrustTool * instance;

  QWidget *parent_;

  QLabel *translation_x_label_;
  QLabel *translation_y_label_;
  QLabel *translation_z_label_;
  QLabel *rotation_x_label_;
  QLabel *rotation_y_label_;
  QLabel *rotation_z_label_;
  QLabel *scaling_x_label_;
  QLabel *scaling_y_label_;
  QLabel *scaling_z_label_;

  QDoubleSpinBox *translation_x_spin_;
  QDoubleSpinBox *translation_y_spin_;
  QDoubleSpinBox *translation_z_spin_;
  QDoubleSpinBox *rotation_x_spin_;
  QDoubleSpinBox *rotation_y_spin_;
  QDoubleSpinBox *rotation_z_spin_;
  QDoubleSpinBox *scaling_x_spin_;
  QDoubleSpinBox *scaling_y_spin_;
  QDoubleSpinBox *scaling_z_spin_;

  QPushButton *reset_button_;
  QPushButton *apply_button_;

  map<DLFLVertexPtr, Vector3d> init_positions_;
  bool active_;

 public slots:
  // We might not need a parameter for DoAction since it will grab parameters
  // from the controls directly.
  void DoAction();
  void Apply();
  void Reset();
  void Activate();
};

#endif /* CREATE_CRUST_TOOL_H_ */
