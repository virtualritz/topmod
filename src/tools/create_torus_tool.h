#ifndef CREATE_TORUS_TOOL_H
#define CREATE_TORUS_TOOL_H

#include <QtGui>
#include <QWidget>

#include "topmod_tool.h"

// Declaration of the class CreateTorusTool.
// We should move it to a file named create_torus_tool.h later.
// I will create a super class for all the tools. -- Fenghui
class CreateTorusTool: public TopModTool {
  Q_OBJECT // does it have to be a Q_OBJECT?

public:
  //QAction *action_;
  //QWidget *widget_;
  //QGridLayout *layout_;

  CreateTorusTool(QWidget *parent);
  void RetranslateUi();
protected:
  //QWidget *parent_;

  QDoubleSpinBox *cross_section_radius_spin_;
  QDoubleSpinBox *cross_section_rotation_spin_;
  QDoubleSpinBox *num_partial_twist_spin_;
  QDoubleSpinBox *num_segment_spin_;
  QDoubleSpinBox *num_twist_spin_;
  QDoubleSpinBox *pinch_spin_;
  QDoubleSpinBox *pinch_center_spin_;
  QDoubleSpinBox *pinch_width_spin_;
  QDoubleSpinBox *radius_spin_;
  QCheckBox *triangulate_new_faces_check_;
  QCheckBox *scherk_collins_check_;
  QDoubleSpinBox *size_cross_section_spin_;

  QDoubleSpinBox *hole_num_seg_spin_;
  QDoubleSpinBox *hole_init_skip_spin_;
  QDoubleSpinBox *hole_skip_spin_;
  QDoubleSpinBox *hole_twist_spin_;
  QDoubleSpinBox *hole_seg_skip_spin_;
  QPushButton *apply_button_;

public slots:

  void Apply();
  void Activate();
  void ToggleTriangulateNewFace(int);
  void ToggleScherkCollins(int);
  void ChangeSizeCrossSection(double);
};

#endif
